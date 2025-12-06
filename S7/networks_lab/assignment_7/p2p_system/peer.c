#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>

#define SERVER_PORT 8080
#define SERVER_IP "127.0.0.1"
#define BUFFER_SIZE 4096
#define MAX_FILES 100
#define MAX_FILENAME 256
#define SHARED_DIR "./shared/"
#define DOWNLOAD_DIR "./downloads/"

int my_port = 3300;  // Default port for peer server

// Forward declaration
void download_file_from_peer(char *filename, int peer_port);

// Handle incoming file requests from other peers
void *handle_peer_request(void *arg) {
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
    
    if (packet_type == 2) {
        // Packet Type 2: File Request
        // Format: "2|FILENAME"
        char filename[MAX_FILENAME];
        strcpy(filename, buffer + 2);
        
        // Construct full path
        char filepath[512];
        snprintf(filepath, sizeof(filepath), "%s%s", SHARED_DIR, filename);
        
        // Open and send file
        FILE *file = fopen(filepath, "rb");
        if (file == NULL) {
            char error_msg[] = "ERROR|File not found";
            send(client_socket, error_msg, strlen(error_msg), 0);
            printf("File not found: %s\n", filename);
        } else {
            printf("Sending file: %s\n", filename);
            
            // Send file size first
            fseek(file, 0, SEEK_END);
            long file_size = ftell(file);
            fseek(file, 0, SEEK_SET);
            
            char size_msg[64];
            sprintf(size_msg, "SIZE|%ld", file_size);
            send(client_socket, size_msg, strlen(size_msg), 0);
            
            // Wait for acknowledgment
            recv(client_socket, buffer, BUFFER_SIZE, 0);
            
            // Send file content
            size_t bytes;
            while ((bytes = fread(buffer, 1, BUFFER_SIZE, file)) > 0) {
                send(client_socket, buffer, bytes, 0);
            }
            
            fclose(file);
            printf("File sent successfully: %s\n", filename);
        }
    }
    
    close(client_socket);
    return NULL;
}

// Peer server thread
void *peer_server(void *arg) {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    
    // Create TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Peer server socket creation failed");
        return NULL;
    }
    
    // Set socket options
    int opt = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // Bind socket
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(my_port);
    
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Peer server bind failed");
        close(server_socket);
        return NULL;
    }
    
    // Listen for connections
    if (listen(server_socket, 5) < 0) {
        perror("Peer server listen failed");
        close(server_socket);
        return NULL;
    }
    
    printf("Peer server listening on port %d\n", my_port);
    
    // Accept and handle connections
    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_len);
        if (client_socket < 0) {
            perror("Accept failed");
            continue;
        }
        
        // Create thread to handle request
        pthread_t thread_id;
        int *client_sock_ptr = malloc(sizeof(int));
        *client_sock_ptr = client_socket;
        
        if (pthread_create(&thread_id, NULL, handle_peer_request, client_sock_ptr) != 0) {
            perror("Thread creation failed");
            free(client_sock_ptr);
            close(client_socket);
        } else {
            pthread_detach(thread_id);
        }
    }
    
    close(server_socket);
    return NULL;
}

// Get list of files in shared directory
int get_shared_files(char files[][MAX_FILENAME]) {
    DIR *dir;
    struct dirent *entry;
    struct stat file_stat;
    int count = 0;
    
    dir = opendir(SHARED_DIR);
    if (dir == NULL) {
        // Create directory if it doesn't exist
        mkdir(SHARED_DIR, 0755);
        return 0;
    }
    
    while ((entry = readdir(dir)) != NULL && count < MAX_FILES) {
        char filepath[512];
        snprintf(filepath, sizeof(filepath), "%s%s", SHARED_DIR, entry->d_name);
        
        if (stat(filepath, &file_stat) == 0 && S_ISREG(file_stat.st_mode)) {
            strcpy(files[count], entry->d_name);
            count++;
        }
    }
    
    closedir(dir);
    return count;
}

// Send Packet Type 1 to directory server (Login)
void login_to_server() {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    
    // Get shared files
    char files[MAX_FILES][MAX_FILENAME];
    int file_count = get_shared_files(files);
    
    if (file_count == 0) {
        printf("No files found in %s directory\n", SHARED_DIR);
        printf("Add some files to share first!\n");
        return;
    }
    
    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return;
    }
    
    // Connect to directory server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to directory server failed");
        close(sock);
        return;
    }
    
    // Build Packet Type 1
    // Format: "1|PORT_NO|FILE1|FILE2|..."
    sprintf(buffer, "1|%d", my_port);
    for (int i = 0; i < file_count; i++) {
        strcat(buffer, "|");
        strcat(buffer, files[i]);
    }
    
    // Send packet
    send(sock, buffer, strlen(buffer), 0);
    
    // Receive response
    int bytes = recv(sock, buffer, BUFFER_SIZE, 0);
    buffer[bytes] = '\0';
    
    if (strcmp(buffer, "OK") == 0) {
        printf("Successfully logged in! Shared %d files.\n", file_count);
        for (int i = 0; i < file_count; i++) {
            printf("  - %s\n", files[i]);
        }
    }
    
    close(sock);
}

