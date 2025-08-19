#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sstream>
#include <thread>
#include <mutex>

using namespace std ; 

unordered_map<string, vector<string>> fileDB;
mutex dbMutex;

void handle_client(int client_socket , string client_ip) {
    char buffer[4096];
    int bytes_read = recv(client_socket, buffer, sizeof(buffer), 0);
    if(bytes_read <= 0) {
        close(client_socket);
        return;
    }
  
    string request(buffer, bytes_read);

    if(request.find("Send") == 0) {
        string response;
        istringstream iss(request.substr(5));
        string filename;
        
        cout << "Received request: " << request << " from " << client_ip << endl;

        while(getline(iss, filename, ',')) {
            filename = filename.substr(filename.find_first_not_of(" \t"));
            filename = filename.substr(0, filename.find_last_not_of(" \t")+1);
            
            if(!filename.empty()) {
                lock_guard<mutex> lock(dbMutex);
                if(fileDB.find(filename) != fileDB.end()) {
                    //for(const auto& ip : fileDB[filename]) {
                        response += filename + ":" + fileDB[filename][0] + "\n";
                      //}
                }
            }
        }
        send(client_socket, response.c_str(), response.size(), 0);
    }

    else if(request.find("UPDATE") == 0) {
      istringstream iss(request);
      string cmd, filename;
      iss >> cmd >> filename;
      
      sockaddr_in client_addr;
      socklen_t len = sizeof(client_addr);
      getpeername(client_socket, (sockaddr*)&client_addr, &len);
      string ip = inet_ntoa(client_addr.sin_addr);

      lock_guard<mutex> lock(dbMutex);
      fileDB[filename].push_back(ip);
      send(client_socket, "UPDATED", 7, 0);
    }

    else {
        // Handle file registration
        sockaddr_in client_addr;
        socklen_t len = sizeof(client_addr);
        getpeername(client_socket, (sockaddr*)&client_addr, &len);
        string ip = inet_ntoa(client_addr.sin_addr);

        istringstream iss(request);
        string filename;
        
        while(getline(iss, filename, ',')) {
            filename = filename.substr(filename.find_first_not_of(" \t"));
            filename = filename.substr(0, filename.find_last_not_of(" \t")+1);
            
            if(!filename.empty()) {
                lock_guard<mutex> lock(dbMutex);
                fileDB[filename].push_back(ip);
            }
        }
        send(client_socket, "OK", 2, 0);
    }
    
    close(client_socket);
}

int main() {
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in address{};
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(4444);

    bind(server_fd, (sockaddr*)&address, sizeof(address));
    listen(server_fd, 5);
    
    cout << "Server listening on port 4444" << endl;
    
    while(true) {
        sockaddr_in client_addr{};
        socklen_t addrlen = sizeof(client_addr);
        int client_socket = accept(server_fd, (sockaddr*)&client_addr, &addrlen);
      
        getpeername(client_socket, (sockaddr*)&client_addr, &addrlen);
        string client_ip = inet_ntoa(client_addr.sin_addr);
        
        //cout << "Client connected : " << client_ip << "\n" ; 
        
        thread(handle_client, client_socket , client_ip ).detach();
    }
}
