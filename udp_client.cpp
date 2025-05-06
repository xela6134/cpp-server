#include <iostream>
#include <cstring>
#include <csignal>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 8000

int sockfd;

void handleSigint(int sig) {
    std::cout << "\nGracefully closing server...\n";
    close(sockfd);
    exit(0);
}

int main() { 
    signal(SIGINT, handleSigint);

    // IPv4 socket address structure
    // Holds server's IP address (sin_addr) and port number (sin_port)
    struct sockaddr_in serverAddr;

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);

    memset(&serverAddr, 0, sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;                        // Always set to AF_INET (IPv4)
    serverAddr.sin_port = htons(SERVER_PORT);               // Numbers are stored in memory in network byte order (most significant byte first)
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr);  // Converts IP address into binary form

    if (sockfd < 0) {
        perror("UDP socket creation failed");
        return 1;
    }

    std::string message;
    std::cout << "------------------------------------------\n";
    while (true) {
        std::cout << "Enter message: ";
        if (!std::getline(std::cin, message)) {
            std::cout << "\nCtrl+D detected. Closing connection...\n";
            break;
        }

        ssize_t sent = sendto(sockfd, message.c_str(), message.size(), 0,
                              (struct sockaddr*)&serverAddr, sizeof(serverAddr));
        if (sent < 0) {
            perror("Failed to send!");
            std::cout << "------------------------------------------\n";
            continue;
        }

        std::cout << "You have sent: " << message << std::endl;
        std::cout << "------------------------------------------\n";
    }

    close(sockfd);
    return 0;
}
