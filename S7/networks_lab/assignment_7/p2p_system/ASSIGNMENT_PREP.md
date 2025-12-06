# P2P Assignment - Preparation Guide

## ✅ What You Have (Features Checklist)

### Core Requirements Met:
- ✅ **Directory Server**: Central server tracks peers and their files
- ✅ **Packet Protocol**: Types 1 (Register), 2 (Query), 3 (Response)
- ✅ **Peer Registration**: Peers send their port + file list to server
- ✅ **File Search**: Query server for file locations
- ✅ **P2P Transfer**: Direct file transfer between peers
- ✅ **Multi-threading**: `pthread` for concurrent connections
- ✅ **Thread Safety**: Mutex-protected shared data structures
- ✅ **Concurrent Operations**: ⭐ **THIS IS KEY FOR YOUR DEMO**

### Concurrent Transfer Feature (Your Assignment Requirement):
✅ **Peer acts as BOTH client AND server simultaneously**
- Peer 1 downloads from Peer 3 (acting as CLIENT)
- WHILE Peer 2 downloads from Peer 1 (Peer 1 acting as SERVER)
- Both transfers happen AT THE SAME TIME
- Implemented via separate server thread + client operations

---

## 🎯 How Concurrent Transfer Works in Your Code

### Architecture:
```
Peer 1 (Port 3300)
├── Main Thread: Menu + Client operations (downloads from Peer 3)
└── Server Thread: Listens on 3300, serves files to Peer 2

Simultaneous:
- Main thread: Peer 1 → connects to Peer 3 (downloads fileE.txt)
- Server thread: Peer 2 → connects to Peer 1 (downloads fileA.txt)
```

### Code Implementation:

**In `peer.c` - Line 88:**
```c
void *peer_server(void *arg) {
    // This thread runs CONTINUOUSLY in background
    // Accepts connections from other peers
    // Each request handled in NEW thread
    while (true) {
        client_socket = accept(...);
        pthread_create(&thread_id, NULL, handle_peer_request, ...);
        pthread_detach(thread_id);  // Non-blocking
    }
}
```

**In `main()` - Line 410:**
```c
// Start peer server thread (background)
pthread_create(&server_thread, NULL, peer_server, NULL);
pthread_detach(server_thread);  // Runs independently

// Main loop for user menu (client operations)
while (1) {
    // Can search and download (client)
    // While server thread handles uploads
}
```

**Key Point for Evaluator:**
- Server thread is DETACHED → runs independently
- Main thread doesn't block on server operations
- Multiple simultaneous connections possible

---

## 📝 DEMO SCRIPT for Evaluators

### Step 1: Setup (Show this first)
```bash
cd /mnt/e/College_Assignments/S7/networks_lab/assignment_7/p2p_system

# Show directory structure
ls -la peer1/shared/
ls -la peer2/shared/
ls -la peer3/shared/

# Show code has threading
grep -n "pthread_create" peer.c
grep -n "pthread_detach" peer.c
```

### Step 2: Start System
**Terminal 1 - Server:**
```bash
./server
```

**Terminal 2 - Peer 1:**
```bash
cd peer1
../peer 3300
# Choose: 1 (Login)
```

**Terminal 3 - Peer 2:**
```bash
cd peer2
../peer 3301
# Choose: 1 (Login)
```

**Terminal 4 - Peer 3:**
```bash
cd peer3
../peer 3302
# Choose: 1 (Login)
```

### Step 3: DEMONSTRATE CONCURRENT TRANSFER ⭐

**This is the critical demo for your assignment!**

**In Terminal 2 (Peer 1) - Start download:**
```
Choice: 2
Filename: fileE.txt
Select: 1 (Peer 3)
```

**IMMEDIATELY - In Terminal 3 (Peer 2) - Start another download:**
```
Choice: 2
Filename: fileA.txt
Select: 1 (Peer 1)
```

**What happens:**
1. Peer 1 is downloading fileE.txt FROM Peer 3 (Peer 1 = CLIENT)
2. AT THE SAME TIME, Peer 2 is downloading fileA.txt FROM Peer 1 (Peer 1 = SERVER)
3. Both complete successfully!

**Show Terminal 2 output:**
```
Downloading file (size: 27 bytes)...
File request: fileA.txt from 127.0.0.1  ← Server serving Peer 2
File downloaded successfully...          ← Client downloaded from Peer 3
Sent fileA.txt to 127.0.0.1             ← Confirmed upload
```

**Verify:**
```bash
# Peer 1 received file from Peer 3
ls -lh peer1/downloads/fileE.txt

# Peer 2 received file from Peer 1
ls -lh peer2/downloads/fileA.txt
```

---

## 🎤 Answers to Evaluator Questions

### Q1: "How does concurrent transfer work?"
**Answer:**
"Each peer runs TWO roles simultaneously:
1. **Server role**: Background thread listens on assigned port, handles incoming requests in separate threads (pthread_create + pthread_detach)
2. **Client role**: Main thread handles user menu, initiates downloads

When Peer 1 downloads from Peer 3, the main thread makes the connection. Meanwhile, the server thread independently accepts Peer 2's connection and serves the file. Both happen at the same time because of threading."

### Q2: "Show me the threading code"
**Answer:**
```c
// In main() - Line 410
pthread_t server_thread;
pthread_create(&server_thread, NULL, peer_server, NULL);
pthread_detach(server_thread);  // Non-blocking

// In peer_server() - Line 120
pthread_t thread_id;
pthread_create(&thread_id, NULL, handle_peer_request, client_sock_ptr);
pthread_detach(thread_id);  // Each request in separate thread
```

