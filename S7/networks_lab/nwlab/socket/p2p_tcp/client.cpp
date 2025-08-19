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

using namespace std ;
namespace fs = std::filesystem;

void update_server(const string& filename) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return;

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(MAIN_SERVER_PORT);
    inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) >= 0) {
        string request = "UPDATE " + filename;
        send(sock, request.c_str(), request.size(), 0);
        
        char response[8];
        recv(sock, response, sizeof(response), 0); // Wait for confirmation
    }
    close(sock);
}

vector<string> get_shared_files() {
    vector<string> files;
    try {
        for (const auto& entry : fs::directory_iterator(fs::current_path())) {
            if (entry.is_regular_file()) {
                files.push_back(entry.path().filename().string());
            }
        }
    } catch (const fs::filesystem_error& e) {
        cerr << "Directory error: " << e.what() << endl;
    }
    return files;
}

void download_file(const string& filename, const string& peer_ip) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        cerr << "Socket error for " << filename << endl;
        return;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(FILE_PORT);
    inet_pton(AF_INET, peer_ip.c_str(), &server_addr.sin_addr);

    if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "Failed to connect to " << peer_ip << endl;
        close(sock);
        return;
    }

    send(sock, filename.c_str(), filename.size(), 0);
    
    ofstream file(filename, ios::binary);
    char buffer[BUFFER_SIZE];
    int bytes_received;

    while ((bytes_received = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        file.write(buffer, bytes_received);
    }

    file.close();
    close(sock);
    cout << "Downloaded " << filename << " from " << peer_ip << "\n" << endl;
    
    update_server(filename);
}

void handle_file_request(int client_socket) {
    char filename_buffer[BUFFER_SIZE];
    
    // Get client IP address
    sockaddr_in client_addr;
    socklen_t len = sizeof(client_addr);
    getpeername(client_socket, (sockaddr*)&client_addr, &len);
    string client_ip = inet_ntoa(client_addr.sin_addr);

    int bytes_read = recv(client_socket, filename_buffer, BUFFER_SIZE, 0);

    if (bytes_read > 0) {
        string filename(filename_buffer, bytes_read);
        cout << "File request: " << filename 
                  << " from " << client_ip << endl;

        ifstream file(filename, ios::binary);

        if (file) {
            char buffer[BUFFER_SIZE];
            while (file.read(buffer, BUFFER_SIZE)) {
                send(client_socket, buffer, file.gcount(), 0);
            }
            if (file.gcount() > 0) {
                send(client_socket, buffer, file.gcount(), 0);
            }
            cout << "Sent " << filename 
                      << " to " << client_ip << endl;
        } else {
            send(client_socket, "FILE_NOT_FOUND", 14, 0);
            cerr << "File not found: " << filename
                      << " for " << client_ip << endl;
        }
    }
    close(client_socket);
}


void start_file_server() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(FILE_PORT);

    bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);
    
    while (true) {
        sockaddr_in client_addr{};
        socklen_t addrlen = sizeof(client_addr);
        int client_socket = accept(server_fd, (sockaddr*)&client_addr, &addrlen);
        thread(handle_file_request, client_socket).detach();
    }
}

int main() {
    // Start file server
    thread server_thread(start_file_server);
    server_thread.detach();

    // Automatically register files
    auto shared_files = get_shared_files();
    if (!shared_files.empty()) {
        int reg_sock = socket(AF_INET, SOCK_STREAM, 0);
        if (reg_sock >= 0) {
            sockaddr_in server_addr{};
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(MAIN_SERVER_PORT);
            inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

            if (connect(reg_sock, (sockaddr*)&server_addr, sizeof(server_addr)) >= 0) {
                ostringstream oss;
                for (const auto& file : shared_files) {
                    oss << file << ",";
                }
                string file_list = oss.str();
                if (!file_list.empty()) file_list.pop_back();
                
                send(reg_sock, file_list.c_str(), file_list.size(), 0);
                cout << "Registered " << shared_files.size() 
                          << " files from current directory\n";
            }
            close(reg_sock);
        }
    }

    // Command loop
    while (true) {
        cout << "Enter command (Send file1,file2 or Exit): \n";
        string input;
        getline(cin, input);

        if (input == "Exit") break;

        if (input.find("Send") == 0) {
            string files = input.substr(5);
            
            int sock = socket(AF_INET, SOCK_STREAM, 0);
            if (sock < 0) continue;

            sockaddr_in server_addr{};
            server_addr.sin_family = AF_INET;
            server_addr.sin_port = htons(MAIN_SERVER_PORT);
            inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr);

            if (connect(sock, (sockaddr*)&server_addr, sizeof(server_addr)) >= 0) {
                string request = "Send " + files;
                send(sock, request.c_str(), request.size(), 0);

                char buffer[BUFFER_SIZE];
                int bytes = recv(sock, buffer, BUFFER_SIZE, 0);
                if (bytes > 0) {
                    string response(buffer, bytes);
                    istringstream iss(response);
                    string line;
                    
                    while (getline(iss, line)) {
                        size_t colon = line.find(':');
                        if (colon != string::npos) {
                            thread(download_file, 
                                      line.substr(0, colon), 
                                      line.substr(colon+1)).detach();
                        }
                    }
                    cout << "Downloads started\n";
                }
            }
            close(sock);
        }
    }

    return 0;
}
