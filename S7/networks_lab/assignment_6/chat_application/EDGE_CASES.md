# Edge Cases Handled

This document lists all edge cases and error conditions that the chat application handles gracefully.

## 1. Duplicate Username Prevention ✅

**Scenario:** User tries to register with a username that's already taken.

**Detection:**
- Server checks all active clients during registration
- Compares new username against existing ones

**Handling:**
```c
// In server.c, MSG_REGISTER handler
for (int i = 0; i < client_count; i++) {
    if (clients[i].is_active && strcmp(clients[i].username, msg.sender) == 0) {
        duplicate = 1;
        break;
    }
}

if (duplicate) {
    send_error(client_index, "Username already taken");
    close(socket);
    goto cleanup;
}
```

**User Experience:**
```
$ ./client 127.0.0.1 9002
Enter your username: alice
[ERROR] Username already taken. Please disconnect and try a different username.
[Connection closes]
```

**Server Log:**
```
Registration rejected: alice (username already taken)
```

---

## 2. Self-Chat Prevention ✅

**Scenario:** User tries to chat with themselves.

**Detection:**
- Server compares sender and receiver in MSG_CHAT_REQUEST

**Handling:**
```c
// In server.c, MSG_CHAT_REQUEST handler
if (strcmp(msg.sender, msg.receiver) == 0) {
    send_error(client_index, "Cannot chat with yourself");
    break;
}
```

**User Experience:**
```
>> chat alice
Request sent via server. Waiting for alice to accept...
[ERROR] Cannot chat with yourself
>>
```

**Server Log:**
```
Chat request rejected: alice tried to chat with themselves
```

**Why This Matters:**
- Prevents logical inconsistency (connecting to yourself)
- Avoids resource waste (unnecessary socket creation)
- Prevents confusing UI (messages appearing twice)

---

## 3. Client Disconnection During Chat ✅

**Scenario:** Client crashes or loses network connection while chatting.

**Detection:**
- TCP detects broken connection
- `recv()` returns 0 or -1 with error

**Handling:**
```c
// In p2p_chat_handler thread
bytes = recv(peer_fd, buffer, BUFFER_SIZE, 0);
if (bytes <= 0) {
    printf("\n[%s disconnected from chat]\n", chat_partner);
    in_chat = 0;
    close(peer_fd);
    break;
}
```

**User Experience:**
```
[alice]: Hello!
[alice disconnected from chat]
>>
```

**Cleanup:**
- P2P chat handler thread exits
- Socket closed
- Chat state reset to allow new chats

---

## 4. Client Disconnection from Server ✅

**Scenario:** Client exits or crashes while connected to directory server.

**Detection:**
- Server's `recv()` in client handler returns 0 or -1

**Handling:**
```c
// In handle_client thread
bytes_received = recv(clients[client_index].socket_fd, buffer, BUFFER_SIZE, 0);
if (bytes_received <= 0) {
    printf("Client %s disconnected\n", clients[client_index].username);
    break;
}

// Cleanup section
notify_user_left(clients[client_index].username);
clients[client_index].is_active = 0;
close(clients[client_index].socket_fd);
broadcast_user_list();
```

**User Experience (other clients):**
```
[SYSTEM] alice has left the chat
```

**Server Log:**
```
Client alice disconnected
Client handler terminated for alice
```

---

## 5. User Already in Chat ✅

**Scenario:** User receives chat request while already chatting with someone else.

**Detection:**
- Peer listener checks `in_chat` flag before accepting

**Handling:**
```c
// In peer_listener thread
pthread_mutex_lock(&chat_mutex);
if (in_chat) {
    // Auto-reject
    Message reject;
    reject.type = MSG_CHAT_REJECT;
    strcpy(reject.content, "User busy");
    send_msg(new_peer, &reject);
    close(new_peer);
    pthread_mutex_unlock(&chat_mutex);
    continue;
}
pthread_mutex_unlock(&chat_mutex);
```

**User Experience (requester):**
```
>> chat bob
Request sent via server. Waiting for bob to accept...
[ERROR] User busy
>>
```

---

## 6. Target User Offline ✅

**Scenario:** User tries to chat with someone who has disconnected.

