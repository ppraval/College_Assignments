# Makefile for P2P File Transfer System
# Networks Lab - Assignment 7

CC = gcc
CFLAGS = -Wall -Wextra -pthread -O2

# Executables
SERVER = server
PEER = peer

# Source files
SERVER_SRC = server.c
PEER_SRC = peer.c

# Default target
all: $(SERVER) $(PEER)
	@echo "======================================"
	@echo "✓ P2P system built successfully!"
	@echo "======================================"
	@echo "Run './server' to start directory server"
	@echo "Run './peer <PORT>' to start a peer"
	@echo ""

# Build directory server
$(SERVER): $(SERVER_SRC)
	$(CC) $(CFLAGS) -o $(SERVER) $(SERVER_SRC)
	@echo "✓ Built: $(SERVER)"

# Build peer application
$(PEER): $(PEER_SRC)
	$(CC) $(CFLAGS) -o $(PEER) $(PEER_SRC)
	@echo "✓ Built: $(PEER)"

# Setup directories
setup:
	@mkdir -p shared downloads
	@echo "✓ Created shared/ and downloads/ directories"
	@echo "Add files to shared/ directory to share them"

# Create test files
test-files:
	@mkdir -p shared
	@echo "This is test file 1" > shared/test1.txt
	@echo "This is test file 2" > shared/test2.txt
	@echo "Sample document content" > shared/document.txt
	@echo "✓ Created 3 test files in shared/"
	@ls -lh shared/

# Clean executables
clean:
	rm -f $(SERVER) $(PEER)
	@echo "✓ Cleaned executables"

# Deep clean (including directories)
distclean: clean
	rm -rf downloads shared
	@echo "✓ Removed all files and directories"

# Clean downloaded files only (keep original test files)
clean-downloads:
	@echo "Cleaning downloaded files..."
	@find peer1/downloads -type f -delete 2>/dev/null || true
	@find peer2/downloads -type f -delete 2>/dev/null || true
	@find peer3/downloads -type f -delete 2>/dev/null || true
	@echo "✓ Cleaned downloads folders"

# Clean extra files from shared folders (keep only original files)
clean-shared:
	@echo "Cleaning extra files from shared folders..."
	@# Keep original Peer 1 files: fileA.txt, fileB.txt, binary1.dat
	@cd peer1/shared && ls | grep -v -E "^(fileA\.txt|fileB\.txt|binary1\.dat)$$" | xargs rm -f 2>/dev/null || true
	@# Keep original Peer 2 files: fileC.txt, fileD.txt, binary2.dat
	@cd peer2/shared && ls | grep -v -E "^(fileC\.txt|fileD\.txt|binary2\.dat)$$" | xargs rm -f 2>/dev/null || true
	@# Keep original Peer 3 files: fileE.txt, fileF.txt, binary3.dat
	@cd peer3/shared && ls | grep -v -E "^(fileE\.txt|fileF\.txt|binary3\.dat)$$" | xargs rm -f 2>/dev/null || true
	@echo "✓ Cleaned extra files from shared folders"

# Full cleanup - reset to original state
clean-all: clean clean-downloads clean-shared
	@echo "✓ Full cleanup complete - system reset to original state"

# Rebuild everything
rebuild: clean all

# Help
help:
	@echo "P2P File Transfer System - Makefile"
	@echo ""
	@echo "Available targets:"
	@echo "  all            - Build server and peer (default)"
	@echo "  server         - Build directory server only"
	@echo "  peer           - Build peer application only"
	@echo "  setup          - Create shared/ and downloads/ directories"
	@echo "  test-files     - Create sample test files"
	@echo "  clean          - Remove executables"
	@echo "  clean-downloads - Remove all downloaded files"
	@echo "  clean-shared   - Remove extra files from shared/ (keep originals)"
	@echo "  clean-all      - Full cleanup (executables + downloads + shared extras)"
	@echo "  distclean      - Remove everything (including directories)"
	@echo "  rebuild        - Clean and rebuild"
	@echo "  help           - Show this help message"
	@echo ""
	@echo "Example usage:"
	@echo "  make                    # Build everything"
	@echo "  make setup              # Create directories"
	@echo "  make test-files         # Create test files"
	@echo "  make clean-all          # Reset to original state"
	@echo "  ./server                # Start server"
	@echo "  ./peer 3300             # Start peer on port 3300"

.PHONY: all setup test-files clean distclean rebuild help clean-downloads clean-shared clean-all