### Q3: "What if multiple peers request files simultaneously?"
**Answer:**
"The server thread accepts connections in a loop. Each accepted connection spawns a NEW thread via handle_peer_request(). These threads are detached, so they run independently. No blocking - can handle 10, 20, 100+ simultaneous requests."

### Q4: "How do you prevent race conditions?"
**Answer:**
```c
// In server.c - mutex protection
pthread_mutex_t list_mutex = PTHREAD_MUTEX_INITIALIZER;

void add_peer(...) {
    pthread_mutex_lock(&list_mutex);
    // Modify peer_list
    pthread_mutex_unlock(&list_mutex);
}
```

### Q5: "Show infinite loop for connections"
**Answer:**
```c
// peer_server() - Line 100
while (1) {  // Infinite loop
    client_socket = accept(server_fd, ...);
    // Handle in new thread
}

// main() - Line 420
while (1) {  // Menu loop - doesn't terminate
    // User interactions
}
```

---

## 🔧 Common Modification Questions & Solutions

### Mod 1: "Add file size limit"
```c
// In download_file_from_peer()
#define MAX_FILE_SIZE (10 * 1024 * 1024)  // 10MB

if (file_size > MAX_FILE_SIZE) {
    printf("Error: File too large (%ld bytes)\n", file_size);
    return;
}
```

### Mod 2: "Add file type filtering (only .txt)"
```c
// In get_shared_files()
char *ext = strrchr(entry->d_name, '.');
if (ext && strcmp(ext, ".txt") == 0) {
    strcpy(files[count], entry->d_name);
    count++;
}
```

### Mod 3: "Add peer disconnect handling"
```c
// In server.c - Packet Type 4: Peer Offline
if (packet_type == 4) {
    int port;
    sscanf(buffer + 2, "%d", &port);
    remove_peer(port);  // Remove from list
}
```

### Mod 4: "Add file transfer progress"
```c
// In download_file_from_peer()
while ((bytes = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
    fwrite(buffer, 1, bytes, file);
    received += bytes;
    printf("\rProgress: %ld / %ld bytes (%.1f%%)", 
           received, file_size, (received * 100.0) / file_size);
    fflush(stdout);
}
```

### Mod 5: "Limit number of concurrent downloads"
```c
#define MAX_CONCURRENT 5
sem_t download_semaphore;

// In main()
sem_init(&download_semaphore, 0, MAX_CONCURRENT);

// In download_file_from_peer()
sem_wait(&download_semaphore);
// ... download code ...
sem_post(&download_semaphore);
```

---

## 📚 Technical Concepts to Review

### 1. **Threading Model**
- `pthread_create()` - Create new thread
- `pthread_detach()` - Make thread independent
- `pthread_mutex_lock/unlock()` - Synchronization

### 2. **Socket Programming**
- `socket()` - Create endpoint
- `bind()` - Assign address
- `listen()` - Wait for connections
- `accept()` - Accept connection
- `connect()` - Initiate connection
- `send()/recv()` - Data transfer

### 3. **Protocol Design**
- Packet Type 1: `"1|3300|file1.txt|file2.txt"`
- Packet Type 2: `"2|filename.txt"`
- Packet Type 3: `"3|3300|3301|3302"`

### 4. **File I/O**
- `opendir()/readdir()` - Directory listing
- `fopen()/fread()/fwrite()` - File operations
- Binary mode (`"rb"`, `"wb"`) for all file types

---

## 🎯 Key Points to Emphasize

1. **Concurrent = Peer is CLIENT and SERVER at same time** ✅
2. **Threading = Multiple connections handled simultaneously** ✅
3. **Infinite loops = System doesn't terminate after one transfer** ✅
4. **Mutex = Thread-safe shared data** ✅
5. **Detached threads = Non-blocking operations** ✅

---

## 🧪 Quick Test Commands

```bash
# Build
make clean && make

# Setup (from p2p_system directory)
cd peer1 && ../peer 3300 &
cd peer2 && ../peer 3301 &
cd peer3 && ../peer 3302 &

# Verify all listening
lsof -i :3300
lsof -i :3301
lsof -i :3302

# Check processes
ps aux | grep peer
```

---

## 📊 Assignment Evaluation Rubric (Likely)

| Criteria | Points | Your Status |
|----------|--------|-------------|
| Directory server working | 20 | ✅ |
| Packet protocol correct | 15 | ✅ |
| File search & transfer | 20 | ✅ |
| **Concurrent operations** | 25 | ✅ **KEY DEMO** |
| Multi-threading | 10 | ✅ |
| Code quality | 10 | ✅ |
| **TOTAL** | **100** | **Ready** |

---

## ⚠️ Critical: How to Run Properly

**WRONG** (Your current approach):
```bash
./peer 3300  # From p2p_system directory - no files!
```

**CORRECT**:
```bash
cd peer1
../peer 3300  # Runs from peer1/, sees peer1/shared/
```

---

## 🎬 30-Second Elevator Pitch

*"My P2P system uses a centralized directory server that tracks which peers have which files. Each peer runs TWO threads: a server thread that continuously listens for incoming file requests, and a main thread for the user interface and client operations. This allows a single peer to upload a file to one peer while simultaneously downloading from another peer, demonstrating true concurrent operation with pthread. The system uses mutex locks for thread safety and implements a custom packet protocol with types 1, 2, and 3 for registration, queries, and responses."*

---

**You're ready for evaluation! Focus on demonstrating the concurrent transfer.**
