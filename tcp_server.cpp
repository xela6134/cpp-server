#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9000
#define BUFFER_SIZE 1024

void handleClient(int clientSock) {
    char buffer[BUFFER_SIZE];

    while (true) {
        ssize_t bytesRead = recv(clientSock, buffer, BUFFER_SIZE - 1, 0);
        if (bytesRead <= 0) {
            std::cout << "Client disconnected\n";
            break;
        }

        buffer[bytesRead] = '\0';
        std::cout << "Client Sent: " << buffer << "\n";

        std::string response = "You have sent '";
        response += buffer;
        response += "'.";

        // Echo back the same message
        send(clientSock, response.c_str(), response.length(), 0);
    }

    close(clientSock);
}

int main() {
    // sockaddr_in is a struct representing IP address + port
    int serverSock, clientSock;
    serverSock = socket(AF_INET, SOCK_STREAM, 0);   // SOCK_STREAM is TCP

    struct sockaddr_in serverAddr, clientAddr;
    socklen_t addrLen = sizeof(clientAddr);

    serverAddr.sin_family = AF_INET;                // IPv4
    serverAddr.sin_addr.s_addr = INADDR_ANY;        // Refuses connection if this isnt specified
    serverAddr.sin_port = htons(PORT);              // Make port readable

    // links the socket to a local IP and port, then listens
    if (bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(serverSock);
        return 1;
    }

    listen(serverSock, 10);

    std::cout << "TCP server listening on port " << PORT << "...\n";

    while (true) {
        clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &addrLen);
        std::cout << "New client connected!\n";

        std::thread t(handleClient, clientSock);
        t.detach();
    }

    close(serverSock);
    return 0;
}
