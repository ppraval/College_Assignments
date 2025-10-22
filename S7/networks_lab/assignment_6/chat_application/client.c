#include "common.h"
#include <signal.h>

// Global variables
int server_socket = -1;
int peer_socket = -1;
char my_username[MAX_USERNAME_LEN];
char chat_partner[MAX_USERNAME_LEN];
int in_chat = 0;
int running = 1;
int pending_request = 0; // Flag for pending incoming chat request
pthread_mutex_t chat_mutex = PTHREAD_MUTEX_INITIALIZER;
int my_port = 9000; // default peer listening port (global so handlers can access)

// Function to serialize message into buffer
void serialize_message(Message *msg, char *buffer) {
    sprintf(buffer, "%d|%s|%s|%s|%d", msg->type, msg->sender, msg->receiver, msg->content, msg->data);
}

// Function to deserialize buffer into message
void deserialize_message(char *buffer, Message *msg) {
    // Manual parsing to handle empty fields properly
    char *ptr = buffer;
    char *field_start;
    int field_num = 0;
    
    field_start = ptr;
    while (*ptr && field_num < 5) {
        if (*ptr == '|') {
            *ptr = '\0';  // Temporarily null-terminate
            
            switch(field_num) {
                case 0: msg->type = atoi(field_start); break;
                case 1: strcpy(msg->sender, field_start); break;
                case 2: strcpy(msg->receiver, field_start); break;
                case 3: strcpy(msg->content, field_start); break;
            }
            
            *ptr = '|';  // Restore
            field_start = ptr + 1;
            field_num++;
        }
        ptr++;
    }
    
    // Last field (data)
    if (field_num == 4) {
        msg->data = atoi(field_start);
    }
}

// Function to send message
void send_msg(int socket, Message *msg) {
    char buffer[BUFFER_SIZE];
    serialize_message(msg, buffer);
    printf("[DEBUG CLIENT] Sending: %s\n", buffer);
    send(socket, buffer, strlen(buffer), 0);
}

// Display user list
void display_user_list(const char *user_list) {
    printf("\n===========================================\n");
    printf("Active Users:\n");
    printf("===========================================\n");
    
    char temp[MAX_MESSAGE_LEN];
    strcpy(temp, user_list);
    
    char *token = strtok(temp, ",");
    int count = 1;
    while (token != NULL) {
        if (strcmp(token, my_username) != 0) {
            printf("%d. %s\n", count++, token);
        }
        token = strtok(NULL, ",");
    }
    
    if (count == 1) {
        printf("No other users online\n");
    }
    printf("===========================================\n\n");
}

// Handle peer-to-peer chat
void *p2p_chat_handler(void *arg) {
    int peer_fd = *((int *)arg);
    char buffer[BUFFER_SIZE];
    Message msg;
    
    while (in_chat) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes = recv(peer_fd, buffer, BUFFER_SIZE - 1, 0);
        
        if (bytes <= 0) {
            printf("\n[%s disconnected from chat]\n", chat_partner);
            printf(">> ");
            fflush(stdout);
            pthread_mutex_lock(&chat_mutex);
            in_chat = 0;
            pthread_mutex_unlock(&chat_mutex);
            break;
        }
        
        buffer[bytes] = '\0';
        deserialize_message(buffer, &msg);
        
        if (msg.type == MSG_CHAT_MESSAGE) {
            printf("\n[%s]: %s\n", msg.sender, msg.content);
            printf(">> ");
            fflush(stdout);
        } else if (msg.type == MSG_DISCONNECT) {
            printf("\n[%s has ended the chat]\n", chat_partner);
            printf(">> ");
            fflush(stdout);
            pthread_mutex_lock(&chat_mutex);
            in_chat = 0;
            pthread_mutex_unlock(&chat_mutex);
            break;
        }
    }
    
    close(peer_fd);
    return NULL;
}

