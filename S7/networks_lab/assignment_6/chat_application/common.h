#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

// Constants
#define SERVER_PORT 8888
#define MAX_CLIENTS 100
#define MAX_USERNAME_LEN 32
#define MAX_MESSAGE_LEN 1024
#define BUFFER_SIZE 2048

// Message Types
#define MSG_REGISTER 1
#define MSG_USER_LIST 2
#define MSG_CHAT_REQUEST 3
#define MSG_CHAT_ACCEPT 4
#define MSG_CHAT_REJECT 5
#define MSG_CHAT_MESSAGE 6
#define MSG_DISCONNECT 7
#define MSG_USER_JOINED 8
#define MSG_USER_LEFT 9
#define MSG_ERROR 10

// User structure
typedef struct {
    int socket_fd;
    char username[MAX_USERNAME_LEN];
    char ip_address[INET_ADDRSTRLEN];
    int port;
    int is_active;
    int in_chat;
    pthread_mutex_t mutex;
} User;

// Message structure
typedef struct {
    int type;
    char sender[MAX_USERNAME_LEN];
    char receiver[MAX_USERNAME_LEN];
    char content[MAX_MESSAGE_LEN];
    int data; // For port numbers or other numeric data
} Message;

// Function prototypes for message handling
void serialize_message(Message *msg, char *buffer);
void deserialize_message(char *buffer, Message *msg);

#endif // COMMON_H
