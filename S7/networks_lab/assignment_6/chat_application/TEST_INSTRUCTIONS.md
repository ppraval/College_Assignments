# Testing Instructions

## Quick Test Setup

### Terminal 1 - Start Server
```bash
cd /home/iamthope/nw_lab/chat_application
./server
```

**Expected Output:**
```
===========================================
Chat Server started on port 8888
Waiting for client connections...
===========================================
```

---

### Terminal 2 - Start Client A (iamthope)
```bash
cd /home/iamthope/nw_lab/chat_application
./client 127.0.0.1 9000
```

When prompted:
```
Enter your username: iamthope
```

**Expected Output:**
```
Listening for peer connections on port 9000
Registration successful
```

**Server should show:**
```
New connection from 127.0.0.1:XXXXX (socket: 4)
Client handler started for socket 4
User registered: iamthope (peer port: 9000)
```

---

### Terminal 3 - Start Client B (gmr)
```bash
cd /home/iamthope/nw_lab/chat_application
./client 127.0.0.1 9001
```

When prompted:
```
Enter your username: gmr
```

**Expected Output:**
```
Listening for peer connections on port 9001
Registration successful
```

**Server should show:**
```
New connection from 127.0.0.1:XXXXX (socket: 5)
Client handler started for socket 5
User registered: gmr (peer port: 9001)
```

**Client A (iamthope) should show:**
```
[SYSTEM] gmr has joined the chat
```

---

## Test 1: List Active Users

In **Terminal 2** (iamthope), type:
```
list
```

**Expected Output:**
```
Requesting user list...

===========================================
Active Users:
===========================================
1. gmr
===========================================
```

---

## Test 2: Initiate Chat

In **Terminal 2** (iamthope), type:
```
chat gmr
```

**Expected Output in Terminal 2:**
```
Request sent via server. Waiting for gmr to accept...

[SERVER] Peer info: 127.0.0.1:9001
>> Connecting to peer...
```

**Server should show:**
```
Chat request from iamthope to gmr (target peer port: 9001)
```

**Expected Output in Terminal 3 (gmr):**
```
===========================================
Incoming chat request from: iamthope
Accept? (yes/no):
```

---

## Test 3: Accept Chat

In **Terminal 3** (gmr), type:
```
yes
```

**Expected Output in Terminal 3:**
```
Chat started with iamthope. Type 'exit' to end chat.
===========================================
>>
```

**Expected Output in Terminal 2:**
```
Chat started with gmr. Type 'exit' to end chat.
>>
```

---

## Test 4: Send Messages

In **Terminal 2** (iamthope), type:
```
Hello gmr!
```

**Terminal 3 (gmr) should show:**
```
[iamthope]: Hello gmr!
>>
```

In **Terminal 3** (gmr), type:
```
Hi iamthope!
```

**Terminal 2 (iamthope) should show:**
```
[gmr]: Hi iamthope!
>>
```

---

## Test 5: End Chat

In **Terminal 2** (iamthope), type:
```
exit
```

**Expected Output in Terminal 2:**
```
Chat ended.
>>
```

**Expected Output in Terminal 3:**
```
[iamthope has ended the chat]
>>
```

---

## Test 6: Edge Case - Duplicate Username

**Scenario:** Try to connect with a username that's already taken.

In **Terminal 4** (new terminal), type:
```bash
./client 127.0.0.1 9002
```

When prompted:
```
Enter your username: iamthope
```

**Expected Output:**
```
[ERROR] Username already taken. Please disconnect and try a different username.
```

**Connection will close automatically.**

**Server should show:**
```
New connection from 127.0.0.1:XXXXX (socket: 6)
Client handler started for socket 6
Registration rejected: iamthope (username already taken)
Client handler terminated for
```

---

## Test 7: Edge Case - Chat with Yourself

In **Terminal 2** (iamthope), type:
```
chat iamthope
```

**Expected Output:**
```
Request sent via server. Waiting for iamthope to accept...
[ERROR] Cannot chat with yourself
>>
```

**Server should show:**
```
Chat request rejected: iamthope tried to chat with themselves
```

---

## Common Issues and Fixes

### Issue 1: "Peer info: 127.0.0.1:0"
**Cause:** Client didn't send peer port correctly or server didn't store it.

**Check Server Output:** Look for `User registered: USERNAME (peer port: XXXX)`
- If it shows `(peer port: 0)`, the client isn't sending the port correctly
- If it shows the correct port but chat still fails, check firewall/permissions

**Fix:** 
1. Make sure you're starting clients with: `./client 127.0.0.1 <unique_port>`
2. Each client needs a different port (9000, 9001, 9002, etc.)
3. Rebuild: `make clean && make all`

### Issue 2: "Connection to peer failed: Connection refused"
**Causes:**
1. Peer listener thread hasn't started yet (wait 1-2 seconds after registration)
2. Peer is using port 0 (see Issue 1)
3. Firewall blocking the port
4. Peer already in another chat

**Fix:**
- Wait a moment after both clients have registered
- Verify server shows correct peer ports in registration messages
- Check that peer listener shows "Listening for peer connections on port XXXX"

### Issue 3: "User not found or offline"
**Causes:**
1. Typo in username
2. Extra text after username (e.g., `chat bob hi` instead of `chat bob`)
3. User disconnected

**Fix:**
- Use `list` to see exact usernames
- Use only `chat <username>` with no extra text
- Username is case-sensitive

### Issue 4: "Username already taken"
**Cause:** Another client is already using that username

**Fix:**
- Choose a different username
- If you previously disconnected, wait a moment (server may still be cleaning up)
- Check `list` on another client to see active users

### Issue 5: "Cannot chat with yourself"
**Cause:** Tried to chat with your own username

**Fix:**
- Use `list` to see other users
- Chat with a different user

---

## Debugging Commands

If issues persist, check these:

1. **See what the server received:**
   - Look at server terminal for registration messages showing peer ports

2. **Verify client is listening:**
   - Check for "Listening for peer connections on port XXXX" message

3. **Test with netstat:**
   ```bash
   netstat -an | grep 9000
   netstat -an | grep 9001
   ```
   Should show LISTEN state for peer ports

4. **Rebuild everything:**
   ```bash
   make clean
   make all
   ```

---

## Success Checklist

- ✅ Server shows: `User registered: USERNAME (peer port: XXXX)` with non-zero port
- ✅ Client shows: `Listening for peer connections on port XXXX`
- ✅ `list` command shows other users
- ✅ `chat <username>` shows `[SERVER] Peer info: 127.0.0.1:XXXX` with non-zero port
- ✅ Target receives chat request prompt
- ✅ After accepting, both can exchange messages
- ✅ `exit` cleanly ends the chat

---

## Quick Commands Reference

```bash
# Start server
./server

# Start client with peer port 9000
./client 127.0.0.1 9000

# Client commands:
list              # Show active users
chat <username>   # Start chat (username only!)
<any text>        # Send message (when in chat)
exit              # End chat or quit
```