// Search for a file
void search_file(char *filename) {
    int sock;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];
    
    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return;
    }
    
    // Connect to directory server
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);
    
    if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Connection to directory server failed");
        close(sock);
        return;
    }
    
    // Build Packet Type 2
    // Format: "2|FILENAME"
    sprintf(buffer, "2|%s", filename);
    
    // Send packet
    send(sock, buffer, strlen(buffer), 0);
    
    // Receive Packet Type 3
    int bytes = recv(sock, buffer, BUFFER_SIZE, 0);
    buffer[bytes] = '\0';
    
    close(sock);
    
    // Parse response
    if (buffer[0] != '3') {
        printf("Invalid response from server\n");
        return;
    }
    
    // Extract peer ports
    int ports[MAX_FILES];
    int port_count = 0;
    
    char *token = strtok(buffer + 2, "|");
    while (token != NULL && port_count < MAX_FILES) {
        ports[port_count] = atoi(token);
        port_count++;
        token = strtok(NULL, "|");
    }
    
    if (port_count == 0) {
        printf("File '%s' not found on any peer\n", filename);
        return;
    }
    
    printf("File '%s' found on %d peer(s):\n", filename, port_count);
    for (int i = 0; i < port_count; i++) {
        printf("  %d. Peer on port %d\n", i + 1, ports[i]);
    }
    
    // Ask user which peer to download from
    int choice;
    printf("Enter peer number to download from (0 to cancel): ");
    scanf("%d", &choice);
    getchar(); // consume newline
    
    if (choice < 1 || choice > port_count) {
        printf("Download cancelled\n");
        return;
    }
    
    int selected_port = ports[choice - 1];
    
    // Download file from selected peer
    download_file_from_peer(filename, selected_port);
}

// Download file from another peer
void download_file_from_peer(char *filename, int peer_port) {
    int sock;
    struct sockaddr_in peer_addr;
    char buffer[BUFFER_SIZE];
    
    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        perror("Socket creation failed");
        return;
    }
    
    // Connect to peer (localhost)
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port = htons(peer_port);
    inet_pton(AF_INET, SERVER_IP, &peer_addr.sin_addr);
    
    if (connect(sock, (struct sockaddr *)&peer_addr, sizeof(peer_addr)) < 0) {
        perror("Connection to peer failed");
        close(sock);
        return;
    }
    
    // Send file request (Packet Type 2)
    sprintf(buffer, "2|%s", filename);
    send(sock, buffer, strlen(buffer), 0);
    
    // Receive file size
    int bytes = recv(sock, buffer, BUFFER_SIZE, 0);
    buffer[bytes] = '\0';
    
    if (strncmp(buffer, "ERROR", 5) == 0) {
        printf("Error: %s\n", buffer);
        close(sock);
        return;
    }
    
    long file_size = 0;
    if (strncmp(buffer, "SIZE|", 5) == 0) {
        file_size = atol(buffer + 5);
        printf("Downloading file (size: %ld bytes)...\n", file_size);
    }
    
    // Send acknowledgment
    send(sock, "ACK", 3, 0);
    
    // Create downloads directory if it doesn't exist
    mkdir(DOWNLOAD_DIR, 0755);
    
    // Open file for writing
    char filepath[512];
    snprintf(filepath, sizeof(filepath), "%s%s", DOWNLOAD_DIR, filename);
    FILE *file = fopen(filepath, "wb");
    if (file == NULL) {
        perror("Failed to create file");
        close(sock);
        return;
    }
    
    // Receive and write file content
    long received = 0;
    while ((bytes = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        fwrite(buffer, 1, bytes, file);
        received += bytes;
    }
    
    fclose(file);
    close(sock);
    
    printf("File downloaded successfully to %s\n", filepath);
    printf("Total bytes received: %ld\n", received);
    
    // Copy downloaded file to shared folder so it can be shared with others
    char shared_path[512];
    snprintf(shared_path, sizeof(shared_path), "%s%s", SHARED_DIR, filename);
    
    // Create shared directory if it doesn't exist
    mkdir(SHARED_DIR, 0755);
    
    // Copy file to shared folder
    FILE *src = fopen(filepath, "rb");
    FILE *dst = fopen(shared_path, "wb");
    
    if (src && dst) {
        char copy_buffer[BUFFER_SIZE];
        size_t bytes_read;
        while ((bytes_read = fread(copy_buffer, 1, BUFFER_SIZE, src)) > 0) {
            fwrite(copy_buffer, 1, bytes_read, dst);
        }
        printf("File also copied to %s for sharing\n", shared_path);
    }
    
    if (src) fclose(src);
    if (dst) fclose(dst);
}

int main(int argc, char *argv[]) {
    // Parse command line arguments
    if (argc >= 2) {
        my_port = atoi(argv[1]);
    }
    
    printf("=== P2P File Transfer - Peer Application ===\n");
    printf("My Port: %d\n", my_port);
    printf("Server: localhost:%d\n", SERVER_PORT);
    
    // Start peer server in separate thread
    pthread_t server_thread;
    if (pthread_create(&server_thread, NULL, peer_server, NULL) != 0) {
        perror("Failed to create peer server thread");
        exit(EXIT_FAILURE);
    }
    pthread_detach(server_thread);
    
    sleep(1);  // Give server thread time to start
    
    // Main menu loop
    int choice;
    char filename[MAX_FILENAME];
    
    while (1) {
        printf("\n=== MENU ===\n");
        printf("1. Login (Go Online)\n");
        printf("2. Search for a File\n");
        printf("3. Exit\n");
        printf("Enter choice: ");
        
        scanf("%d", &choice);
        getchar();  // consume newline
        
        switch (choice) {
            case 1:
                login_to_server();
                break;
            case 2:
                printf("Enter filename to search: ");
                fgets(filename, MAX_FILENAME, stdin);
                filename[strcspn(filename, "\n")] = 0;  // remove newline
                search_file(filename);
                break;
            case 3:
                printf("Exiting...\n");
                exit(0);
            default:
                printf("Invalid choice!\n");
        }
    }
    
    return 0;
}