// Handle incoming peer connections
void *peer_listener(void *arg) {
    int listen_port = *((int *)arg);
    free(arg);
    
    int listen_fd, new_peer;
    struct sockaddr_in addr, peer_addr;
    socklen_t addr_len = sizeof(peer_addr);
    
    // Create listening socket
    if ((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Peer socket creation failed");
        return NULL;
    }
    
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(listen_port);
    
    if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        perror("Peer bind failed");
        close(listen_fd);
        return NULL;
    }
    
    if (listen(listen_fd, 5) < 0) {
        perror("Peer listen failed");
        close(listen_fd);
        return NULL;
    }
    
    printf("Listening for peer connections on port %d\n", listen_port);
    
    while (running) {
        new_peer = accept(listen_fd, (struct sockaddr *)&peer_addr, &addr_len);
        
        if (new_peer < 0) {
            continue;
        }
        
        pthread_mutex_lock(&chat_mutex);
        if (in_chat) {
            close(new_peer);
            pthread_mutex_unlock(&chat_mutex);
            continue;
        }
        
        peer_socket = new_peer;
        pthread_mutex_unlock(&chat_mutex);
        
        // Receive initial message to know who's connecting
        char buffer[BUFFER_SIZE];
        int bytes = recv(new_peer, buffer, BUFFER_SIZE - 1, 0);
        if (bytes > 0) {
            buffer[bytes] = '\0';
            Message msg;
            deserialize_message(buffer, &msg);
            
            if (msg.type == MSG_CHAT_REQUEST) {
                strcpy(chat_partner, msg.sender);
                printf("\n===========================================\n");
                printf("Incoming chat request from: %s\n", msg.sender);
                printf("Type 'accept' to accept or 'reject' to decline\n");
                printf("===========================================\n");
                printf(">> ");
                fflush(stdout);
                
                // Set pending flag - main thread will handle accept/reject
                pthread_mutex_lock(&chat_mutex);
                if (!in_chat) {
                    peer_socket = new_peer;
                    pending_request = 1;
                } else {
                    // Already in chat, auto-reject
                    Message reply;
                    reply.type = MSG_CHAT_REJECT;
                    strcpy(reply.sender, my_username);
                    strcpy(reply.receiver, msg.sender);
                    strcpy(reply.content, "User busy");
                    send_msg(new_peer, &reply);
                    close(new_peer);
                }
                pthread_mutex_unlock(&chat_mutex);
            }
        }
    }
    
    close(listen_fd);
    return NULL;
}

// Handle server messages
void *server_handler(void *arg) {
    char buffer[BUFFER_SIZE];
    Message msg;
    
    while (running) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes = recv(server_socket, buffer, BUFFER_SIZE - 1, 0);
        
        if (bytes <= 0) {
            printf("\nDisconnected from server\n");
            running = 0;
            break;
        }
        
        buffer[bytes] = '\0';
        deserialize_message(buffer, &msg);
        
        switch (msg.type) {
            case MSG_REGISTER:
                printf("%s\n", msg.content);
                break;
                
            case MSG_USER_LIST:
                if (!in_chat) {
                    display_user_list(msg.content);
                    printf(">> ");
                    fflush(stdout);
                }
                break;
                
            case MSG_USER_JOINED:
                if (!in_chat) {
                    printf("\n[SYSTEM] %s\n", msg.content);
                    printf(">> ");
                    fflush(stdout);
                }
                break;
                
            case MSG_USER_LEFT:
                if (!in_chat) {
                    printf("\n[SYSTEM] %s\n", msg.content);
                    printf(">> ");
                    fflush(stdout);
                }
                break;
                
            case MSG_CHAT_REQUEST:
                // Server forwarded connection info for target: content = ip, data = port
                if (!in_chat) {
                    char target_ip[INET_ADDRSTRLEN];
                    int target_port = msg.data;
                    strncpy(target_ip, msg.content, INET_ADDRSTRLEN);
                    target_ip[INET_ADDRSTRLEN-1] = '\0';

                    printf("\n[SERVER] Peer info: %s:%d\n", target_ip, target_port);
                    printf(">> Connecting to peer...\n");

                    // Connect to peer
                    int fd = socket(AF_INET, SOCK_STREAM, 0);
                    if (fd < 0) {
                        perror("Peer socket creation failed");
                        break;
                    }
                    struct sockaddr_in peer_addr;
                    peer_addr.sin_family = AF_INET;
                    peer_addr.sin_port = htons(target_port);
                    if (inet_pton(AF_INET, target_ip, &peer_addr.sin_addr) <= 0) {
                        perror("Invalid peer IP");
                        close(fd);
                        break;
                    }

                    if (connect(fd, (struct sockaddr *)&peer_addr, sizeof(peer_addr)) < 0) {
                        perror("Connection to peer failed");
                        close(fd);
                        break;
                    }

                    // Send initial chat request over P2P socket so peer listener recognizes it
                    Message init;
                    init.type = MSG_CHAT_REQUEST;
                    strcpy(init.sender, my_username);
                    strcpy(init.receiver, "");
                    strcpy(init.content, my_username);
                    init.data = my_port;
                    send_msg(fd, &init);

                    // Set peer_socket and start chat handler (chat_partner was set when command issued)
                    pthread_mutex_lock(&chat_mutex);
                    peer_socket = fd;
                    // chat_partner remains as the target username set earlier
                    in_chat = 1;
                    pthread_mutex_unlock(&chat_mutex);

                    printf("Chat started with %s. Type 'exit' to end chat.\n", chat_partner);
                    printf(">> ");

                    pthread_t chat_thread;
                    int *peer_fd = malloc(sizeof(int));
                    *peer_fd = fd;
                    pthread_create(&chat_thread, NULL, p2p_chat_handler, peer_fd);
                    pthread_detach(chat_thread);
                }
                break;
                
            case MSG_ERROR:
                printf("\n[ERROR] %s\n", msg.content);
                printf(">> ");
                fflush(stdout);
                break;
                
            default:
                break;
        }
    }
    
    return NULL;
}

