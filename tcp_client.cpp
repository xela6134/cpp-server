#include <iostream>
#include <cstring>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_PORT 9000
#define BUFFER_SIZE 1024

int main() {
    int sockfd;
    struct sockaddr_in serverAddr{};
    char buffer[BUFFER_SIZE];

    // Create TCP socket
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        perror("Socket creation failed");
        return 1;
    }

    // Define server address
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    inet_pton(AF_INET, "127.0.0.1", &serverAddr.sin_addr); // Connect to localhost

    // Connect to the server
    if (connect(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        perror("Connection failed");
        close(sockfd);
        return 1;
    }

    std::cout << "Connected to localhost server 127.0.0.1:" << SERVER_PORT << "\n";

    std::string input;
    while (true) {
        std::cout << "> ";
        if (!std::getline(std::cin, input)) {
            std::cout << "\n[!] Ctrl+D detected. Closing connection...\n";
            break;
        }

        if (input.empty()) continue;

        // Send message
        send(sockfd, input.c_str(), input.size(), 0);

        // Receive server response
        ssize_t n = recv(sockfd, buffer, BUFFER_SIZE - 1, 0);
        if (n <= 0) {
            std::cout << "[!] Server closed connection or error occurred.\n";
            break;
        }

        buffer[n] = '\0';
        std::cout << "[Server] " << buffer << "\n";
    }

    close(sockfd);
    return 0;
}
