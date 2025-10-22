#include "common.h"
#include <signal.h>

// Global variables
User clients[MAX_CLIENTS];
int client_count = 0;
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// Function to serialize message into buffer
void serialize_message(Message *msg, char *buffer) {
    sprintf(buffer, "%d|%s|%s|%s|%d", msg->type, msg->sender, msg->receiver, msg->content, msg->data);
}

// Function to deserialize buffer into message
void deserialize_message(char *buffer, Message *msg) {
    printf("[DEBUG] Deserializing: %s\n", buffer);
    
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
    
    printf("[DEBUG] type=%d, sender=%s, receiver=%s, content=%s, data=%d\n",
           msg->type, msg->sender, msg->receiver, msg->content, msg->data);
}

// Function to find user by username
int find_user_by_username(const char *username) {
    for (int i = 0; i < client_count; i++) {
        if (clients[i].is_active && strcmp(clients[i].username, username) == 0) {
            return i;
        }
    }
    return -1;
}

// Function to send message to a specific client
void send_message_to_client(int client_index, Message *msg) {
    char buffer[BUFFER_SIZE];
    serialize_message(msg, buffer);
    send(clients[client_index].socket_fd, buffer, strlen(buffer), 0);
}

// Function to broadcast user list to all clients
void broadcast_user_list() {
    Message msg;
    msg.type = MSG_USER_LIST;
    strcpy(msg.sender, "SERVER");
    strcpy(msg.receiver, "ALL");
    
    // Build user list
    char user_list[MAX_MESSAGE_LEN] = "";
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].is_active) {
            strcat(user_list, clients[i].username);
            strcat(user_list, ",");
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    
    // Remove trailing comma
    if (strlen(user_list) > 0) {
        user_list[strlen(user_list) - 1] = '\0';
    }
    
    strcpy(msg.content, user_list);
    msg.data = 0;
    
    // Send to all active clients
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].is_active) {
            send_message_to_client(i, &msg);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Function to notify all clients when a user joins
void notify_user_joined(const char *username) {
    Message msg;
    msg.type = MSG_USER_JOINED;
    strcpy(msg.sender, "SERVER");
    strcpy(msg.receiver, "ALL");
    sprintf(msg.content, "%s has joined the chat", username);
    msg.data = 0;
    
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].is_active && strcmp(clients[i].username, username) != 0) {
            send_message_to_client(i, &msg);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Function to notify all clients when a user leaves
void notify_user_left(const char *username) {
    Message msg;
    msg.type = MSG_USER_LEFT;
    strcpy(msg.sender, "SERVER");
    strcpy(msg.receiver, "ALL");
    sprintf(msg.content, "%s has left the chat", username);
    msg.data = 0;
    
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].is_active) {
            send_message_to_client(i, &msg);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Handle client communication
void *handle_client(void *arg) {
    int client_index = *((int *)arg);
    free(arg);
    
    char buffer[BUFFER_SIZE];
    Message msg;
    int bytes_received;
    
    printf("Client handler started for socket %d\n", clients[client_index].socket_fd);
    
    while (1) {
        memset(buffer, 0, BUFFER_SIZE);
        bytes_received = recv(clients[client_index].socket_fd, buffer, BUFFER_SIZE - 1, 0);
        
        if (bytes_received <= 0) {
            // Client disconnected
            printf("Client %s disconnected\n", clients[client_index].username);
            break;
        }
        
        buffer[bytes_received] = '\0';
        deserialize_message(buffer, &msg);
        
        switch (msg.type) {
            case MSG_USER_LIST: {
                // Client requested the current user list; send only to requester
                Message reply;
                reply.type = MSG_USER_LIST;
                strcpy(reply.sender, "SERVER");
                strcpy(reply.receiver, msg.sender);

                // Build user list
                char user_list[MAX_MESSAGE_LEN] = "";
                pthread_mutex_lock(&clients_mutex);
                for (int i = 0; i < client_count; i++) {
                    if (clients[i].is_active) {
                        strcat(user_list, clients[i].username);
                        strcat(user_list, ",");
                    }
                }
                pthread_mutex_unlock(&clients_mutex);

                if (strlen(user_list) > 0) {
                    user_list[strlen(user_list) - 1] = '\0';
                }
                strcpy(reply.content, user_list);
                reply.data = 0;

                send_message_to_client(client_index, &reply);
                break;
            }
            case MSG_REGISTER: {
                // Check for duplicate username
                pthread_mutex_lock(&clients_mutex);
                int duplicate = 0;
                for (int i = 0; i < client_count; i++) {
                    if (i != client_index && clients[i].is_active && 
                        strcmp(clients[i].username, msg.sender) == 0) {
                        duplicate = 1;
                        break;
                    }
                }
                
                if (duplicate) {
                    pthread_mutex_unlock(&clients_mutex);
                    
                    // Send error
                    Message error;
                    error.type = MSG_ERROR;
                    strcpy(error.sender, "SERVER");
                    strcpy(error.receiver, msg.sender);
                    strcpy(error.content, "Username already taken. Please disconnect and try a different username.");
                    error.data = 0;
                    send_message_to_client(client_index, &error);
                    
                    printf("Registration rejected: %s (username already taken)\n", msg.sender);
                    
                    // Close connection
                    close(clients[client_index].socket_fd);
                    goto cleanup;
                }
                
                // Register new user
                strcpy(clients[client_index].username, msg.sender);
                // store peer listening port provided by client in msg.data
                clients[client_index].port = msg.data;
                clients[client_index].is_active = 1;
                pthread_mutex_unlock(&clients_mutex);
                
                printf("User registered: %s (peer port: %d)\n", msg.sender, msg.data);
                
                // Send confirmation
                Message confirm;
                confirm.type = MSG_REGISTER;
                strcpy(confirm.sender, "SERVER");
                strcpy(confirm.receiver, msg.sender);
                strcpy(confirm.content, "Registration successful");
                confirm.data = 0;
                send_message_to_client(client_index, &confirm);
                
                // Notify others and send user list
                notify_user_joined(msg.sender);
                broadcast_user_list();
                break;
            }
            
            case MSG_CHAT_REQUEST: {
                // Check if user is trying to chat with themselves
                if (strcmp(msg.sender, msg.receiver) == 0) {
                    Message error;
                    error.type = MSG_ERROR;
                    strcpy(error.sender, "SERVER");
                    strcpy(error.receiver, msg.sender);
                    strcpy(error.content, "Cannot chat with yourself");
                    error.data = 0;
                    send_message_to_client(client_index, &error);
                    printf("Chat request rejected: %s tried to chat with themselves\n", msg.sender);
                    break;
                }
                
                // Provide target's IP and peer port to initiator so it can connect directly
                int target_index = find_user_by_username(msg.receiver);
                if (target_index >= 0) {
                    printf("Chat request from %s to %s (target peer port: %d)\n", 
                           msg.sender, msg.receiver, clients[target_index].port);

                    // Send a reply to initiator (client_index) with target's ip and port
                    Message reply;
                    reply.type = MSG_CHAT_REQUEST;
                    strcpy(reply.sender, "SERVER");
                    strcpy(reply.receiver, msg.sender);
                    // include target ip in content and target peer port in data
                    strcpy(reply.content, clients[target_index].ip_address);
                    reply.data = clients[target_index].port;
                    send_message_to_client(client_index, &reply);
                } else {
                    // User not found
                    Message error;
                    error.type = MSG_ERROR;
                    strcpy(error.sender, "SERVER");
                    strcpy(error.receiver, msg.sender);
                    strcpy(error.content, "User not found or offline");
                    error.data = 0;
                    send_message_to_client(client_index, &error);
                }
                break;
            }
            
            case MSG_CHAT_ACCEPT:
            case MSG_CHAT_REJECT: {
                // Forward response to requester
                int target_index = find_user_by_username(msg.receiver);
                if (target_index >= 0) {
                    send_message_to_client(target_index, &msg);
                }
                break;
            }
            
            case MSG_DISCONNECT: {
                printf("User %s is disconnecting\n", clients[client_index].username);
                goto cleanup;
            }
            
            default:
                printf("Unknown message type: %d\n", msg.type);
                break;
        }
    }
    
cleanup:
    // Cleanup
    pthread_mutex_lock(&clients_mutex);
    notify_user_left(clients[client_index].username);
    clients[client_index].is_active = 0;
    close(clients[client_index].socket_fd);
    pthread_mutex_unlock(&clients_mutex);
    
    broadcast_user_list();
    
    printf("Client handler terminated for %s\n", clients[client_index].username);
    return NULL;
}

// Signal handler for graceful shutdown
void signal_handler(int sig) {
    printf("\nShutting down server...\n");
    
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < client_count; i++) {
        if (clients[i].is_active) {
            close(clients[i].socket_fd);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    
    exit(0);
}

int main() {
    int server_fd, new_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t addr_len = sizeof(client_addr);
    
    // Initialize clients array
    memset(clients, 0, sizeof(clients));
    for (int i = 0; i < MAX_CLIENTS; i++) {
        pthread_mutex_init(&clients[i].mutex, NULL);
    }
    
    // Setup signal handler
    signal(SIGINT, signal_handler);
    
    // Create socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Set socket options
    int opt = 1;
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    
    // Configure server address
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(SERVER_PORT);
    
    // Bind socket
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        exit(EXIT_FAILURE);
    }
    
    // Listen for connections
    if (listen(server_fd, MAX_CLIENTS) < 0) {
        perror("Listen failed");
        exit(EXIT_FAILURE);
    }
    
    printf("===========================================\n");
    printf("Chat Server started on port %d\n", SERVER_PORT);
    printf("Waiting for client connections...\n");
    printf("===========================================\n\n");
    
    // Accept client connections
    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
        
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }
        
        pthread_mutex_lock(&clients_mutex);
        
        if (client_count >= MAX_CLIENTS) {
            printf("Maximum clients reached. Rejecting connection.\n");
            close(new_socket);
            pthread_mutex_unlock(&clients_mutex);
            continue;
        }
        
        // Add new client
        clients[client_count].socket_fd = new_socket;
        inet_ntop(AF_INET, &client_addr.sin_addr, clients[client_count].ip_address, INET_ADDRSTRLEN);
        clients[client_count].port = ntohs(client_addr.sin_port);
        clients[client_count].is_active = 0; // Will be set to 1 after registration
        clients[client_count].in_chat = 0;
        
        printf("New connection from %s:%d (socket: %d)\n", 
               clients[client_count].ip_address, 
               clients[client_count].port,
               new_socket);
        
        // Create thread for client
        pthread_t thread_id;
        int *client_idx = malloc(sizeof(int));
        *client_idx = client_count;
        
        if (pthread_create(&thread_id, NULL, handle_client, (void *)client_idx) != 0) {
            perror("Thread creation failed");
            close(new_socket);
            free(client_idx);
            pthread_mutex_unlock(&clients_mutex);
            continue;
        }
        
        pthread_detach(thread_id);
        client_count++;
        
        pthread_mutex_unlock(&clients_mutex);
    }
    
    close(server_fd);
    return 0;
}
