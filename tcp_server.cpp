#include <iostream>
#include <thread>
#include <vector>
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
            std::cout << "[*] Client disconnected\n";
            break;
        }

        buffer[bytesRead] = '\0';
        std::cout << "[Client] " << buffer << "\n";

        // Echo back the same message
        send(clientSock, buffer, bytesRead, 0);
    }

    close(clientSock);
}

int main() {
    int serverSock, clientSock;
    struct sockaddr_in serverAddr{}, clientAddr{};
    socklen_t addrLen = sizeof(clientAddr);

    serverSock = socket(AF_INET, SOCK_STREAM, 0);
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORT);

    bind(serverSock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(serverSock, 10);

    std::cout << "[+] TCP server listening on port " << PORT << "...\n";

    while (true) {
        clientSock = accept(serverSock, (struct sockaddr*)&clientAddr, &addrLen);
        std::cout << "[+] New client connected!\n";

        std::thread t(handleClient, clientSock);
        t.detach(); // Detach to let it run independently
    }

    close(serverSock);
    return 0;
}
