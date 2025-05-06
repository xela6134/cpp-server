#include <iostream>
#include <cstring>
#include <csignal>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8000
#define BUFFER_SIZE 1024

int sockfd;

void handleSigint(int sig) {
    std::cout << "\nGracefully shutting down server...\n";
    close(sockfd);
    exit(0);
}

int main() {
    signal(SIGINT, handleSigint);

    struct sockaddr_in serverAddr, clientAddr;
    char buffer[BUFFER_SIZE];

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);

    if (bind(sockfd, (const struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Bind failed");
        close(sockfd);
        return 1;
    }

    std::cout << "UDP server is running on port " << SERVER_PORT << "...\n";
    std::cout << "------------------------------------------\n";

    while (true) {
        socklen_t len = sizeof(clientAddr);
        ssize_t n = recvfrom(sockfd, buffer, BUFFER_SIZE - 1, 0,
                             (struct sockaddr*)&clientAddr, &len);
        if (n < 0) {
            perror("Receive failed");
            continue;
        }

        buffer[n] = '\0';  // Null-terminate the received message
        char clientIP[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &clientAddr.sin_addr, clientIP, INET_ADDRSTRLEN);

        std::cout << "Received message from " << clientIP << ":" << ntohs(clientAddr.sin_port) << "\n";
        std::cout << "Message: " << buffer << "\n";
        std::cout << "------------------------------------------\n";
    }

    close(sockfd);
    return 0;
}