**Detection:**
- Server's `find_user_by_username()` returns -1

**Handling:**
```c
// In server.c, MSG_CHAT_REQUEST handler
int target_index = find_user_by_username(msg.receiver);
if (target_index < 0) {
    send_error(client_index, "User not found or offline");
    break;
}
```

**User Experience:**
```
>> chat bob
Request sent via server. Waiting for bob to accept...
[ERROR] User not found or offline
>>
```

---

## 7. Chat Request Rejection ✅

**Scenario:** Target user rejects incoming chat request.

**Detection:**
- Target sends MSG_CHAT_REJECT

**Handling:**
```c
// Target user types: reject
Message reply;
reply.type = MSG_CHAT_REJECT;
strcpy(reply.content, "Chat rejected");
send_msg(peer_socket, &reply);
close(peer_socket);
```

**User Experience (requester):**
```
>> chat alice
Request sent via server. Waiting for alice to accept...
[Connection attempt fails or receives reject notification]
>>
```

**User Experience (target):**
```
Incoming chat request from: bob
Type 'accept' to accept or 'reject' to decline
>> reject
Chat request rejected.
>>
```

---

## 8. Invalid Username in Chat Command ✅

**Scenario:** User types username with typo or non-existent user.

**Detection:**
- Server can't find username in active users list

**Handling:**
- Same as "Target User Offline" (case 6)

**User Experience:**
```
>> chat boob
Request sent via server. Waiting for boob to accept...
[ERROR] User not found or offline
>>
```

**Fix:**
```
>> list
Active Users:
1. bob
2. alice

>> chat bob
[Success]
```

---

## 9. Extra Text in Chat Command ✅

**Scenario:** User types `chat bob hello` instead of `chat bob`.

**Detection:**
- Client parses everything after "chat " as username
- Server tries to find user "bob hello" (fails)

**Handling:**
- Falls into "User not found" error

**User Experience:**
```
>> chat bob hello
Request sent via server. Waiting for bob hello to accept...
[ERROR] User not found or offline
>>
```

**Documentation:**
- README clearly states: "chat <username> (username only, no spaces)"
- TEST_INSTRUCTIONS warns about this

---

## 10. Port Already in Use ✅

**Scenario:** Try to start client on a port that's already bound.

**Detection:**
- `bind()` system call fails with EADDRINUSE

**Handling:**
```c
if (bind(listen_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
    perror("Peer bind failed");
    close(listen_fd);
    return NULL;
}
```

**User Experience:**
```
$ ./client 127.0.0.1 9000
[... registration succeeds ...]
Peer bind failed: Address already in use
```

**Fix:**
- Use a different port: `./client 127.0.0.1 9002`
- Or kill previous client: `killall client`

---

## 11. Empty Message Handling ✅

**Scenario:** User presses Enter without typing anything in chat.

**Detection:**
- Client checks `strlen(input) == 0`

**Handling:**
```c
if (strlen(input) == 0) {
    continue;  // Skip empty input
}
```

**User Experience:**
```
>> [Enter]
>> [Nothing happens, prompt appears again]
```

---

## 12. Graceful Shutdown (Ctrl+C) ✅

**Scenario:** User presses Ctrl+C on server or client.

**Detection:**
- Signal handler catches SIGINT

**Handling:**

**Server:**
```c
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
```

**Client:**
```c
void signal_handler(int sig) {
    printf("\nExiting...\n");
    
    if (server_socket >= 0) {
        Message msg;
        msg.type = MSG_DISCONNECT;
        send_msg(server_socket, &msg);
        close(server_socket);
    }
    
    exit(0);
}
```

**User Experience:**
```
[Client presses Ctrl+C]
^C
Exiting...
[Program terminates cleanly]
```

**Other clients see:**
```
[SYSTEM] alice has left the chat
```

---

## 13. Maximum Clients Reached ✅

**Scenario:** 101st client tries to connect (MAX_CLIENTS = 100).

**Detection:**
- Server checks `client_count >= MAX_CLIENTS`

**Handling:**
```c
if (client_count >= MAX_CLIENTS) {
    printf("Maximum clients reached. Rejecting connection.\n");
    close(new_socket);
    pthread_mutex_unlock(&clients_mutex);
    continue;
}
```

