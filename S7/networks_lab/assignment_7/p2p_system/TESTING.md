# P2P File Transfer System - Complete Testing Guide

## 🧪 Full Testing Scenario with Multiple Peers

This guide provides step-by-step instructions to test the P2P file transfer system with 3 peers, demonstrating concurrent uploads and downloads.

---

## 📋 Prerequisites

```bash
cd /mnt/e/College_Assignments/S7/networks_lab/assignment_7/p2p_system

# Build the system
make clean
make all

# Verify executables exist
ls -l server peer
```

**Expected output:**
```
-rwxrwxrwx 1 user user 17K server
-rwxrwxrwx 1 user user 26K peer
```

---

## 🔧 Setup Phase

### Step 1: Create Working Directories for Each Peer

```bash
# Create separate directories for 3 peers
mkdir -p peer1/shared peer1/downloads
mkdir -p peer2/shared peer2/downloads
mkdir -p peer3/shared peer3/downloads
```

### Step 2: Create Unique Test Files for Each Peer

```bash
# Peer 1 files
echo "This is File A from Peer 1" > peer1/shared/fileA.txt
echo "This is File B from Peer 1" > peer1/shared/fileB.txt
dd if=/dev/urandom of=peer1/shared/binary1.dat bs=1024 count=10 2>/dev/null

# Peer 2 files
echo "This is File C from Peer 2" > peer2/shared/fileC.txt
echo "This is File D from Peer 2" > peer2/shared/fileD.txt
dd if=/dev/urandom of=peer2/shared/binary2.dat bs=1024 count=10 2>/dev/null

# Peer 3 files
echo "This is File E from Peer 3" > peer3/shared/fileE.txt
echo "This is File F from Peer 3" > peer3/shared/fileF.txt
dd if=/dev/urandom of=peer3/shared/binary3.dat bs=1024 count=10 2>/dev/null
```

### Step 3: Verify Files Created

```bash
echo "=== Peer 1 Files ==="
ls -lh peer1/shared/

echo "=== Peer 2 Files ==="
ls -lh peer2/shared/

echo "=== Peer 3 Files ==="
ls -lh peer3/shared/
```

**Expected output:**
```
=== Peer 1 Files ===
-rw-r--r-- 1 user user   27 fileA.txt
-rw-r--r-- 1 user user   27 fileB.txt
-rw-r--r-- 1 user user  10K binary1.dat

=== Peer 2 Files ===
-rw-r--r-- 1 user user   27 fileC.txt
-rw-r--r-- 1 user user   27 fileD.txt
-rw-r--r-- 1 user user  10K binary2.dat

=== Peer 3 Files ===
-rw-r--r-- 1 user user   27 fileE.txt
-rw-r--r-- 1 user user   27 fileF.txt
-rw-r--r-- 1 user user  10K binary3.dat
```

---

## 🚀 Testing Scenario

### TERMINAL 1: Start Directory Server

```bash
cd /mnt/e/College_Assignments/S7/networks_lab/assignment_7/p2p_system
./server
```

**Expected output:**
```
Directory Server listening on port 8080
```

**✓ Verification:**
```bash
# In another terminal, check if server is running
pgrep -f "p2p_system/server"
lsof -i :8080
```

**Expected:** Process ID should be shown, port 8080 should be LISTENING.

---

### TERMINAL 2: Start Peer 1 (Port 3300)

```bash
cd /mnt/e/College_Assignments/S7/networks_lab/assignment_7/p2p_system/peer1
../peer 3300
```

**Expected output:**
```
=== P2P File Transfer - Peer Application ===
My Port: 3300
Server: localhost:8080
Peer server listening on port 3300

=== MENU ===
1. Login (Go Online)
2. Search for a File
3. Exit
Enter choice:
```

**Action:** Enter `1` (Login)

**Expected output:**
```
Successfully logged in! Shared 3 files.
  - fileA.txt
  - fileB.txt
  - binary1.dat
```

**✓ Verification (Terminal 1 - Server):**
```
Peer on port 3300 is online with 3 files
```

**✓ Verification Command:**
```bash
# In another terminal
lsof -i :3300
```

