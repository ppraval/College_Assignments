# Chat Application - Complete Viva Preparation Guide

**Author:** Network Programming Lab Assignment  
**Date:** October 20, 2025  
**Topic:** Concurrent Peer-to-Peer Chat Application with Directory Server

---

## Table of Contents
1. [Project Overview](#project-overview)
2. [Architecture & Design](#architecture--design)
3. [Key Concepts](#key-concepts)
4. [Implementation Details](#implementation-details)
5. [Protocol Design](#protocol-design)
6. [Threading Model](#threading-model)
7. [Bug Fixes & Debugging](#bug-fixes--debugging)
8. [Viva Questions & Answers](#viva-questions--answers)
9. [Code Walkthrough](#code-walkthrough)
10. [Testing & Demonstration](#testing--demonstration)

---

## 1. Project Overview

### What is this application?
A **fully concurrent chat application** implemented in C that allows multiple users to chat with each other through direct peer-to-peer (P2P) connections while using a central directory server for user discovery and registration.

### Key Features
- ✅ **Concurrent Server**: Handles multiple clients simultaneously using POSIX threads
- ✅ **Directory Service**: Central server maintains list of active users
- ✅ **P2P Communication**: Direct connections between clients for chatting
- ✅ **Thread-Safe**: Uses mutex locks to prevent race conditions
- ✅ **Real-time Notifications**: Users notified when others join/leave
- ✅ **Chat Request System**: Accept/reject incoming chat requests

### Why P2P instead of Server-mediated chat?
- **Scalability**: Server only handles registration, not every message
- **Efficiency**: Direct communication is faster (no intermediary)
- **Learning**: Demonstrates both client-server and P2P paradigms
- **Real-world**: Similar to how apps like Skype/WhatsApp work (signaling vs data)

---

## 2. Architecture & Design

### High-Level Architecture

```
┌─────────────────────────────────────────────────────────────┐
│                   DIRECTORY SERVER (Port 8888)               │
│  - Accepts client connections (TCP)                         │
│  - Maintains active user list                               │
│  - Stores: username, IP address, peer listening port        │
│  - Provides peer discovery (returns IP:port for P2P)        │
│  - Thread per client for concurrent handling                │
└─────────────────────────────────────────────────────────────┘
                            ▲  ▲  ▲
                            │  │  │
              Registration  │  │  │  User List Requests
              Chat Requests │  │  │  
                            │  │  │
                    ┌───────┘  │  └────────┐
                    │          │           │
                    ▼          ▼           ▼
           ┌────────────┬────────────┬────────────┐
           │  Client A  │  Client B  │  Client C  │
           │            │            │            │
           │ Server     │ Server     │ Server     │
           │ Socket     │ Socket     │ Socket     │
           │            │            │            │
           │ P2P Listen │ P2P Listen │ P2P Listen │
           │ Port: 9000 │ Port: 9001 │ Port: 9002 │
           └────────────┴────────────┴────────────┘
                    │                   │
                    │  Direct P2P Chat  │
                    │  (TCP Connection) │
                    └───────────────────┘
```

### Communication Flows

#### Flow 1: User Registration
```
Client                          Server
  |                               |
  |--- TCP Connect (8888) ------->|
  |                               |
  |--- MSG_REGISTER ------------->|
  |    (username, peer_port)      |
  |                               |--- Store user info
  |                               |--- Add to active list
  |<-- MSG_REGISTER (confirm) ----|
  |                               |
  |<-- MSG_USER_LIST -------------|--- Broadcast to all
```

#### Flow 2: Initiating a Chat
```
Alice (Client A)            Server              Bob (Client B)
     |                        |                       |
     |--- MSG_CHAT_REQUEST -->|                       |
     |    (target: Bob)       |                       |
     |                        |--- Lookup Bob's      |
     |                        |    IP and port       |
     |<-- IP:9001 ------------|                       |
     |                        |                       |
     |--------- Direct TCP Connect (9001) ----------->|
     |                                                 |
     |--- MSG_CHAT_REQUEST (initial handshake) ------>|
     |                                                 |
     |                               Bob sees prompt  |
     |                               User types:      |
     |                               "accept"         |
     |                                                 |
     |<-- MSG_CHAT_ACCEPT -----------------------------|
     |                                                 |
     |<==========P2P Chat Connection Active==========>|
     |                                                 |
     |--- MSG_CHAT_MESSAGE (Hello!) ------------------>|
     |<-- MSG_CHAT_MESSAGE (Hi!) ----------------------|
```

### Design Decisions

#### 1. Why Separate Server and P2P Sockets?
- **Server socket**: Persistent connection for directory services
- **P2P socket**: Temporary connection for active chat
- Keeps concerns separated and allows concurrent operations

#### 2. Why Each Client Listens on a Port?
- Enables **incoming** P2P connections
- Acts as both client (to server) and server (for peers)
- Peer-to-peer requires bidirectional connection capability

#### 3. Why Threads Instead of select()/poll()?
- **Simpler code**: Each connection has dedicated thread
- **Blocking I/O**: Can use simple recv/send without complexity
- **Learning objective**: Understand thread synchronization
- **Scalability**: Modern systems handle thousands of threads efficiently

---

## 3. Key Concepts

### 3.1 Socket Programming

#### TCP Socket Lifecycle

**Server Side:**
```c
socket()      // Create socket endpoint
  ↓
bind()        // Bind to IP:Port
  ↓
listen()      // Mark as passive socket (accepting connections)
  ↓
accept()      // Block until client connects (returns new socket)
  ↓
recv/send()   // Communicate with client
  ↓
close()       // Close connection
```

**Client Side:**
```c
socket()      // Create socket endpoint
  ↓
connect()     // Connect to server IP:Port
  ↓
send/recv()   // Communicate with server
  ↓
close()       // Close connection
```

#### Important Socket Functions

```c
// Create a socket
int socket(int domain, int type, int protocol);
// domain: AF_INET (IPv4), AF_INET6 (IPv6)
// type: SOCK_STREAM (TCP), SOCK_DGRAM (UDP)
// protocol: 0 (auto-select)

// Bind socket to address
int bind(int sockfd, struct sockaddr *addr, socklen_t addrlen);

// Listen for connections (server)
int listen(int sockfd, int backlog);
// backlog: max pending connections queue

// Accept incoming connection (server)
int accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
// Returns: new socket for this connection

// Connect to server (client)
int connect(int sockfd, struct sockaddr *addr, socklen_t addrlen);

// Send data
ssize_t send(int sockfd, const void *buf, size_t len, int flags);

// Receive data
ssize_t recv(int sockfd, void *buf, size_t len, int flags);
```

### 3.2 Multithreading with POSIX Threads (pthreads)

#### Thread Basics

**Creating a Thread:**
```c
pthread_t thread_id;
int pthread_create(pthread_t *thread, 
                   const pthread_attr_t *attr,
                   void *(*start_routine)(void*), 
                   void *arg);
```

**Example:**
```c
pthread_t thread;
void *my_function(void *arg) {
    int *data = (int *)arg;
    printf("Thread received: %d\n", *data);
    return NULL;
}

int *value = malloc(sizeof(int));
*value = 42;
pthread_create(&thread, NULL, my_function, value);
```

#### Thread Synchronization - Mutex

**Why Mutex?**
- Prevents **race conditions** when multiple threads access shared data
- Ensures **mutual exclusion** (only one thread at a time)

```c
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_lock(&lock);
// Critical section - only one thread can be here
shared_variable++;
pthread_mutex_unlock(&lock);
```

**In Our Application:**
```c
pthread_mutex_lock(&clients_mutex);
// Safely access clients[] array
strcpy(clients[i].username, username);
pthread_mutex_unlock(&clients_mutex);
```

#### Thread Detachment

```c
pthread_detach(thread_id);
```
- Thread resources automatically released when it exits
- Main thread doesn't need to `pthread_join()`
- Used for "fire and forget" threads

### 3.3 Network Byte Order

**Problem:** Different CPUs store multi-byte numbers differently
- **Big-endian**: Most significant byte first (network standard)
- **Little-endian**: Least significant byte first (x86/x64)

**Solution:** Convert to network byte order
```c
uint16_t port = 8888;
server_addr.sin_port = htons(port);  // Host TO Network Short
// htons: host to network short (16-bit)
// htonl: host to network long (32-bit)
// ntohs: network to host short
// ntohl: network to host long
```

### 3.4 IP Address Conversion

```c
// String to Binary
inet_pton(AF_INET, "127.0.0.1", &addr.sin_addr);
// pton = Presentation TO Network

// Binary to String
char ip_str[INET_ADDRSTRLEN];
inet_ntop(AF_INET, &addr.sin_addr, ip_str, INET_ADDRSTRLEN);
// ntop = Network TO Presentation
```

---

## 4. Implementation Details

### 4.1 File Structure

```
chat_application/
├── common.h              # Shared definitions, structures, constants
├── server.c              # Directory server implementation
├── client.c              # Client application
├── Makefile              # Build automation
├── README.md             # User documentation
├── TEST_INSTRUCTIONS.md  # Testing guide
└── VIVA_PREPARATION.md   # This file
```

### 4.2 Data Structures

#### Message Structure (common.h)
```c
typedef struct {
    int type;                          // Message type (1-10)
    char sender[MAX_USERNAME_LEN];     // Who sent it
    char receiver[MAX_USERNAME_LEN];   // Who should receive it
    char content[MAX_MESSAGE_LEN];     // Message text/data
    int data;                          // Additional numeric data (port, etc.)
} Message;
```

**Design rationale:**
- Fixed-size structure for predictable memory layout
- Type field for message discrimination
- Flexible content field for various data
- Data field for numeric values (avoids string-to-int conversions)

#### User Structure (server.c)
```c
typedef struct {
    int socket_fd;                     // Connection to this client
    char username[MAX_USERNAME_LEN];   // Username
    char ip_address[INET_ADDRSTRLEN]; // Client's IP (for P2P)
    int port;                          // Client's P2P listening port
    int is_active;                     // Currently connected?
    int in_chat;                       // Currently in a chat?
    pthread_mutex_t mutex;             // Per-user lock
} User;
```

### 4.3 Message Serialization

#### Why Serialization?
- Network sends **bytes**, not C structures
- Different machines may have different struct padding
- Need human-readable format for debugging

#### Our Protocol Format
```
type|sender|receiver|content|data
```

**Example:**
```
1|alice|SERVER||9000
```
Means: Type=1 (REGISTER), sender=alice, receiver=SERVER, content=(empty), data=9000

#### Serialization Function
```c
void serialize_message(Message *msg, char *buffer) {
    sprintf(buffer, "%d|%s|%s|%s|%d", 
            msg->type, msg->sender, msg->receiver, 
            msg->content, msg->data);
}
```

#### Deserialization Function (Manual Parsing)
```c
void deserialize_message(char *buffer, Message *msg) {
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
```

**Why not strtok()?**
- strtok() treats consecutive delimiters as ONE delimiter
- `1|alice|SERVER||9000` has empty content field (||)
- strtok would skip the empty field, misaligning all subsequent fields
- Manual parsing handles empty fields correctly

### 4.4 Server Implementation

#### Main Server Loop
```c
while (1) {
    // Accept new connection
    new_socket = accept(server_fd, ...);
    
    // Store client info
    pthread_mutex_lock(&clients_mutex);
    clients[client_count].socket_fd = new_socket;
    // ... store IP, initialize fields
    
    // Create handler thread
    pthread_t thread_id;
    int *client_idx = malloc(sizeof(int));
    *client_idx = client_count;
    pthread_create(&thread_id, NULL, handle_client, client_idx);
    pthread_detach(thread_id);
    
    client_count++;
    pthread_mutex_unlock(&clients_mutex);
}
```

#### Client Handler Thread
```c
void *handle_client(void *arg) {
    int client_index = *((int *)arg);
    free(arg);  // Free the malloc'd argument
    
    while (1) {
        bytes = recv(clients[client_index].socket_fd, buffer, ...);
        if (bytes <= 0) break;  // Client disconnected
        
        deserialize_message(buffer, &msg);
        
        switch (msg.type) {
            case MSG_REGISTER:
                // Store username and peer port
                strcpy(clients[client_index].username, msg.sender);
                clients[client_index].port = msg.data;
                clients[client_index].is_active = 1;
                // Send confirmation
                // Notify others
                // Broadcast user list
                break;
                
            case MSG_USER_LIST:
                // Build and send user list to requester
                break;
                
            case MSG_CHAT_REQUEST:
                // Find target user
                // Send target's IP:port to requester
                break;
        }
    }
    
    // Cleanup when client disconnects
    notify_user_left(clients[client_index].username);
    clients[client_index].is_active = 0;
    close(clients[client_index].socket_fd);
    return NULL;
}
```

### 4.5 Client Implementation

#### Client Threads

**1. Main Thread:**
- Handles user input
- Sends commands to server
- Manages chat state

**2. Server Handler Thread:**
- Receives messages from directory server
- Processes user list updates
- Handles chat request responses (IP:port from server)
- Initiates P2P connections

**3. Peer Listener Thread:**
- Listens on a port for incoming P2P connections
- Accepts chat requests from other clients
- Prompts user to accept/reject

**4. P2P Chat Handler Thread:**
- Receives messages from chat partner
- Displays incoming messages
- Handles disconnect notifications

#### Client Main Flow
```c
int main(int argc, char *argv[]) {
    // Parse command line arguments
    my_port = atoi(argv[2]);  // e.g., 9000
    
    // Connect to directory server
    connect(server_socket, server_addr, ...);
    
    // Register with server
    Message reg_msg;
    reg_msg.type = MSG_REGISTER;
    strcpy(reg_msg.sender, my_username);
    reg_msg.data = my_port;  // Send our listening port
    send_msg(server_socket, &reg_msg);
    
    // Start peer listener thread (accepts incoming chats)
    pthread_create(&peer_thread, NULL, peer_listener, &my_port);
    
    // Start server handler thread (receives from server)
    pthread_create(&server_thread, NULL, server_handler, NULL);
    
    // Main command loop
    while (running) {
        fgets(input, ...);
        
        if (in_chat) {
            // Send messages to peer
        } else {
            // Handle commands: list, chat, accept, reject, exit
        }
    }
}
```

#### Initiating a Chat
```c
// User types: chat alice
strcpy(chat_partner, "alice");

Message req;
req.type = MSG_CHAT_REQUEST;
strcpy(req.sender, my_username);
strcpy(req.receiver, "alice");
req.data = my_port;
send_msg(server_socket, &req);  // Ask server for alice's IP:port
```

#### Server Handler Receives Response
```c
case MSG_CHAT_REQUEST:
    // Server sent: content=target_ip, data=target_port
    int target_port = msg.data;
    strcpy(target_ip, msg.content);
    
    // Connect directly to peer
    int peer_fd = socket(...);
    connect(peer_fd, target_addr, ...);
    
    // Send initial handshake
    Message init;
    init.type = MSG_CHAT_REQUEST;
    strcpy(init.sender, my_username);
    send_msg(peer_fd, &init);
    
    // Start chat handler thread
    peer_socket = peer_fd;
    in_chat = 1;
    pthread_create(&chat_thread, NULL, p2p_chat_handler, &peer_fd);
```

#### Accepting a Chat (Peer Listener)
```c
void *peer_listener(void *arg) {
    // Listen on my_port
    listen(listen_fd, 5);
    
    while (running) {
        new_peer = accept(listen_fd, ...);
        
        // Receive initial message
        recv(new_peer, buffer, ...);
        deserialize_message(buffer, &msg);
        
        if (msg.type == MSG_CHAT_REQUEST) {
            strcpy(chat_partner, msg.sender);
            printf("Incoming chat request from: %s\n", msg.sender);
            
            // Set pending flag for main thread to handle
            pending_request = 1;
            peer_socket = new_peer;
        }
    }
}
```

---

## 5. Protocol Design

### Message Types

| Type | Name | Sender→Receiver | Purpose |
|------|------|-----------------|---------|
| 1 | MSG_REGISTER | Client→Server | Register username and peer port |
| 2 | MSG_USER_LIST | Server→Client or Client→Server | Request or deliver active user list |
| 3 | MSG_CHAT_REQUEST | Client→Server→Client | Initiate chat (get peer info) or P2P handshake |
| 4 | MSG_CHAT_ACCEPT | Client→Client (P2P) | Accept chat request |
| 5 | MSG_CHAT_REJECT | Client→Client (P2P) | Reject chat request |
| 6 | MSG_CHAT_MESSAGE | Client→Client (P2P) | Actual chat message |
| 7 | MSG_DISCONNECT | Client→Server or Client→Client | Graceful disconnect |
| 8 | MSG_USER_JOINED | Server→All Clients | Broadcast: new user joined |
| 9 | MSG_USER_LEFT | Server→All Clients | Broadcast: user left |
| 10 | MSG_ERROR | Server→Client | Error notification |

### Protocol Examples

#### Example 1: User Registration
```
Client sends: 1|bob|SERVER||9001
              ↑  ↑   ↑     ↑ ↑
              │  │   │     │ └─ peer listening port
              │  │   │     └─── empty content
              │  │   └───────── receiver (server)
              │  └───────────── username
              └──────────────── message type (REGISTER)

Server responds: 1|SERVER|bob|Registration successful|0
Server broadcasts: 8|SERVER|ALL|bob has joined the chat|0
Server broadcasts: 2|SERVER|ALL|alice,bob,charlie|0
```

#### Example 2: Chat Initiation
```
Alice wants to chat with Bob:

1. Alice→Server: 3|alice|bob||9000
   (MSG_CHAT_REQUEST: alice wants to chat with bob, alice's port=9000)

2. Server→Alice: 3|SERVER|alice|127.0.0.1|9001
   (MSG_CHAT_REQUEST response: bob's IP=127.0.0.1, port=9001)

3. Alice connects directly to 127.0.0.1:9001

4. Alice→Bob (P2P): 3|alice||alice|9000
   (MSG_CHAT_REQUEST handshake)

5. Bob's UI prompts: "Incoming chat request from alice"
   Bob types: accept

6. Bob→Alice (P2P): 4|bob|alice|Chat accepted|0
   (MSG_CHAT_ACCEPT)

7. Now both can exchange MSG_CHAT_MESSAGE
```

#### Example 3: Chat Messages
```
Alice→Bob: 6|alice|bob|Hello Bob!|0
Bob→Alice: 6|bob|alice|Hi Alice! How are you?|0
Alice→Bob: 6|alice|bob|I'm good, thanks!|0
```

#### Example 4: Ending Chat
```
Alice→Bob: 7|alice|bob||0
(MSG_DISCONNECT)

Bob's UI shows: "[alice has ended the chat]"
```

---

## 6. Threading Model

### Server Threading

```
Main Thread
    │
    ├─ accept() loop
    │
    ├─ Thread 1: handle_client(client_index=0)
    │   └─ Serves Client A
    │
    ├─ Thread 2: handle_client(client_index=1)
    │   └─ Serves Client B
    │
    └─ Thread 3: handle_client(client_index=2)
        └─ Serves Client C
```

**Each client handler thread:**
- Receives messages from its client
- Processes requests (registration, user list, chat requests)
- Sends responses
- Updates shared `clients[]` array (with mutex protection)

### Client Threading

```
Main Thread (User Input)
    │
    ├─ Server Handler Thread
    │   └─ Receives from directory server
    │   └─ Initiates P2P connections
    │
    ├─ Peer Listener Thread
    │   └─ Accepts incoming P2P connections
    │   └─ Prompts user for accept/reject
    │
    └─ P2P Chat Handler Thread (created when chat starts)
        └─ Receives messages from chat partner
        └─ Displays incoming messages
```

### Thread Synchronization

#### Critical Sections in Server
```c
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// Protect clients[] array
pthread_mutex_lock(&clients_mutex);
clients[i].is_active = 1;
strcpy(clients[i].username, username);
pthread_mutex_unlock(&clients_mutex);
```

**Why needed?**
- Multiple threads access `clients[]` array
- Without mutex: race condition, data corruption
- Example race: Thread A reads `client_count=5`, Thread B reads `client_count=5`, both write to `clients[5]`, one overwrites the other

#### Critical Sections in Client
```c
pthread_mutex_t chat_mutex = PTHREAD_MUTEX_INITIALIZER;

// Protect chat state
pthread_mutex_lock(&chat_mutex);
in_chat = 1;
pending_request = 0;
pthread_mutex_unlock(&chat_mutex);
```

**Why needed?**
- Multiple threads check/modify `in_chat` flag
- Main thread reads it, server handler modifies it
- Peer listener reads it to reject if already busy

### Potential Race Conditions (and how we avoid them)

**Scenario 1: Two clients connect simultaneously**
```
Thread A                    Thread B
├─ clients_mutex lock
├─ clients[5] = new_client
├─ client_count = 6
├─ clients_mutex unlock
                            ├─ clients_mutex lock
                            ├─ clients[6] = new_client
                            ├─ client_count = 7
                            ├─ clients_mutex unlock
```
✅ **Safe:** Mutex ensures serialization

**Scenario 2: User disconnects while receiving message**
```
Handler Thread              Main Thread
├─ recv() [blocked]
                            ├─ User presses Ctrl+C
                            ├─ close(socket)
├─ recv() returns 0
├─ goto cleanup
```
✅ **Safe:** recv() returns 0 when socket closed, thread cleans up properly

---

## 7. Bug Fixes & Debugging

### Bug 1: Empty Field Serialization (THE MAJOR BUG)

#### Problem
```
Client sends: 1|alice|SERVER||9000
                               ↑↑
                               Empty content field creates ||
```

When using `strtok(buffer, "|")`:
```c
token1 = "1"        → type
token2 = "alice"    → sender
token3 = "SERVER"   → receiver
token4 = "9000"     → content (WRONG! Should be empty)
token5 = NULL       → data becomes 0 (WRONG! Should be 9000)
```

**Why?** strtok treats consecutive delimiters as ONE delimiter, skipping empty fields.

#### Solution
Manual parsing that respects empty fields:
```c
void deserialize_message(char *buffer, Message *msg) {
    char *ptr = buffer;
    char *field_start = ptr;
    int field_num = 0;
    
    while (*ptr && field_num < 5) {
        if (*ptr == '|') {
            *ptr = '\0';  // Temporarily terminate
            
            // Process field (even if empty)
            switch(field_num) {
                case 0: msg->type = atoi(field_start); break;
                case 1: strcpy(msg->sender, field_start); break;
                case 2: strcpy(msg->receiver, field_start); break;
                case 3: strcpy(msg->content, field_start); break;
            }
            
            *ptr = '|';  // Restore
            field_start = ptr + 1;  // Next field
            field_num++;
        }
        ptr++;
    }
    
    // Last field
    if (field_num == 4) {
        msg->data = atoi(field_start);
    }
}
```

**Result:** Correctly parses `1|alice|SERVER||9000`
- type=1, sender=alice, receiver=SERVER, content="", data=9000 ✅

### Bug 2: stdin Conflict (scanf vs fgets)

#### Problem
```c
// Peer listener thread
scanf("%s", response);  // Blocks waiting for input

// Main thread
fgets(input, ...);      // Also reading stdin
```

**Issue:** Both threads read from stdin. When user types "yes", it might go to the wrong thread!

**Symptoms:**
```
Incoming chat request from: alice
Accept? (yes/no): yes
Unknown command. Available commands: list, chat <user>, exit
```

The "yes" was consumed by main thread's fgets, not scanf!

#### Solution
Remove stdin reading from peer listener. Instead:
1. Peer listener sets `pending_request = 1`
2. Main thread checks this flag
3. Main thread handles "accept" or "reject" commands

```c
// Peer listener
pending_request = 1;
printf("Type 'accept' or 'reject'\n");

// Main thread
if (has_pending) {
    if (strcmp(input, "accept") == 0) {
        // Handle accept
    } else if (strcmp(input, "reject") == 0) {
        // Handle reject
    }
}
```

**Result:** Single thread (main) handles all stdin → no conflicts ✅

### Debugging Techniques Used

1. **Debug Printf Statements**
```c
printf("[DEBUG] Deserializing: %s\n", buffer);
printf("[DEBUG] type=%d, sender=%s, data=%d\n", msg->type, msg->sender, msg->data);
```

2. **Tracking Message Flow**
```c
printf("[DEBUG CLIENT] Sending: %s\n", buffer);
printf("[DEBUG SERVER] Received: %s\n", buffer);
```

3. **Monitoring Thread State**
```c
printf("User registered: %s (peer port: %d)\n", username, port);
printf("Chat request from %s to %s (target peer port: %d)\n", ...);
```

4. **Checking Critical Values**
```c
printf("[DEBUG] my_port=%d\n", my_port);
printf("[DEBUG] peer_socket=%d, in_chat=%d\n", peer_socket, in_chat);
```

---

## 8. Viva Questions & Answers

### Basic Concepts

**Q1: What is the difference between TCP and UDP?**

**A:** 
- **TCP (Transmission Control Protocol)**:
  - Connection-oriented (establish connection first)
  - Reliable (guaranteed delivery, in-order)
  - Error checking and retransmission
  - Flow control
  - Example use: HTTP, FTP, SSH, **our chat app**
  
- **UDP (User Datagram Protocol)**:
  - Connectionless (no connection setup)
  - Unreliable (no delivery guarantee)
  - Faster (less overhead)
  - No flow control
  - Example use: DNS, video streaming, gaming

We use TCP because chat requires reliable, ordered message delivery.

---

**Q2: What is socket programming?**

**A:** Socket programming is a way to enable communication between processes over a network using sockets as endpoints. A socket is identified by an IP address and port number. It provides an API for:
- Creating connections (client-server or peer-to-peer)
- Sending and receiving data
- Managing network resources

**Socket = IP Address + Port Number**

---

**Q3: Explain client-server architecture.**

**A:** 
- **Server**: Passive entity that listens for requests
  - Always running
  - Known address (IP:Port)
  - Serves multiple clients
  
- **Client**: Active entity that initiates communication
  - Connects to server
  - Sends requests
  - Receives responses

In our app, the directory server follows this model.

---

**Q4: What is peer-to-peer (P2P) architecture?**

**A:** In P2P, each node acts as both client and server. No central authority needed for data exchange. 

**Advantages:**
- Scalability (no server bottleneck)
- Direct communication (faster)
- Decentralized (no single point of failure)

**Disadvantages:**
- Complex (each peer must handle connections)
- Discovery problem (how to find peers?)

**Our approach:** Hybrid - directory server for discovery, P2P for data.

---

### Threading & Concurrency

**Q5: What is a thread?**

**A:** A thread is the smallest unit of execution within a process. Threads in the same process share:
- Memory space (global variables, heap)
- File descriptors
- Process ID

But each thread has:
- Own stack
- Own program counter
- Own register values

**Benefits:**
- Concurrent execution
- Responsive applications
- Efficient (less overhead than processes)

---

**Q6: What is a race condition?**

**A:** A race condition occurs when multiple threads access shared data simultaneously, and at least one modifies it, leading to unpredictable results.

**Example:**
```c
// Global variable
int client_count = 0;

// Thread A
client_count++;  // Read 0, increment, write 1

// Thread B (simultaneously)
client_count++;  // Read 0, increment, write 1

// Result: client_count = 1 (WRONG! Should be 2)
```

**Solution:** Use mutex locks to ensure mutual exclusion.

---

**Q7: How does mutex work?**

**A:** Mutex (Mutual Exclusion) is a locking mechanism.

```c
pthread_mutex_lock(&mutex);
// Only ONE thread can be here at a time
critical_section();
pthread_mutex_unlock(&mutex);
```

**Behavior:**
- First thread acquires lock → enters critical section
- Other threads block (wait) at lock
- When first thread unlocks, one waiting thread acquires lock
- Ensures serial access to shared resource

---

**Q8: What is the difference between pthread_join and pthread_detach?**

**A:** 
- **pthread_join(thread_id):**
  - Calling thread waits for specified thread to terminate
  - Retrieves return value
  - Resources held until joined
  - Use when you need thread's result
  
- **pthread_detach(thread_id):**
  - Thread runs independently
  - Resources automatically released on termination
  - Cannot join later
  - Use for "fire and forget" threads
  
We use detach because handler threads run indefinitely and we don't need their return values.

---

### Network Programming

**Q9: What is the difference between bind() and connect()?**

**A:** 
- **bind():** Server-side
  - Associates socket with local address (IP:Port)
  - Tells OS "I want to receive data on this port"
  - Required before listen()
  
- **connect():** Client-side
  - Initiates connection to server
  - Tells OS "I want to send data to this address"
  - Performs TCP handshake

---

**Q10: What is the three-way handshake in TCP?**

**A:** TCP connection establishment process:

```
Client                          Server
  |                               |
  |--- SYN (seq=x) -------------->|  (1. Client requests)
  |                               |
  |<-- SYN-ACK (seq=y, ack=x+1) -|  (2. Server acknowledges & requests)
  |                               |
  |--- ACK (ack=y+1) ------------>|  (3. Client acknowledges)
  |                               |
  [Connection Established]
```

- **SYN:** Synchronize sequence numbers
- **ACK:** Acknowledge receipt

This happens automatically when we call `connect()`!

---

**Q11: What is the difference between accept() and listen()?**

**A:** 
- **listen(sockfd, backlog):**
  - Marks socket as passive (accepting connections)
  - Sets maximum queue length for pending connections
  - Does NOT accept connections itself
  
- **accept(sockfd, addr, addrlen):**
  - Actually accepts a pending connection
  - Blocks until a client connects
  - Returns NEW socket for this specific client
  - Original socket remains for accepting more connections

**Analogy:** listen() = open shop, accept() = serve each customer

---

**Q12: Why does accept() return a new socket?**

**A:** 
- Original socket (listening socket): Used only to accept new connections
- New socket (connected socket): Used for data exchange with specific client
- This allows server to:
  - Continue accepting new clients (on original socket)
  - Serve existing clients (on their respective sockets)
  - Handle multiple clients concurrently

---

### Application-Specific

**Q13: Explain the architecture of your chat application.**

**A:** Our application uses a **hybrid architecture**:

1. **Centralized Directory Server (Port 8888):**
   - Maintains list of active users
   - Stores each user's IP address and P2P listening port
   - Handles user registration
   - Provides peer discovery (returns IP:port when requested)
   - Uses threading to handle multiple clients concurrently

2. **Distributed P2P Communication:**
   - Clients connect directly to each other for chatting
   - Each client listens on a unique port for incoming chat requests
   - Actual messages bypass the server (sent peer-to-peer)
   - Reduces server load and latency

**Benefits:**
- Server doesn't relay every message (scalable)
- Direct P2P is faster (no intermediary)
- Central directory solves peer discovery problem

---

**Q14: Why does each client need to listen on a port?**

**A:** For **incoming P2P connections**. 

**Scenario:**
- Alice wants to chat with Bob
- Alice needs to CONNECT to Bob
- Bob needs to ACCEPT Alice's connection
- For Bob to accept, Bob must be LISTENING on a port

**Flow:**
1. Bob runs: `./client 127.0.0.1 9001` (Bob listens on 9001)
2. Alice asks server: "Where is Bob?"
3. Server replies: "Bob is at 127.0.0.1:9001"
4. Alice connects to Bob at 127.0.0.1:9001
5. Bob's listener thread accepts Alice's connection

Without listening ports, peers couldn't accept incoming connections!

---

**Q15: How does your application handle multiple concurrent users?**

**A:** 

**Server side:**
- Uses **one thread per client**
- Each thread handles messages from one client independently
- Shared `clients[]` array protected by mutex
- Total threads = 1 (main) + N (client handlers)

**Client side:**
- Main thread: User input
- Server handler thread: Receives from directory server
- Peer listener thread: Accepts incoming P2P connections
- P2P chat thread: Handles active chat session
- Total threads = 4

**Concurrency benefits:**
- Multiple users can register simultaneously
- User can receive messages while typing
- Incoming chat requests handled while chatting with server

---

**Q16: Explain the message serialization protocol.**

**A:** We use a **pipe-delimited text format**:

```
type|sender|receiver|content|data
```

**Why text-based?**
- Human-readable (easy debugging)
- Platform-independent (no endianness issues)
- Simple to parse
- Flexible field sizes

**Why not binary?**
- Binary requires exact structure alignment
- Different machines may have different padding
- Harder to debug
- For production, binary would be more efficient (smaller size)

**Parsing challenge:** Empty fields create `||`. Standard `strtok()` fails, so we use manual parsing that handles consecutive delimiters correctly.

---

**Q17: How do you ensure thread safety in your application?**

**A:** We use **mutex locks** to protect shared data:

**Server:**
```c
pthread_mutex_t clients_mutex;

// Before accessing clients[] array
pthread_mutex_lock(&clients_mutex);
clients[i].username = username;  // Critical section
pthread_mutex_unlock(&clients_mutex);
```

**Client:**
```c
pthread_mutex_t chat_mutex;

// Before modifying chat state
pthread_mutex_lock(&chat_mutex);
in_chat = 1;
pending_request = 0;
pthread_mutex_unlock(&chat_mutex);
```

**What we protect:**
- Server: `clients[]` array (multiple threads read/write)
- Client: `in_chat`, `pending_request` flags (multiple threads check)

**What we DON'T protect:**
- Local variables (each thread has own stack)
- Socket I/O (each socket used by only one thread)

---

**Q18: What happens if a client crashes during a chat?**

**A:** 

**At TCP level:**
- OS detects connection loss
- `recv()` returns 0 (or -1 with error)

**Our handling:**
```c
bytes = recv(peer_socket, buffer, BUFFER_SIZE, 0);
if (bytes <= 0) {
    printf("[%s disconnected from chat]\n", chat_partner);
    in_chat = 0;
    close(peer_socket);
    break;  // Exit chat handler thread
}
```

**For directory server:**
- Same detection in client handler thread
- Marks user as inactive
- Notifies other users
- Broadcasts updated user list

**Graceful disconnect:**
- Client sends `MSG_DISCONNECT` before closing
- Cleaner, but not required (TCP handles ungraceful too)

---

**Q19: How would you scale this application to 10,000 users?**

**A:** Current design limitations and solutions:

**Current (Thread-per-client):**
- 10,000 users = 10,000 threads
- High memory usage (each thread has stack)
- Context switching overhead

**Solutions:**

1. **Event-driven I/O (epoll/select):**
   - Single thread monitors multiple sockets
   - OS notifies when data available
   - Much more scalable
   ```c
   epoll_wait(epoll_fd, events, MAX_EVENTS, timeout);
   for (each ready socket) {
       handle_message(socket);
   }
   ```

2. **Thread pool:**
   - Fixed number of worker threads (e.g., 100)
   - Queue of tasks
   - Threads pick tasks from queue

3. **Asynchronous I/O:**
   - Non-blocking sockets
   - Event loop (like Node.js)

4. **Load balancing:**
   - Multiple directory servers
   - Distribute users across servers
   - Use consistent hashing for user→server mapping

5. **Database for user list:**
   - Don't keep all users in memory
   - Store in Redis/MongoDB
   - Query on demand

**For P2P scaling:**
- P2P naturally scales (server doesn't handle messages)
- But NAT traversal becomes an issue (need STUN/TURN servers)

---

**Q20: What are the security concerns in your application?**

**A:** 

**Current vulnerabilities:**

1. **No Authentication:**
   - Anyone can register any username
   - No password verification
   - **Fix:** Add password-based auth, hashing (bcrypt)

2. **No Encryption:**
   - Messages sent in plain text
   - Anyone on network can read
   - **Fix:** Use TLS/SSL (like HTTPS)

3. **No Input Validation:**
   - Buffer overflow if message > MAX_MESSAGE_LEN
   - **Fix:** Strict bounds checking, sanitization

4. **No Rate Limiting:**
   - User can spam messages
   - **Fix:** Rate limit per user

5. **Denial of Service:**
   - Malicious user can flood server with connections
   - **Fix:** Connection limits per IP, CAPTCHA

6. **Username Spoofing:**
   - In P2P, client claims to be someone else
   - **Fix:** Cryptographic signatures, verify via server

**For production:**
- Use TLS for all connections
- Implement user authentication (OAuth, JWT)
- Validate and sanitize all inputs
- Add rate limiting and DoS protection
- Use prepared statements if adding database

**Edge cases we handle:**
- ✅ Duplicate username detection (prevents same username twice)
- ✅ Self-chat prevention (can't chat with yourself)
- ✅ Connection cleanup on crashes
- ✅ User busy state (auto-reject if already in chat)

---

**Q21: How do you handle duplicate usernames?**

**A:** During registration, the server checks if the username already exists:

```c
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
        // Send error and close connection
        send_error(client_index, "Username already taken");
        close(clients[client_index].socket_fd);
        pthread_mutex_unlock(&clients_mutex);
        goto cleanup;
    }
    // ... proceed with registration
}
```

**Benefits:**
- Ensures username uniqueness
- Client immediately knows to reconnect with different name
- Prevents confusion in chat (who is who?)

---

**Q22: What happens if someone tries to chat with themselves?**

**A:** The server detects and prevents this:

```c
case MSG_CHAT_REQUEST: {
    // Check if user is trying to chat with themselves
    if (strcmp(msg.sender, msg.receiver) == 0) {
        send_error(client_index, "Cannot chat with yourself");
        break;
    }
    // ... proceed with chat request
}
```

This prevents:
- Logical inconsistency (P2P connection to yourself)
- Resource waste (unnecessary socket)
- Confusing UI (messages appearing twice)

---

## 9. Code Walkthrough

### Key Functions Explained

#### 1. Server: `handle_client()`

```c
void *handle_client(void *arg) {
    int client_index = *((int *)arg);
    free(arg);  // Important! Free malloc'd argument
    
    // Message buffer and structure
    char buffer[BUFFER_SIZE];
    Message msg;
    int bytes_received;
    
    // Main message loop
    while (1) {
        // Clear buffer before each receive
        memset(buffer, 0, BUFFER_SIZE);
        
        // Blocking receive
        bytes_received = recv(clients[client_index].socket_fd, 
                             buffer, BUFFER_SIZE - 1, 0);
        
        // Check for disconnect
        if (bytes_received <= 0) {
            printf("Client disconnected\n");
            break;
        }
        
        // Null-terminate (safety)
        buffer[bytes_received] = '\0';
        
        // Parse message
        deserialize_message(buffer, &msg);
        
        // Handle based on type
        switch (msg.type) {
            case MSG_REGISTER:
                // Store user info (with mutex!)
                pthread_mutex_lock(&clients_mutex);
                strcpy(clients[client_index].username, msg.sender);
                clients[client_index].port = msg.data;
                clients[client_index].is_active = 1;
                pthread_mutex_unlock(&clients_mutex);
                
                // Confirm to user
                send_confirmation(client_index);
                
                // Notify others
                notify_user_joined(msg.sender);
                
                // Broadcast updated list
                broadcast_user_list();
                break;
                
            case MSG_CHAT_REQUEST:
                // Find target user
                int target_idx = find_user_by_username(msg.receiver);
                
                if (target_idx >= 0) {
                    // Send target's IP:port to requester
                    Message reply;
                    reply.type = MSG_CHAT_REQUEST;
                    strcpy(reply.content, clients[target_idx].ip_address);
                    reply.data = clients[target_idx].port;
                    send_message_to_client(client_index, &reply);
                } else {
                    // Send error
                    send_error(client_index, "User not found");
                }
                break;
        }
    }
    
    // Cleanup on disconnect
    pthread_mutex_lock(&clients_mutex);
    notify_user_left(clients[client_index].username);
    clients[client_index].is_active = 0;
    close(clients[client_index].socket_fd);
    pthread_mutex_unlock(&clients_mutex);
    
    return NULL;
}
```

**Key points:**
- `free(arg)`: Prevents memory leak (arg was malloc'd in main)
- `memset`: Clear buffer to avoid leftover data
- `bytes_received <= 0`: Detects disconnect
- Mutex around shared `clients[]` access
- Cleanup section runs when client disconnects

---

#### 2. Client: `server_handler()`

```c
void *server_handler(void *arg) {
    char buffer[BUFFER_SIZE];
    Message msg;
    
    while (running) {
        memset(buffer, 0, BUFFER_SIZE);
        int bytes = recv(server_socket, buffer, BUFFER_SIZE - 1, 0);
        
        if (bytes <= 0) {
            printf("Disconnected from server\n");
            running = 0;
            break;
        }
        
        buffer[bytes] = '\0';
        deserialize_message(buffer, &msg);
        
        switch (msg.type) {
            case MSG_USER_LIST:
                // Display user list
                display_user_list(msg.content);
                break;
                
            case MSG_CHAT_REQUEST:
                // Server sent us peer's IP:port
                char target_ip[INET_ADDRSTRLEN];
                int target_port = msg.data;
                strcpy(target_ip, msg.content);
                
                // Connect to peer
                int peer_fd = socket(AF_INET, SOCK_STREAM, 0);
                struct sockaddr_in peer_addr;
                peer_addr.sin_family = AF_INET;
                peer_addr.sin_port = htons(target_port);
                inet_pton(AF_INET, target_ip, &peer_addr.sin_addr);
                
                connect(peer_fd, (struct sockaddr *)&peer_addr, 
                       sizeof(peer_addr));
                
                // Send initial handshake
                Message init;
                init.type = MSG_CHAT_REQUEST;
                strcpy(init.sender, my_username);
                send_msg(peer_fd, &init);
                
                // Mark as in chat
                pthread_mutex_lock(&chat_mutex);
                peer_socket = peer_fd;
                in_chat = 1;
                pthread_mutex_unlock(&chat_mutex);
                
                // Start chat handler thread
                pthread_t chat_thread;
                int *fd_ptr = malloc(sizeof(int));
                *fd_ptr = peer_fd;
                pthread_create(&chat_thread, NULL, 
                              p2p_chat_handler, fd_ptr);
                pthread_detach(chat_thread);
                break;
                
            case MSG_ERROR:
                printf("[ERROR] %s\n", msg.content);
                break;
        }
    }
    
    return NULL;
}
```

**Key points:**
- Runs in background receiving server messages
- When server sends peer info (MSG_CHAT_REQUEST):
  - Extracts IP and port from message
  - Creates new socket and connects to peer
  - Sends P2P handshake
  - Starts chat handler thread
- Updates shared state (in_chat) with mutex

---

#### 3. Client: `peer_listener()`

```c
void *peer_listener(void *arg) {
    int listen_port = *((int *)arg);
    free(arg);
    
    int listen_fd;
    struct sockaddr_in addr;
    
    // Create listening socket
    listen_fd = socket(AF_INET, SOCK_STREAM, 0);
    
    // Allow port reuse (useful during development)
    int opt = 1;
    setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    
    // Bind to our peer port
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;  // Accept from any IP
    addr.sin_port = htons(listen_port);
    bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr));
    
    // Listen for connections
    listen(listen_fd, 5);  // Queue up to 5 pending
    
    printf("Listening for peer connections on port %d\n", listen_port);
    
    while (running) {
        // Accept incoming connection
        int new_peer = accept(listen_fd, NULL, NULL);
        
        if (new_peer < 0) continue;
        
        // Check if already in chat
        pthread_mutex_lock(&chat_mutex);
        if (in_chat) {
            // Busy, auto-reject
            Message reject;
            reject.type = MSG_CHAT_REJECT;
            strcpy(reject.content, "User busy");
            send_msg(new_peer, &reject);
            close(new_peer);
            pthread_mutex_unlock(&chat_mutex);
            continue;
        }
        pthread_mutex_unlock(&chat_mutex);
        
        // Receive initial handshake
        char buffer[BUFFER_SIZE];
        int bytes = recv(new_peer, buffer, BUFFER_SIZE - 1, 0);
        
        if (bytes > 0) {
            buffer[bytes] = '\0';
            Message msg;
            deserialize_message(buffer, &msg);
            
            if (msg.type == MSG_CHAT_REQUEST) {
                // Store partner name
                strcpy(chat_partner, msg.sender);
                
                // Prompt user
                printf("\nIncoming chat request from: %s\n", msg.sender);
                printf("Type 'accept' to accept or 'reject' to decline\n");
                
                // Set pending flag (main thread will handle accept/reject)
                pthread_mutex_lock(&chat_mutex);
                peer_socket = new_peer;
                pending_request = 1;
                pthread_mutex_unlock(&chat_mutex);
            }
        }
    }
    
    close(listen_fd);
    return NULL;
}
```

**Key points:**
- Runs in background accepting incoming P2P connections
- Checks if already busy (in_chat flag)
- Sets `pending_request` flag instead of blocking on stdin
- Main thread will handle "accept" or "reject" command

---

### Data Flow Example

**Scenario:** Alice wants to chat with Bob

```
Step 1: Alice types "chat bob"
├─ Main thread:
│  ├─ Parses command: target = "bob"
│  ├─ Creates MSG_CHAT_REQUEST
│  └─ Sends to directory server

Step 2: Server receives request
├─ handle_client() thread:
│  ├─ Looks up "bob" in clients[] array
│  ├─ Finds: IP=127.0.0.1, port=9001
│  ├─ Creates MSG_CHAT_REQUEST response
│  │  ├─ content = "127.0.0.1"
│  │  └─ data = 9001
│  └─ Sends to Alice

Step 3: Alice's server_handler receives response
├─ server_handler() thread:
│  ├─ Extracts IP and port
│  ├─ Creates socket
│  ├─ Connects to 127.0.0.1:9001
│  ├─ Sends MSG_CHAT_REQUEST handshake
│  ├─ Sets in_chat = 1
│  └─ Starts p2p_chat_handler thread

Step 4: Bob's peer_listener accepts connection
├─ peer_listener() thread:
│  ├─ accept() returns new socket
│  ├─ Receives MSG_CHAT_REQUEST handshake
│  ├─ Extracts sender = "alice"
│  ├─ Prints prompt
│  └─ Sets pending_request = 1

Step 5: Bob types "accept"
├─ Main thread:
│  ├─ Checks pending_request flag
│  ├─ Sends MSG_CHAT_ACCEPT to Alice
│  ├─ Sets in_chat = 1
│  └─ Starts p2p_chat_handler thread

Step 6: Both in chat mode
├─ Alice's main thread:
│  └─ User input → MSG_CHAT_MESSAGE → send to Bob
├─ Bob's p2p_chat_handler:
│  └─ Receives → displays message
├─ Bob's main thread:
│  └─ User input → MSG_CHAT_MESSAGE → send to Alice
└─ Alice's p2p_chat_handler:
   └─ Receives → displays message
```

---

## 10. Testing & Demonstration

### Test Scenario 1: Basic Chat

**Terminal 1 - Server:**
```bash
$ ./server
===========================================
Chat Server started on port 8888
Waiting for client connections...
===========================================
```

**Terminal 2 - Alice:**
```bash
$ ./client 127.0.0.1 9000
Enter your username: alice
Listening for peer connections on port 9000
Registration successful

>> list
===========================================
Active Users:
===========================================
1. bob
===========================================

>> chat bob
Request sent via server. Waiting for bob to accept...
[SERVER] Peer info: 127.0.0.1:9001
Connecting to peer...
Chat started with bob. Type 'exit' to end chat.
>> Hello Bob!
[bob]: Hi Alice!
>> How are you?
[bob]: I'm doing great!
>> exit
Chat ended.
>>
```

**Terminal 3 - Bob:**
```bash
$ ./client 127.0.0.1 9001
Enter your username: bob
Listening for peer connections on port 9001
Registration successful

>>
===========================================
Incoming chat request from: alice
Type 'accept' to accept or 'reject' to decline
===========================================
>> accept
Chat started with alice. Type 'exit' to end chat.
>> [alice]: Hello Bob!
Hi Alice!
>> [alice]: How are you?
I'm doing great!
>> [alice has ended the chat]
>>
```

**Server Output:**
```
New connection from 127.0.0.1:45678 (socket: 4)
Client handler started for socket 4
User registered: alice (peer port: 9000)

New connection from 127.0.0.1:45679 (socket: 5)
Client handler started for socket 5
User registered: bob (peer port: 9001)

Chat request from alice to bob (target peer port: 9001)
```

### Test Scenario 2: User Leaves

**Bob presses Ctrl+C:**

**Alice sees:**
```
[SYSTEM] bob has left the chat
>> list
===========================================
Active Users:
===========================================
No other users online
===========================================
```

**Server sees:**
```
Client bob disconnected
Client handler terminated for bob
```

### Test Scenario 3: Reject Chat

**Terminal 2 - Alice:**
```bash
>> chat bob
Request sent via server. Waiting for bob to accept...
```

**Terminal 3 - Bob:**
```bash
===========================================
Incoming chat request from: alice
Type 'accept' to accept or 'reject' to decline
===========================================
>> reject
Chat request rejected.
>>
```

**Terminal 2 - Alice:**
```bash
>> [ERROR] Chat rejected
>>
```

### Common Issues During Demo

**Issue: "Connection refused"**
- **Cause:** Target client not listening on specified port
- **Check:** Ensure client started with correct peer port
- **Verify:** Look for "Listening for peer connections on port XXXX"

**Issue: "User not found"**
- **Cause:** Typo in username or user disconnected
- **Solution:** Use `list` to see exact usernames

**Issue: Port already in use**
- **Cause:** Previous client didn't close properly
- **Solution:** Use different port or `killall client`

---

## Summary for Viva

### What to Emphasize

1. **Architecture:**
   - Hybrid (centralized directory + distributed P2P)
   - Server for discovery, P2P for efficiency

2. **Concurrency:**
   - Thread-per-client model
   - Mutex for thread safety
   - Multiple concurrent operations

3. **Protocol:**
   - Custom text-based serialization
   - Pipe-delimited format
   - Handles empty fields correctly

4. **Key Challenges Solved:**
   - Empty field serialization (strtok bug)
   - stdin conflict (scanf vs fgets)
   - Thread synchronization
   - Peer discovery

5. **Technologies Used:**
   - TCP sockets (reliable communication)
   - POSIX threads (concurrency)
   - Mutex locks (thread safety)
   - Custom protocol (message format)

### Quick Facts to Remember

- **Server Port:** 8888
- **Default Client P2P Ports:** 9000, 9001, 9002, etc.
- **Max Clients:** 100 (configurable)
- **Message Types:** 10 (REGISTER, USER_LIST, CHAT_REQUEST, etc.)
- **Thread Count (Server):** 1 + N (N = number of clients)
- **Thread Count (Client):** 4 (main, server_handler, peer_listener, p2p_chat)
- **Buffer Size:** 2048 bytes
- **Max Username:** 32 characters
- **Max Message:** 1024 characters

### Demonstration Flow

1. Start server
2. Start 2-3 clients
3. Show `list` command
4. Initiate chat between two clients
5. Show message exchange
6. End chat with `exit`
7. Show user leaving notification
8. Explain code while running

### Code Sections to Know Well

1. `handle_client()` in server.c - Message routing
2. `deserialize_message()` - Protocol parsing
3. `server_handler()` in client.c - P2P initiation
4. `peer_listener()` in client.c - Accept incoming chats
5. Main server loop - Thread creation
6. Main client loop - Command handling

---

## Additional Resources

### Commands Reference

```bash
# Compilation
make all              # Build everything
make server           # Build server only
make client           # Build client only
make clean            # Remove binaries

# Running
./server              # Start directory server
./client <ip> <port>  # Start client

# In-app Commands
list                  # Show active users
chat <user>           # Start chat
accept                # Accept incoming request
reject                # Reject incoming request
exit                  # End chat or quit
```

### File Locations

- Source: `/home/iamthope/nw_lab/chat_application/`
- Server: `server.c`
- Client: `client.c`
- Common: `common.h`
- Build: `Makefile`
- Docs: `README.md`, `TEST_INSTRUCTIONS.md`, `VIVA_PREPARATION.md`

---

**Good luck with your viva! 🎓**

Remember: Understand the concepts, don't just memorize. Be ready to explain WHY design decisions were made, not just WHAT was implemented.
