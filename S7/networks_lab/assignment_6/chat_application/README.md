# Concurrent Chat Application

A fully concurrent peer-to-peer chat application implemented in C with a central directory server that manages user registration and facilitates peer discovery.

## Table of Contents
- [Overview](#overview)
- [Architecture](#architecture)
- [Features](#features)
- [Requirements](#requirements)
- [Compilation](#compilation)
- [Usage](#usage)
- [Implementation Details](#implementation-details)
- [Protocol Design](#protocol-design)
- [Limitations and Future Enhancements](#limitations-and-future-enhancements)

---

## Overview

This chat application consists of two main components:
1. **Server (Directory Server)**: Manages user registration, maintains a list of active users, and facilitates peer discovery
2. **Client**: Connects to the server for directory services and establishes direct peer-to-peer connections for chatting

The system supports multiple concurrent users and allows any user to initiate a private chat with any other online user.

---

## Architecture

### System Design

```
┌─────────────────────────────────────────────────────────────┐
│                      Directory Server                        │
│  - Listens on port 8888                                     │
│  - Maintains active user list                               │
│  - Handles user registration                                │
│  - Facilitates peer discovery                               │
│  - Broadcasts user join/leave notifications                 │
└─────────────────────────────────────────────────────────────┘
                            ▲ ▲ ▲
                            │ │ │
                    ┌───────┘ │ └───────┐
                    │         │         │
                    ▼         ▼         ▼
           ┌────────────┬────────────┬────────────┐
           │  Client A  │  Client B  │  Client C  │
           │            │            │            │
           │ Port: 9000 │ Port: 9001 │ Port: 9002 │
           └────────────┴────────────┴────────────┘
                    │                   │
                    │  P2P Connection   │
                    └───────────────────┘
```

### Communication Model

1. **Client-Server Communication**: 
   - TCP connection for user registration and directory services
   - Persistent connection maintained throughout session
   - Server broadcasts user list updates to all connected clients

2. **Peer-to-Peer Communication**:
   - Direct TCP connections between clients for private chat
   - Each client listens on a unique port for incoming peer connections
   - Chat messages bypass the server entirely

---

## Features

### Server Features
- ✅ Concurrent handling of multiple client connections using POSIX threads
- ✅ Maintains real-time list of active users
- ✅ Thread-safe operations with mutex locks
- ✅ Automatic cleanup when clients disconnect
- ✅ Broadcasts user join/leave notifications
- ✅ Graceful shutdown handling (CTRL+C)

### Client Features
- ✅ User registration with custom username
- ✅ View list of active users
- ✅ Initiate chat with any online user
- ✅ Accept or reject incoming chat requests
- ✅ Simultaneous listening for incoming connections
- ✅ Clean exit from chat sessions
- ✅ Real-time notifications for user join/leave events

---

## Requirements

### Software Requirements
- GCC compiler (with C99 support or later)
- POSIX-compliant operating system (Linux, macOS, Unix)
- pthread library
- Standard C libraries

### System Requirements
- Minimum 2 terminal windows for testing (1 server + 1 client)
- Recommended: 3+ terminals for full testing (1 server + 2+ clients)

---

## Compilation

### Using Makefile (Recommended)

```bash
# Compile both server and client
make all

# Compile only server
make server

# Compile only client
make client

# Clean build files
make clean

# View help
make help
```

### Manual Compilation

```bash
# Compile server
gcc -Wall -Wextra -pthread -g server.c -o server -lpthread

# Compile client
gcc -Wall -Wextra -pthread -g client.c -o client -lpthread
```

---

## Usage

### Starting the Server

```bash
./server
```

The server will start listening on port **8888** and display connection status.

**Expected Output:**
```
===========================================
Chat Server started on port 8888
Waiting for client connections...
===========================================
```

### Starting a Client

```bash
# Default: Connect to localhost on default port
./client

# Specify server IP
./client <server_ip>

# Specify server IP and custom peer listening port
./client <server_ip> <peer_port>
```

**Example:**
```bash
# Client 1
./client 127.0.0.1 9000

# Client 2 (in another terminal)
./client 127.0.0.1 9001

# Client 3 (in another terminal)
./client 127.0.0.1 9002
```

### Client Commands

Once connected, the client provides the following commands:

| Command | Description |
|---------|-------------|
| `list` | Display all active users |
| `chat <username>` | Initiate a chat with specified user (username only, no spaces) |
| `accept` | Accept an incoming chat request |
| `reject` | Reject an incoming chat request |
| `exit` | Exit current chat or quit application |

**Important:** 
- The `chat` command takes **only the username** as an argument
- Do NOT include messages or extra words after the username
- Correct: `chat Bob`
- Incorrect: `chat Bob hi` (this will try to chat with user "Bob hi")

### Example Workflow

**Terminal 1 (Server):**
```bash
$ ./server
===========================================
Chat Server started on port 8888
Waiting for client connections...
===========================================

New connection from 127.0.0.1:45678 (socket: 4)
User registered: Alice
User registered: Bob
Chat request from Alice to Bob
```

**Terminal 2 (Client - Alice):**
```bash
$ ./client 127.0.0.1 9000
===========================================
Connected to chat server
===========================================
Enter your username: Alice
Registration successful
Listening for peer connections on port 9000

Commands:
  list           - Show active users
  chat <user>    - Start chat with user
  exit           - Exit chat or quit application
===========================================

>> list
===========================================
Active Users:
===========================================
1. Bob
===========================================

>> chat Bob
Connecting to Bob...
Request sent via server. Waiting for Bob to accept...
Chat started with Bob. Type 'exit' to end chat.
===========================================
>> Hello Bob!
[Bob]: Hi Alice!
>> How are you?
[Bob]: I'm good, thanks!
>> exit
Chat ended.
>>
```

**Terminal 3 (Client - Bob):**
```bash
$ ./client 127.0.0.1 9001
===========================================
Connected to chat server
===========================================
Enter your username: Bob
Registration successful
Listening for peer connections on port 9001

Commands:
  list           - Show active users
  chat <user>    - Start chat with user
  accept         - Accept incoming chat request
  reject         - Reject incoming chat request
  exit           - Exit chat or quit application
===========================================

>> 
===========================================
Incoming chat request from: Alice
Type 'accept' to accept or 'reject' to decline
===========================================
>> accept
Chat started with Alice. Type 'exit' to end chat.
===========================================
>> [Alice]: Hello Bob!
Hi Alice!
>> [Alice]: How are you?
I'm good, thanks!
>> [Alice has ended the chat]
>>
```

---

## Implementation Details

### File Structure

```
chat_application/
├── common.h        # Shared header with structures and constants
├── server.c        # Directory server implementation
├── client.c        # Client implementation
├── Makefile        # Build automation
└── README.md       # This file
```

### Key Data Structures

#### Message Structure
```c
typedef struct {
    int type;                          // Message type (see protocol)
    char sender[MAX_USERNAME_LEN];     // Sender username
    char receiver[MAX_USERNAME_LEN];   // Receiver username
    char content[MAX_MESSAGE_LEN];     // Message content
    int data;                          // Additional numeric data
} Message;
```

#### User Structure (Server-side)
```c
typedef struct {
    int socket_fd;                     // Client socket descriptor
    char username[MAX_USERNAME_LEN];   // Username
    char ip_address[INET_ADDRSTRLEN]; // IP address
    int port;                          // Peer listening port
    int is_active;                     // Active status
    int in_chat;                       // Currently in chat
    pthread_mutex_t mutex;             // Thread safety
} User;
```

### Thread Model

#### Server Threads
- **Main Thread**: Accepts new client connections
- **Client Handler Threads**: One per connected client, handles registration and message routing

#### Client Threads
- **Main Thread**: User input and command processing
- **Server Handler Thread**: Receives messages from directory server
- **Peer Listener Thread**: Listens for incoming P2P connection requests
- **P2P Chat Handler Thread**: Handles active peer-to-peer chat session

### Synchronization

- **Server**: Uses `pthread_mutex_t` to protect the shared client list
- **Client**: Uses mutex to protect chat state (in_chat flag)
- All critical sections are properly synchronized to prevent race conditions

---

## Protocol Design

### Message Types

| Type | Name | Description |
|------|------|-------------|
| 1 | `MSG_REGISTER` | User registration request/response |
| 2 | `MSG_USER_LIST` | Active user list broadcast |
| 3 | `MSG_CHAT_REQUEST` | Request to start a chat |
| 4 | `MSG_CHAT_ACCEPT` | Accept a chat request |
| 5 | `MSG_CHAT_REJECT` | Reject a chat request |
| 6 | `MSG_CHAT_MESSAGE` | Actual chat message |
| 7 | `MSG_DISCONNECT` | User disconnect notification |
| 8 | `MSG_USER_JOINED` | New user joined notification |
| 9 | `MSG_USER_LEFT` | User left notification |
| 10 | `MSG_ERROR` | Error message |

### Message Format

Messages are serialized as pipe-delimited strings:
```
<type>|<sender>|<receiver>|<content>|<data>
```

**Example:**
```
6|Alice|Bob|Hello Bob!|0
```

This format is simple, human-readable (for debugging), and efficient for our use case.

---

## Limitations and Future Enhancements

### Current Limitations

1. **Localhost Only**: Current implementation assumes all clients are on localhost (127.0.0.1)
2. **Port Management**: Simple port calculation (my_port + 1) may cause conflicts
3. **No Encryption**: Messages are sent in plain text
4. **No Persistence**: Chat history is not saved
5. **One-to-One Chat**: Only supports private conversations between two users
6. **No File Transfer**: Text messages only

### Potential Enhancements

- ✨ **Dynamic IP Resolution**: Implement proper IP address exchange via the server
- ✨ **Group Chat**: Support for multi-user chat rooms
- ✨ **Encryption**: Add TLS/SSL for secure communication
- ✨ **Chat History**: Store and retrieve previous conversations
- ✨ **File Transfer**: Support sending files between users
- ✨ **User Authentication**: Add password-based authentication
- ✨ **GUI**: Develop a graphical user interface
- ✨ **Message Delivery Confirmation**: Implement ACK mechanism
- ✨ **Offline Messages**: Queue messages for offline users
- ✨ **Port Allocation**: Dynamic port assignment by server

---

## Testing

### Basic Test Scenario

1. **Start the server:**
   ```bash
   ./server
   ```

2. **Open 3 client terminals and connect:**
   ```bash
   # Terminal 2
   ./client 127.0.0.1 9000
   # Enter username: Alice

   # Terminal 3
   ./client 127.0.0.1 9001
   # Enter username: Bob

   # Terminal 4
   ./client 127.0.0.1 9002
   # Enter username: Charlie
   ```

3. **Test user list:**
   - In Alice's terminal: `list`
   - Should see Bob and Charlie

4. **Test chat:**
   - In Alice's terminal: `chat Bob`
   - In Bob's terminal: Accept the chat request
   - Exchange messages
   - Type `exit` to end chat

5. **Test notifications:**
   - Start a new client
   - Existing clients should see join notification
   - Close a client (Ctrl+C)
   - Remaining clients should see leave notification

### Stress Test

- Connect 10+ clients simultaneously
- Initiate multiple concurrent chats
- Monitor server resource usage
- Verify no race conditions or deadlocks

---

## Troubleshooting

### Port Already in Use
```
Error: Bind failed: Address already in use
```
**Solution:** Either stop the previous instance or change the port in `common.h`

### Connection Refused
```
Error: Connection to server failed: Connection refused
```
**Solution:** Ensure the server is running before starting clients

### Compilation Errors
```
Error: pthread.h: No such file or directory
```
**Solution:** Ensure pthread library is installed:
```bash
sudo apt-get install build-essential  # Ubuntu/Debian
```

---

## License

This project is created for educational purposes as part of a networking lab assignment.

---

## Author

Created as part of Network Programming Lab Assignment
Date: October 20, 2025

---

## Acknowledgments

- POSIX Threads Programming
- Beej's Guide to Network Programming
- Unix Socket Programming

---

**Note:** This is a demonstration implementation. For production use, additional security measures, error handling, and testing would be required.
