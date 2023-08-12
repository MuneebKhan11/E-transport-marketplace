#include "include/SocketConnection.h"
#include <fcntl.h>
#include <unistd.h>
#include <iostream>



SocketConnection* SocketConnection::instance = nullptr;

SocketConnection::SocketConnection() {
    // Create the socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw ConnectionException("Socket creation error, is the server running?");
    }

    // Set the server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8080);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
        throw ConnectionException("Invalid address/ Address not supported");
    }

    // Set the socket to non-blocking mode
    int flags = fcntl(sock, F_GETFL, 0);
    fcntl(sock, F_SETFL, flags | O_NONBLOCK);

    // Connect to the server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        if (errno != EINPROGRESS) {
            throw ConnectionException("SocketConnection failed, is the server running?");
        }
    }
}

// destructor to close socket when object is destroyed
SocketConnection::~SocketConnection() {
    // Close the socket
    close(sock);
}

SocketConnection* SocketConnection::getInstance() {
    if (!instance) {
        instance = new SocketConnection();
    }
    return instance;
}

std::string SocketConnection::sendRequest(const std::string& method, const std::string& path, const std::string& payload) {
    // Send the request
    std::string request = method + " " + path + " HTTP/1.1\r\nHost: " + "127.0.0.1" + ":" + "8080" + "\r\nContent-Length: " + std::to_string(payload.length()) + "\r\n\r\n" + payload;
    send(sock, request.c_str(), request.length(), 0);

    // Wait for the response to become available
    std::string response;
    std::string delimiter = "\r\n\r\n";
    while (true) {
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sock, &readfds);
        struct timeval timeout = { 0, 500000 };
        int ready = select(sock + 1, &readfds, nullptr, nullptr, &timeout);
        if (ready < 0) {
            throw ConnectionException("Error waiting for response");
        }
        if (ready > 0) {
            // Receive the response in chunks
            int valread = read(sock, buffer, 1024);
            if (valread <= 0) {
                break; // Break the loop if no more data is available or an error occurred
            }
            response.append(buffer, valread);

            // Check if the end-of-message identifier was in the last reply
            if (response.find(delimiter) != std::string::npos) {
                // if it is, break
                break;
            }
        }
        // Timeout occurred, try again
    }
    return response;
}