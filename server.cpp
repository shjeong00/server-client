#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

#define MAX_BUFFER_SIZE 256
char msg_buffer[MAX_BUFFER_SIZE];

int main(int argc, char *argv[]) {
    int server_socket, client_socket;
    struct sockaddr_in server_addr, client_addr;
    char inet_addr_buf[16];
    socklen_t client_addr_len = sizeof(client_addr);
    int server_port;
    int len, i;

    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
        return -1;
    }

    // Get server port number
    server_port = atoi(argv[1]);

    // Create server socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        std::cerr << "Error: socket creation" << std::endl;
        return -1;
    }

    // Bind an address IP:Port
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(server_port);
    if (bind(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Error: bind" << std::endl;
        return -1;
    }

    inet_ntop(AF_INET, &server_addr.sin_addr, inet_addr_buf, sizeof(inet_addr_buf));
    std::cout << "Addr: " << inet_addr_buf << std::endl;
    std::cout << "Port: " << ntohs(server_addr.sin_port) << std::endl;

    // Waiting for Connection
    if (listen(server_socket, 5) < 0) {
        std::cerr << "Error: listen" << std::endl;
        return -1;
    }

    while (1) {
        client_socket = accept(server_socket, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_socket < 0) {
            std::cerr << "Error: accept" << std::endl;
            return -1;
        }

        inet_ntop(AF_INET, &client_addr.sin_addr, inet_addr_buf, sizeof(inet_addr_buf));
        std::cout << "===== Client Connected =====" << std::endl;
        std::cout << "From " << inet_addr_buf << ":" << ntohs(client_addr.sin_port) << std::endl;

        // Receiving Numbers
        len = recv(client_socket, msg_buffer, MAX_BUFFER_SIZE, 0);
        if (len > 0) {
            std::cout << "Received: " << msg_buffer << std::endl;
        } else {
            std::cerr << "Error receiving data" << std::endl;
            close(client_socket);
            continue;
        }

        // Parsing the received numbers
        int x, y;
        if (sscanf(msg_buffer, "%d %d", &x, &y) != 2) {
            std::cerr << "Error parsing numbers" << std::endl;
            close(client_socket);
            continue;
        }

        // Calculation
        int sum = x + y;
        snprintf(msg_buffer, MAX_BUFFER_SIZE, "%d", sum);
        len = strlen(msg_buffer);
        
        // Sending Result
        std::cout << "Sending: " << msg_buffer << std::endl;
        send(client_socket, msg_buffer, len + 1, 0);

        // Connection Close
        close(client_socket);
        std::cout << "===== Client Disconnected =====" << std::endl;
    }

    close(server_socket);
    return 0;
}