// Signal handler
void signal_handler(int sig) {
    printf("\nExiting...\n");
    running = 0;
    
    if (server_socket >= 0) {
        Message msg;
        msg.type = MSG_DISCONNECT;
        strcpy(msg.sender, my_username);
        strcpy(msg.receiver, "SERVER");
        strcpy(msg.content, "");
        msg.data = 0;
        send_msg(server_socket, &msg);
        close(server_socket);
    }
    
    exit(0);
}

// Start a chat with another user
void start_chat(const char *target_user, int my_port) {
    Message msg;
    msg.type = MSG_CHAT_REQUEST;
    strcpy(msg.sender, my_username);
    strcpy(msg.receiver, target_user);
    sprintf(msg.content, "localhost"); // In real scenario, use actual IP
    msg.data = my_port;
    
    send_msg(server_socket, &msg);
    printf("Chat request sent to %s. Waiting for response...\n", target_user);
}

int main(int argc, char *argv[]) {
    struct sockaddr_in server_addr;
    char server_ip[INET_ADDRSTRLEN] = "127.0.0.1";
    
    // Parse command line arguments
    if (argc > 1) {
        strcpy(server_ip, argv[1]);
    }
    if (argc > 2) {
        my_port = atoi(argv[2]);
    }
    
    // Setup signal handler
    signal(SIGINT, signal_handler);
    
    // Create socket
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    
    if (inet_pton(AF_INET, server_ip, &server_addr.sin_addr) <= 0) {
        perror("Invalid address");
        exit(EXIT_FAILURE);
    }
    
    // Connect to server
    if (connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to server failed");
        exit(EXIT_FAILURE);
    }
    
    printf("===========================================\n");
    printf("Connected to chat server\n");
    printf("===========================================\n");
    
    // Get username
    printf("Enter your username: ");
    fgets(my_username, MAX_USERNAME_LEN, stdin);
    my_username[strcspn(my_username, "\n")] = 0;
    
    // Register with server
    Message reg_msg;
    reg_msg.type = MSG_REGISTER;
    strcpy(reg_msg.sender, my_username);
    strcpy(reg_msg.receiver, "SERVER");
    strcpy(reg_msg.content, "");
    reg_msg.data = my_port;
    printf("[DEBUG CLIENT] Sending registration with my_port=%d\n", my_port);
    send_msg(server_socket, &reg_msg);
    
    // Start peer listener thread
    pthread_t peer_thread;
    int *port = malloc(sizeof(int));
    *port = my_port;
    pthread_create(&peer_thread, NULL, peer_listener, port);
    pthread_detach(peer_thread);
    
    // Start server handler thread
    pthread_t server_thread;
    pthread_create(&server_thread, NULL, server_handler, NULL);
    pthread_detach(server_thread);
    
    sleep(1); // Wait for registration
    
    printf("\nCommands:\n");
    printf("  list           - Show active users\n");
    printf("  chat <user>    - Start chat with user\n");
    printf("  accept         - Accept incoming chat request\n");
    printf("  reject         - Reject incoming chat request\n");
    printf("  exit           - Exit chat (when in chat) or quit application\n");
    printf("===========================================\n\n");
    
    // Main command loop
    char input[MAX_MESSAGE_LEN];
    while (running) {
        printf(">> ");
        fflush(stdout);
        
        if (fgets(input, MAX_MESSAGE_LEN, stdin) == NULL) {
            break;
        }
        
        input[strcspn(input, "\n")] = 0;
        
        if (strlen(input) == 0) {
            continue;
        }
        
        pthread_mutex_lock(&chat_mutex);
        int currently_in_chat = in_chat;
        int has_pending = pending_request;
        pthread_mutex_unlock(&chat_mutex);
        
        if (currently_in_chat) {
            // In chat mode
            if (strcmp(input, "exit") == 0) {
                Message msg;
                msg.type = MSG_DISCONNECT;
                strcpy(msg.sender, my_username);
                strcpy(msg.receiver, chat_partner);
                strcpy(msg.content, "");
                msg.data = 0;
                send_msg(peer_socket, &msg);
                
                close(peer_socket);
                pthread_mutex_lock(&chat_mutex);
                in_chat = 0;
                pthread_mutex_unlock(&chat_mutex);
                printf("Chat ended.\n");
            } else {
                // Send message to peer
                Message msg;
                msg.type = MSG_CHAT_MESSAGE;
                strcpy(msg.sender, my_username);
                strcpy(msg.receiver, chat_partner);
                strcpy(msg.content, input);
                msg.data = 0;
                send_msg(peer_socket, &msg);
            }
        } else {
            // Command mode
            if (has_pending) {
                // Handle pending chat request
                if (strcmp(input, "accept") == 0) {
                    Message reply;
                    reply.type = MSG_CHAT_ACCEPT;
                    strcpy(reply.sender, my_username);
                    strcpy(reply.receiver, chat_partner);
                    strcpy(reply.content, "Chat accepted");
                    send_msg(peer_socket, &reply);
                    
                    pthread_mutex_lock(&chat_mutex);
                    in_chat = 1;
                    pending_request = 0;
                    pthread_mutex_unlock(&chat_mutex);
                    
                    printf("Chat started with %s. Type 'exit' to end chat.\n", chat_partner);
                    printf("===========================================\n");
                    
                    // Start chat handler
                    pthread_t chat_thread;
                    int *peer_fd = malloc(sizeof(int));
                    *peer_fd = peer_socket;
                    pthread_create(&chat_thread, NULL, p2p_chat_handler, peer_fd);
                    pthread_detach(chat_thread);
                } else if (strcmp(input, "reject") == 0) {
                    Message reply;
                    reply.type = MSG_CHAT_REJECT;
                    strcpy(reply.sender, my_username);
                    strcpy(reply.receiver, chat_partner);
                    strcpy(reply.content, "Chat rejected");
                    send_msg(peer_socket, &reply);
                    close(peer_socket);
                    
                    pthread_mutex_lock(&chat_mutex);
                    pending_request = 0;
                    pthread_mutex_unlock(&chat_mutex);
                    
                    printf("Chat request rejected.\n");
                } else {
                    printf("You have a pending chat request. Type 'accept' or 'reject'.\n");
                }
            } else if (strcmp(input, "list") == 0) {
                // Send a request to server for the current user list
                Message req;
                req.type = MSG_USER_LIST;
                strcpy(req.sender, my_username);
                strcpy(req.receiver, "SERVER");
                strcpy(req.content, "");
                req.data = 0;
                send_msg(server_socket, &req);
                printf("Requesting user list...\n");
            } else if (strncmp(input, "chat ", 5) == 0) {
                char *target = input + 5;
                if (strlen(target) > 0) {
                    // Ask server for target's connection info (server will reply with IP and port)
                    strcpy(chat_partner, target);
                    Message req;
                    req.type = MSG_CHAT_REQUEST;
                    strcpy(req.sender, my_username);
                    strcpy(req.receiver, target);
                    strcpy(req.content, "");
                    req.data = my_port; // our listening port
                    send_msg(server_socket, &req);
                    printf("Request sent via server. Waiting for %s to accept...\n", target);
                } else {
                    printf("Usage: chat <username>\n");
                }
            } else if (strcmp(input, "exit") == 0 || strcmp(input, "quit") == 0) {
                running = 0;
                signal_handler(SIGINT);
            } else {
                printf("Unknown command. Available commands: list, chat <user>, exit\n");
            }
        }
    }
    
    close(server_socket);
    return 0;
}