---

### TERMINAL 3: Start Peer 2 (Port 3301)

```bash
cd /mnt/e/College_Assignments/S7/networks_lab/assignment_7/p2p_system/peer2
../peer 3301
```

**Action:** Enter `1` (Login)

**Expected output:**
```
Successfully logged in! Shared 3 files.
  - fileC.txt
  - fileD.txt
  - binary2.dat
```

**✓ Verification (Terminal 1 - Server):**
```
Peer on port 3301 is online with 3 files
```

---

### TERMINAL 4: Start Peer 3 (Port 3302)

```bash
cd /mnt/e/College_Assignments/S7/networks_lab/assignment_7/p2p_system/peer3
../peer 3302
```

**Action:** Enter `1` (Login)

**Expected output:**
```
Successfully logged in! Shared 3 files.
  - fileE.txt
  - fileF.txt
  - binary3.dat
```

**✓ Verification (Terminal 1 - Server):**
```
Peer on port 3302 is online with 3 files
```

---

## 📥 Test 1: Simple File Search and Download

### Peer 2 searches for file from Peer 1

**In TERMINAL 3 (Peer 2):**

```
Enter choice: 2
Enter filename to search: fileA.txt
```

**Expected output:**
```
File 'fileA.txt' found on 1 peer(s):
  1. Peer on port 3300
Enter peer number to download from (0 to cancel): 1
```

**Action:** Enter `1`

**Expected output:**
```
Downloading file (size: 27 bytes)...
File downloaded successfully to ./downloads/fileA.txt
Total bytes received: 27
```

**✓ Verification:**
```bash
# In another terminal
ls -lh peer2/downloads/
cat peer2/downloads/fileA.txt
```

**Expected:**
```
-rw-r--r-- 1 user user 27 fileA.txt
This is File A from Peer 1
```

