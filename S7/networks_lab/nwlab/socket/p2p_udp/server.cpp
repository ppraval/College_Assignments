#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sstream>
#include <mutex>
#include <thread>
#include <cstring>
#include <arpa/inet.h>
#include <unistd.h> 

using namespace std;

unordered_map<string, vector<string>> fileDB;
mutex dbMutex;

void handle_request(const string& request, const string& client_ip, int server_socket, sockaddr_in client_addr) {
    char response[4096] = {0};
    socklen_t addr_len = sizeof(client_addr);

    if (request.find("Send") == 0) {
        stringstream ss(request.substr(5));
        string filename;
        string result;

        cout << "Received request: " << request << " from " << client_ip << endl;

        while (getline(ss, filename, ',')) {
            filename = filename.substr(filename.find_first_not_of(" \t"));
            filename = filename.substr(0, filename.find_last_not_of(" \t") + 1);

            if (!filename.empty()) {
                lock_guard<mutex> lock(dbMutex);
                if (fileDB.find(filename) != fileDB.end()) {
                    for (const auto& ip : fileDB[filename]) {
                        result += filename + ":" + ip + "\n";
                    }
                }
            }
        }

        strncpy(response, result.c_str(), sizeof(response) - 1);
    } else if (request.find("UPDATE") == 0) {
        stringstream ss(request);
        string cmd, filename;
        ss >> cmd >> filename;

        lock_guard<mutex> lock(dbMutex);
        fileDB[filename].push_back(client_ip);

        strncpy(response, "UPDATED", sizeof(response) - 1);
    } else {
        // Handle file registration
        stringstream ss(request);
        string filename;

        while (getline(ss, filename, ',')) {
            filename = filename.substr(filename.find_first_not_of(" \t"));
            filename = filename.substr(0, filename.find_last_not_of(" \t") + 1);

            if (!filename.empty()) {
                lock_guard<mutex> lock(dbMutex);
                fileDB[filename].push_back(client_ip);
            }
        }

        strncpy(response, "OK", sizeof(response) - 1);
    }

    sendto(server_socket, response, strlen(response), 0, (sockaddr*)&client_addr, addr_len);
}

int main() {
    int server_socket = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_socket < 0) {
        cerr << "Failed to create socket" << endl;
        return -1;
    }

    sockaddr_in server_addr{};
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(4444);

    if (bind(server_socket, (sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        cerr << "Failed to bind socket" << endl;
        return -1;
    }

    cout << "Server listening on port 4444" << endl;

    while (true) {
        char buffer[4096] = {0};
        sockaddr_in client_addr{};
        socklen_t addr_len = sizeof(client_addr);

        int bytes_received = recvfrom(server_socket, buffer, sizeof(buffer), 0, (sockaddr*)&client_addr, &addr_len);
        if (bytes_received > 0) {
            string request(buffer, bytes_received);
            string client_ip = inet_ntoa(client_addr.sin_addr);

            thread(handle_request, request, client_ip, server_socket, client_addr).detach();
        }
    }

    close(server_socket);
    return 0;
}
