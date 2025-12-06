#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 4096
#define MAX_FILES 100
#define MAX_FILENAME 256

// Structure to store peer information
typedef struct PeerNode {
    int port;
    char files[MAX_FILES][MAX_FILENAME];
    int file_count;
    struct PeerNode *next;
} PeerNode;

PeerNode *peer_list = NULL;
pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;

// Add or update peer in the list
void add_peer(int port, char files[][MAX_FILENAME], int file_count) {
    pthread_mutex_lock(&list_mutex);
    
    // Check if peer already exists
    PeerNode *current = peer_list;
    while (current != NULL) {
        if (current->port == port) {
            // Update existing peer
            current->file_count = file_count;
            for (int i = 0; i < file_count; i++) {
                strcpy(current->files[i], files[i]);
            }
            pthread_mutex_unlock(&list_mutex);
            printf("Updated peer on port %d with %d files\n", port, file_count);
            return;
        }
        current = current->next;
    }
    
    // Add new peer
    PeerNode *new_peer = (PeerNode *)malloc(sizeof(PeerNode));
    new_peer->port = port;
    new_peer->file_count = file_count;
    for (int i = 0; i < file_count; i++) {
        strcpy(new_peer->files[i], files[i]);
    }
    new_peer->next = peer_list;
    peer_list = new_peer;
    
    pthread_mutex_unlock(&list_mutex);
    printf("Peer on port %d is online with %d files\n", port, file_count);
}

// Search for peers that have a specific file
void search_file(char *filename, int *ports, int *count) {
    pthread_mutex_lock(&list_mutex);
    
    *count = 0;
    PeerNode *current = peer_list;
    
    while (current != NULL) {
        for (int i = 0; i < current->file_count; i++) {
            if (strcmp(current->files[i], filename) == 0) {
                ports[*count] = current->port;
                (*count)++;
                break;
            }
        }
        current = current->next;
    }
    
    pthread_mutex_unlock(&list_mutex);
}

// Handle client connection
void *handle_client(void *arg) {
    int client_socket = *(int *)arg;
    free(arg);
    char buffer[BUFFER_SIZE];
    
    int bytes_read = recv(client_socket, buffer, BUFFER_SIZE, 0);
    if (bytes_read <= 0) {
        close(client_socket);
        return NULL;
    }
    
    buffer[bytes_read] = '\0';
    
    // Parse packet type
    int packet_type = buffer[0] - '0';
    
    if (packet_type == 1) {
        // Packet Type 1: Peer Online
        // Format: "1|PORT_NO|FILE1|FILE2|..."
        int port;
        char files[MAX_FILES][MAX_FILENAME];
        int file_count = 0;
        
        char *token = strtok(buffer + 2, "|");
        port = atoi(token);
        
        token = strtok(NULL, "|");
        while (token != NULL && file_count < MAX_FILES) {
            strcpy(files[file_count], token);
            file_count++;
            token = strtok(NULL, "|");
        }
        
        add_peer(port, files, file_count);
        
        char response[] = "OK";
        send(client_socket, response, strlen(response), 0);
        
    } else if (packet_type == 2) {
        // Packet Type 2: File Query
        // Format: "2|FILENAME"
        char filename[MAX_FILENAME];
        strcpy(filename, buffer + 2);
        
        int ports[MAX_FILES];
        int count;
        search_file(filename, ports, &count);
        
        // Send Packet Type 3: Response
        // Format: "3|PORT1|PORT2|..."
        char response[BUFFER_SIZE];
        sprintf(response, "3");
        for (int i = 0; i < count; i++) {
            sprintf(response + strlen(response), "|%d", ports[i]);
        }
        
        send(client_socket, response, strlen(response), 0);
        printf("Sent %d peer(s) with file '%s'\n", count, filename);
    }
    
    close(client_socket);
    return NULL;
}

int main() {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    // Create TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    
    // Set socket options
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // Bind socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);
    
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    
    // Listen for connections
    if (listen(server_socket, 5) < 0) {
        perror("Listen failed");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    
    printf("Directory Server listening on port %d\n", PORT);
    
    // Accept and handle connections
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }
        
        // Create thread to handle client
        pthread_t thread_id;
        int *client_sock_ptr = malloc(sizeof(int));
        *client_sock_ptr = client_socket;
        
        if (pthread_create(&thread_id, NULL, handle_client, client_sock_ptr) != 0) {
            perror("Thread creation failed");
            free(client_sock_ptr);
            close(client_socket);
        } else {
            pthread_detach(thread_id);
        }
    }
    
    close(server_socket);
    return 0;
}
