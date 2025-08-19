#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>
#include <sstream>
#include <fstream>
#include <filesystem>

const int MAIN_SERVER_PORT = 4444;
const int FILE_PORT = 5555;
const int BUFFER_SIZE = 4096;
const char* SERVER_IP = "192.168.2.3";
const int UDP_TIMEOUT_MS = 2000;

using namespace std;
namespace fs = std::filesystem;


void send_udp(int sock, const string& data, sockaddr_in& dest_addr) {
    sendto(sock, data.c_str(), data.size(), 0,
          (sockaddr*)&dest_addr, sizeof(dest_addr));
}

string receive_udp(int sock, sockaddr_in& from_addr) {
    char buffer[BUFFER_SIZE];
    socklen_t from_len = sizeof(from_addr);
    
    struct timeval tv {UDP_TIMEOUT_MS/1000, (UDP_TIMEOUT_MS%1000)*1000};
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv));
    
    int bytes = recvfrom(sock, buffer, BUFFER_SIZE, 0,
                       (sockaddr*)&from_addr, &from_len);
    if(bytes <= 0) return "";
    return string(buffer, bytes);
}

void update_server(const string& filename) {
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if(sock < 0) return;

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(MAIN_SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    string request = "UPDATE " + filename;
    send_udp(sock, request, server_addr);

    // Wait for confirmation
    sockaddr_in response_addr{};
    string confirmation = receive_udp(sock, response_addr);
    
    close(sock);
}

vector<string> get_shared_files() {
    vector<string> files;
    try {
        for(const auto& entry : fs::directory_iterator(fs::current_path())) {
            if(entry.is_regular_file()) {
                files.push_back(entry.path().filename().string());
            }
        }
    } catch(const fs::filesystem_error& e) {
        cerr << "Directory error: " << e.what() << endl;
    }
    return files;
}

const string SIZE_PREFIX = "SIZE:";
const string ACK_MSG = "ACK";
const int MAX_RETRIES = 5;

void download_file(const string& filename, const string& peer_ip) {
    cout << "Starting download of " << filename << " from " << peer_ip << endl;

    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in peer_addr{};
    peer_addr.sin_family = AF_INET;
    peer_addr.sin_port = htons(FILE_PORT);
    inet_pton(AF_INET, peer_ip.c_str(), &peer_addr.sin_addr);

    // Send file request
    sendto(sock, filename.c_str(), filename.size(), 0, (sockaddr*)&peer_addr, sizeof(peer_addr));
    
    // Receive size header
    sockaddr_in sender_addr{};
    string response = receive_udp(sock, sender_addr);
    
    if (response.find("SIZE:") != 0) {
        cerr << "Invalid size header: " << response << endl;
        close(sock);
        return;
    }
    
    streamsize expected_size = stoll(response.substr(5));

    ofstream file(filename, ios::binary);
    int total_received = 0;

    while (total_received < expected_size) {
        string data = receive_udp(sock, sender_addr);

        if (data.empty()) {
            // Request retransmission
            sendto(sock, ("RESEND:" + to_string(total_received)).c_str(), BUFFER_SIZE,
                   0, (sockaddr*)&peer_addr, sizeof(peer_addr));
            continue;
        }

        if (data.find("EOF:") == 0) {
            streamsize received_size = stoll(data.substr(4));
            break;
        }

        file.write(data.data(), data.size());
        total_received += data.size();
        
        if (total_received > expected_size) {
            break;
        }
    }

    file.close();
}

void handle_file_request(int server_fd, sockaddr_in client_addr, const string& filename) {
    string client_ip = inet_ntoa(client_addr.sin_addr);
    cout << "Starting transfer of " << filename << " to " << client_ip << endl;

    ifstream file(filename, ios::binary | ios::ate);
    if (!file) {
        cerr << "File not found: " << filename << endl;
        sendto(server_fd, "FILE_NOT_FOUND", 14, 0, (sockaddr*)&client_addr, sizeof(client_addr));
        return;
    }

    streamsize file_size = file.tellg();
    file.seekg(0, ios::beg);

    // Send size header
    string size_header = "SIZE:" + to_string(file_size);
    sendto(server_fd, size_header.c_str(), size_header.size(), 0, (sockaddr*)&client_addr, sizeof(client_addr));

    char buffer[BUFFER_SIZE];
    int total_sent = 0;
    socklen_t addr_len = sizeof(client_addr);

    while (file.read(buffer, BUFFER_SIZE)) {
        int chunk_size = file.gcount();
        sendto(server_fd, buffer, chunk_size, 0, (sockaddr*)&client_addr, addr_len);
        total_sent += chunk_size;
    }

    if (file.gcount() > 0) {
        int chunk_size = file.gcount();
        sendto(server_fd, buffer, chunk_size, 0, (sockaddr*)&client_addr, addr_len);
        total_sent += chunk_size;
    }

    // Send EOF marker
    string eof_msg = "EOF:" + to_string(file_size);
    sendto(server_fd, eof_msg.c_str(), eof_msg.size(), 0, (sockaddr*)&client_addr, addr_len);
}

void start_file_server() {
    int server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if(server_fd < 0) {
        cerr << "Failed to create UDP socket" << endl;
        return;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(FILE_PORT);

    if(bind(server_fd, (sockaddr*)&addr, sizeof(addr)) < 0) {
        cerr << "Bind failed" << endl;
        close(server_fd);
        return;
    }

    cout << "UDP File Server listening on port " << FILE_PORT << endl;

    while(true) {
        char buffer[BUFFER_SIZE];
        sockaddr_in client_addr{};
        socklen_t addr_len = sizeof(client_addr);

        int bytes = recvfrom(server_fd, buffer, BUFFER_SIZE, 0,
                            (sockaddr*)&client_addr, &addr_len);
        if(bytes > 0) {
            string message(buffer, bytes);

            // Check if it's a protocol message 
            if(message == ACK_MSG) {
                continue;  // Ignore ACK messages
            }

            // Otherwise, treat it as a file request
            thread([server_fd, client_addr, message]() {
                handle_file_request(server_fd, client_addr, message);
            }).detach();
        }
    }
    close(server_fd);
}
int main() {
    thread file_server(start_file_server);
    file_server.detach();

    // Auto-register files
    auto shared_files = get_shared_files();
    if(!shared_files.empty()) {
        int reg_sock = socket(AF_INET, SOCK_DGRAM, 0);
        if(reg_sock >= 0) {
            sockaddr_in server_addr{};
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(MAIN_SERVER_PORT);
            inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

            ostringstream oss;
            for(const auto& f : shared_files) oss << f << ",";
            string file_list = oss.str();
            if(!file_list.empty()) file_list.pop_back();

            send_udp(reg_sock, file_list, server_addr);
            cout << "Registered " << shared_files.size() << " files\n";
            close(reg_sock);
        }
    }

    // Command interface
    while(true) {
        cout << "\nEnter command (Send file1,file2 or Exit): " << endl ; 
        string input;
        getline(cin, input);

        if(input == "Exit") break;

        if(input.find("Send") == 0) {
            string files = input.substr(5);
            int sock = socket(AF_INET, SOCK_DGRAM, 0);
            if(sock < 0) continue;

            sockaddr_in server_addr{};
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(MAIN_SERVER_PORT);
            inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

            string request = "Send " + files;
            send_udp(sock, request, server_addr);

            sockaddr_in response_addr{};
            string response = receive_udp(sock, response_addr);
            if(!response.empty()) {
                istringstream iss(response);
                string line;
                while(getline(iss, line)) {
                    size_t colon = line.find(':');
                    if(colon != string::npos) {
                        thread(download_file,
                              line.substr(0, colon),
                              line.substr(colon+1)).detach();
                    }
                }
            }
            close(sock);
        }
    }
    return 0;
}