**User Experience (client):**
```
[Connection established briefly, then closes]
[No registration happens]
```

**Server Log:**
```
Maximum clients reached. Rejecting connection.
```

---

## 14. Concurrent Chat Requests ✅

**Scenario:** Two users try to chat with each other simultaneously.

**Detection & Handling:**
- Both send MSG_CHAT_REQUEST to server
- Server processes them separately (different threads)
- Both get the other's IP:port
- Both try to connect to each other
- One succeeds as initiator, one as receiver
- Race condition resolved at TCP level (one connect succeeds first)

**Outcome:**
- One P2P connection established
- Both users in chat (doesn't matter who "won" the race)

---

## 15. Empty Field Serialization ✅

**Scenario:** Message with empty content field (e.g., registration).

**Problem:**
- Creates `||` (consecutive delimiters)
- Standard `strtok()` skips empty fields

**Solution:**
- Custom manual parsing that handles empty fields

**Handling:**
```c
void deserialize_message(char *buffer, Message *msg) {
    char *ptr = buffer;
    char *field_start = ptr;
    int field_num = 0;
    
    while (*ptr && field_num < 5) {
        if (*ptr == '|') {
            *ptr = '\0';
            // Process field (even if empty)
            switch(field_num) {
                case 0: msg->type = atoi(field_start); break;
                case 1: strcpy(msg->sender, field_start); break;
                case 2: strcpy(msg->receiver, field_start); break;
                case 3: strcpy(msg->content, field_start); break;
            }
            *ptr = '|';
            field_start = ptr + 1;
            field_num++;
        }
        ptr++;
    }
    msg->data = atoi(field_start);
}
```

**Result:**
- Correctly parses `1|alice|SERVER||9000`
- content = "" (empty string)
- data = 9000 ✅

---

## Summary Table

| Edge Case | Detection | Handling | User Feedback |
|-----------|-----------|----------|---------------|
| Duplicate Username | Username exists in active list | Reject registration | "Username already taken" |
| Self-Chat | sender == receiver | Reject request | "Cannot chat with yourself" |
| Client Crash (Chat) | recv() returns 0 | Close socket, exit thread | "[user disconnected]" |
| Client Crash (Server) | recv() returns 0 | Cleanup, notify others | "[user has left]" |
| User Busy | in_chat flag set | Auto-reject | "User busy" |
| User Offline | find_user returns -1 | Send error | "User not found" |
| Chat Rejected | MSG_CHAT_REJECT | Close connection | Connection fails |
| Invalid Username | Not in list | Send error | "User not found" |
| Extra Text in Command | Parse as username | Lookup fails | "User not found" |
| Port in Use | bind() fails | Print error | "Address in use" |
| Empty Input | strlen() == 0 | Skip | No action |
| Ctrl+C | SIGINT signal | Graceful shutdown | "Exiting..." |
| Max Clients | count >= MAX | Reject connection | Connection closes |
| Concurrent Requests | TCP race | First wins | Both end up in chat |
| Empty Fields | Custom parser | Handle correctly | Correct data parsing |

---

## Not Handled (Potential Future Enhancements)

1. **Network Partitions:** If network splits, no automatic reconnection
2. **Message Ordering:** No sequence numbers (relies on TCP ordering)
3. **Duplicate Messages:** No deduplication mechanism
4. **Session Persistence:** No reconnect with session recovery
5. **NAT Traversal:** Assumes all clients on same network or routable IPs
6. **Buffer Overflow:** Limited bounds checking (assumes well-behaved clients)
7. **Malformed Messages:** Assumes correct protocol format
8. **Resource Exhaustion:** No per-user limits on message rate/size
9. **Authentication:** No verification of user identity
10. **Encryption:** No protection against eavesdropping

---

## Testing Each Edge Case

See `TEST_INSTRUCTIONS.md` for detailed test procedures for:
- Test 6: Duplicate Username
- Test 7: Chat with Yourself

Other edge cases are tested implicitly through normal usage or by simulating failures (disconnecting clients, pressing Ctrl+C, etc.).

---

**Last Updated:** October 20, 2025