**📌 IMPORTANT - Auto-sharing feature:**
The downloaded file is **automatically copied to peer2/shared/** so it can be shared with other peers!

```bash
# Check that file is also in shared folder
ls -lh peer2/shared/
```

You should now see `fileA.txt` in peer2's shared folder.

**✓ Verification (Terminal 2 - Peer 1):**
Should show:
```
File request: fileA.txt from 127.0.0.1
Sent fileA.txt to 127.0.0.1
```

---

## 🔄 Test 2: File Available on Multiple Peers

### Copy a file to multiple peers

```bash
# In another terminal
cp peer1/shared/fileB.txt peer3/shared/fileB.txt
```

### Have Peer 3 re-login to register new file

**In TERMINAL 4 (Peer 3):**
```
Enter choice: 3   (Exit)
```

Then restart:
```bash
cd /mnt/e/College_Assignments/S7/networks_lab/assignment_7/p2p_system/peer3
../peer 3302
Enter choice: 1   (Login)
```

### Peer 2 searches for file now available on multiple peers

**In TERMINAL 3 (Peer 2):**
```
Enter choice: 2
Enter filename to search: fileB.txt
```

**Expected output:**
```
File 'fileB.txt' found on 2 peer(s):
  1. Peer on port 3300
  2. Peer on port 3302
Enter peer number to download from (0 to cancel):
```

**Action:** Enter `2` (Download from Peer 3)

**✓ Verification:**
```bash
ls -lh peer2/downloads/fileB.txt
cat peer2/downloads/fileB.txt
```

---

## 🔄 Test 2b: Auto-Sharing Downloaded Files

**This feature automatically makes downloaded files available for sharing!**

When Peer 2 downloaded `fileA.txt` from Peer 1 (in Test 1), it was automatically added to Peer 2's shared folder.

### Verify Peer 2 can now share fileA.txt

**Step 1: Peer 2 needs to re-login to register the new file**

**In TERMINAL 3 (Peer 2):**
```
Enter choice: 3   (Exit)
```

Then restart and login:
```bash
cd /mnt/e/College_Assignments/S7/networks_lab/assignment_7/p2p_system/peer2
../peer 3301
Enter choice: 1   (Login)
```

You should now see `fileA.txt` in the file list!

**Step 2: Peer 3 searches for fileA.txt**

**In TERMINAL 4 (Peer 3):**
```
Enter choice: 2
Enter filename to search: fileA.txt
```

**Expected output:**
```
File 'fileA.txt' found on 2 peer(s):
  1. Peer on port 3300  ← Original source
  2. Peer on port 3301  ← Downloaded and now sharing!
Enter peer number to download from (0 to cancel):
```

**Action:** Enter `2` (Download from Peer 2, not the original Peer 1!)

**✓ Verification:**
```bash
ls -lh peer3/downloads/fileA.txt
cat peer3/downloads/fileA.txt
```

This proves the file successfully propagated from Peer 1 → Peer 2 → Peer 3! 🎉

---

## ⚡ Test 3: Concurrent Upload and Download

This demonstrates that a peer can upload to one peer while downloading from another.

### Setup: Peer 1 will download from Peer 3 while Peer 2 downloads from Peer 1

**In TERMINAL 2 (Peer 1) - Start Download:**
```
Enter choice: 2
Enter filename to search: fileE.txt
```

Select peer 3302 to download from.

**IMMEDIATELY - In TERMINAL 3 (Peer 2) - Start Another Download:**
```
Enter choice: 2
Enter filename to search: binary1.dat
```

Select peer 3300 to download from.

**✓ Verification:**

Both downloads should complete successfully, proving Peer 1 can:
- Upload `binary1.dat` to Peer 2
- Download `fileE.txt` from Peer 3
- **AT THE SAME TIME** (concurrent operations)

Check downloads:
```bash
# Peer 1 should have fileE.txt
ls -lh peer1/downloads/fileE.txt

# Peer 2 should have binary1.dat
ls -lh peer2/downloads/binary1.dat
```

---

## 🔍 Test 4: File Not Found

**In TERMINAL 3 (Peer 2):**
```
Enter choice: 2
Enter filename to search: nonexistent.txt
```

**Expected output:**
```
File 'nonexistent.txt' not found on any peer
```

**✓ Verification (Terminal 1 - Server):**
```
Sent 0 peer(s) with file 'nonexistent.txt'
```

---

## 📊 Test 5: Binary File Transfer

**In TERMINAL 4 (Peer 3):**
```
Enter choice: 2
Enter filename to search: binary2.dat
```

Select peer 3301 to download.

**✓ Verification:**
```bash
# Check file size matches
ls -lh peer2/shared/binary2.dat
ls -lh peer3/downloads/binary2.dat

# Verify binary integrity
md5sum peer2/shared/binary2.dat
md5sum peer3/downloads/binary2.dat
```

**Expected:** Both checksums should match exactly.

---

## 🧹 Verification Commands Summary

### Check All Running Processes
```bash
ps aux | grep -E "(server|peer)" | grep -v grep
```

**Expected:** 4 processes (1 server + 3 peers)

### Check All Listening Ports
```bash
lsof -i :8080  # Server
lsof -i :3300  # Peer 1
lsof -i :3301  # Peer 2
lsof -i :3302  # Peer 3
```

### Check Network Connections
```bash
netstat -tuln | grep -E "8080|3300|3301|3302"
```

**Expected output:**
```
tcp   0   0 0.0.0.0:8080   0.0.0.0:*   LISTEN
tcp   0   0 0.0.0.0:3300   0.0.0.0:*   LISTEN
tcp   0   0 0.0.0.0:3301   0.0.0.0:*   LISTEN
tcp   0   0 0.0.0.0:3302   0.0.0.0:*   LISTEN
```

### Verify Downloaded Files
```bash
# Peer 1 downloads
ls -lh peer1/downloads/
find peer1/downloads -type f -exec md5sum {} \;

# Peer 2 downloads
ls -lh peer2/downloads/
find peer2/downloads -type f -exec md5sum {} \;

# Peer 3 downloads
ls -lh peer3/downloads/
find peer3/downloads -type f -exec md5sum {} \;
```

### Compare File Integrity
```bash
# Example: Verify fileA.txt downloaded by Peer 2 matches original
diff peer1/shared/fileA.txt peer2/downloads/fileA.txt && echo "✓ Files match"

# Example: Verify binary file integrity
cmp peer2/shared/binary2.dat peer3/downloads/binary2.dat && echo "✓ Binary files match"
```

---

## 📝 Expected Results Summary

| Test | Source Peer | Target Peer | File | Status |
|------|-------------|-------------|------|--------|
| 1 | Peer 1 (3300) | Peer 2 (3301) | fileA.txt | ✓ Downloaded |
| 2 | Peer 3 (3302) | Peer 2 (3301) | fileB.txt | ✓ Multiple sources |
| 3a | Peer 3 (3302) | Peer 1 (3300) | fileE.txt | ✓ Concurrent download |
| 3b | Peer 1 (3300) | Peer 2 (3301) | binary1.dat | ✓ Concurrent upload |
| 4 | N/A | Peer 2 (3301) | nonexistent.txt | ✓ Not found |
| 5 | Peer 2 (3301) | Peer 3 (3302) | binary2.dat | ✓ Binary transfer |

---

## 🛑 Cleanup After Testing

```bash
# Kill all peers and server
pkill -f "p2p_system/server"
pkill -f "p2p_system/peer"

# Verify all stopped
pgrep -f "p2p_system" || echo "✓ All processes stopped"
```

### Option 1: Full cleanup (reset to original state)
```bash
cd /mnt/e/College_Assignments/S7/networks_lab/assignment_7/p2p_system
make clean-all
```

This will:
- Remove executables (server, peer)
- Clear all downloaded files from peer*/downloads/
- Remove extra files from peer*/shared/ (keeps only originals)

