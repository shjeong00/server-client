#include <iostream>
#include <string>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 256
char msg_buffer[MAX_BUFFER_SIZE];

int main(int argc, char *argv[]) {
    int s;
    struct sockaddr_in server_addr;
    char inet_addr_buf[16];
    int len;

    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <IP address> <port>" << std::endl;
        return -1;
    }

    server_addr.sin_family = AF_INET;
    if (inet_pton(AF_INET, argv[1], &server_addr.sin_addr) <= 0) {
        std::cerr << "Error: invalid IP address " << argv[1] << std::endl;
        return -1;
    }
    server_addr.sin_port = htons(atoi(argv[2]));

    // Create a socket
    s = socket(AF_INET, SOCK_STREAM, 0);
    if (s < 0) {
        std::cerr << "Error: socket creation" << std::endl;
        return -1;
    }
    inet_ntop(AF_INET, &server_addr.sin_addr, inet_addr_buf, sizeof(inet_addr_buf));
    std::cout << "Trying " << inet_addr_buf << ":" << ntohs(server_addr.sin_port) << std::endl;

    // Connect to the server
    if (connect(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error: connection error" << std::endl;
        return -1;
    }
    std::cout << "===== Server Connected =====" << std::endl;

    int x, y;

    // Input x
    std::cout << "Enter first number (-9999 ~ 9999): ";
    std::cin >> x;
    if (x < -9999) {
        std::cout << "[Warning]-you can't put number lower than -9999" << std::endl;
        std::cout << "===== Closing Connection =====" << std::endl;
        close(s);
        return 0;
    }
    if (x > 9999) {
        std::cout << "[Warning]-you can't put number larger than 9999" << std::endl;
        std::cout << "===== Closing Connection =====" << std::endl;
        close(s);
        return 0;
    }

    // Input y
    std::cout << "Enter second number(-9999 ~ 9999): ";
    std::cin >> y;
    if (y < -9999) {
        std::cout << "[Warning]-you can't put number lower than -9999" << std::endl;
        std::cout << "===== Closing Connection =====" << std::endl;
        close(s);
        return 0;
    }
    if (y > 9999) {
        std::cout << "[Warning]-you can't put number larger than 9999" << std::endl;
        std::cout << "===== Closing Connection =====" << std::endl;
        close(s);
        return 0;
    }

    // Create message to send
    snprintf(msg_buffer, MAX_BUFFER_SIZE, "%d %d", x, y);
    len = strlen(msg_buffer);

    // Sending Numbers
    send(s, msg_buffer, len + 1, 0);

    // Receiving Result
    len = recv(s, msg_buffer, MAX_BUFFER_SIZE, 0);
    if (len > 0) {
        std::cout << "The sum is: " << msg_buffer << std::endl;
    } else {
        std::cerr << "Error receiving data" << std::endl;
    }

    close(s);
    return 0;
}