### Option 2: Remove just downloads
```bash
make clean-downloads
```

### Option 3: Remove extra shared files (keep originals)
```bash
make clean-shared
```

**Original files that are preserved:**
- Peer 1: fileA.txt, fileB.txt, binary1.dat
- Peer 2: fileC.txt, fileD.txt, binary2.dat
- Peer 3: fileE.txt, fileF.txt, binary3.dat

### Option 4: Complete removal (including test directories)
```bash
# Remove test directories (optional)
cd /mnt/e/College_Assignments/S7/networks_lab/assignment_7/p2p_system
rm -rf peer1 peer2 peer3
```

---

## 📊 Performance Testing (Optional)

### Test with larger files

```bash
# Create larger test files (1MB, 10MB)
dd if=/dev/urandom of=peer1/shared/large1MB.dat bs=1M count=1
dd if=/dev/urandom of=peer2/shared/large10MB.dat bs=1M count=10

# Time the download
time (echo -e "2\nlarge1MB.dat\n1" | ../peer 3303)
```

### Stress test with many files

```bash
# Create 50 test files in Peer 1
for i in {1..50}; do
    echo "Test file $i" > peer1/shared/test$i.txt
done

# Login and verify all 50 files registered
```

---

## ✅ Success Criteria

Your P2P system passes all tests if:

1. ✓ Directory server accepts connections from all 3 peers
2. ✓ All peers successfully register their files (Packet Type 1)
3. ✓ File searches return correct peer list (Packet Type 2 & 3)
4. ✓ Files download successfully with correct content
5. ✓ Binary files transfer without corruption (matching checksums)
6. ✓ Multiple peers can have same file (reported correctly)
7. ✓ Concurrent upload/download works simultaneously
8. ✓ File not found handled gracefully
9. ✓ **Downloaded files automatically appear in shared folder**
10. ✓ **Re-login updates file list with newly downloaded files**
11. ✓ All processes run without crashes
12. ✓ No memory leaks or resource issues

---

## 🐛 Troubleshooting

### Port already in use
```bash
lsof -i :8080
kill -9 <PID>
```

### Peer can't connect to server
```bash
# Check server is running
pgrep -f server

# Check firewall
sudo ufw status
```

### File not downloading
```bash
# Check peer registered files
# Check file exists in shared/ directory
ls -la peer1/shared/

# Check permissions
chmod 644 peer1/shared/*
```

---

**Testing completed! Your P2P file transfer system is fully functional.** 🎉
