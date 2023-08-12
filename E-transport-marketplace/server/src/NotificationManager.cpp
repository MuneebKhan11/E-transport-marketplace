//
// Created by Nathan Ellis on 10/04/2023.
//

#include "include/NotificationManager.h"
#include <iostream>
#include <unistd.h>
#include "include/Utils.h"
#include "include/DatabaseQuery.h"
#include "include/responses.h"
#include "include/Exceptions.h"
#include <cstring>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <json/json.h>
#include <thread>

NotificationManager *NotificationManager::instance = nullptr;
std::mutex NotificationManager::instanceMutex;

NotificationManager::NotificationManager(int port)
        : port(port), running(false) {

    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);

    // Create the server socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        throw ConnectionException("Socket creation failed");
    }

    // Set the socket options
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        throw ConnectionException("Setting socket options failed");
    }

    // Bind the socket to a port
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(port);
    if (::bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("bind failed");
        throw ConnectionException("Bind to port failed - maybe it is already in use?");
    }
}

NotificationManager::~NotificationManager() {
    stop();
}

NotificationManager *NotificationManager::getInstance(int port) {
    if (!instance) {
        std::unique_lock<std::mutex> lock(instanceMutex);
        if (!instance) {
            instance = new NotificationManager(port);
        }
    }
    return instance;
}

void NotificationManager::handleIncomingConnections() {
    while (running) {
        int new_socket;
        struct sockaddr_in address;
        int addrlen = sizeof(address);

        if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &addrlen)) < 0) {
            if (!running) {
                break;
            }

            perror("accept failed");
            throw ConnectionException("Accepting incoming connection failed");
        }

        std::cout << "New connection from " << inet_ntoa(address.sin_addr) << ":" << ntohs(address.sin_port)
                  << std::endl;

        // Start a new thread to handle requests from the new client
        std::thread notification_thread(&NotificationManager::handleRequestFromConnectedClient, this, new_socket);
        notification_thread.detach();
    }
}

void NotificationManager::sendNotificationToClient(int userID, std::string message) {
    std::unique_lock<std::mutex> lock(clientSocketsMutex);
    auto it = clientSockets.find(userID);
    if (it != clientSockets.end()) {
        int client_socket = it->second;
        lock.unlock();
        message += "\r\n\r\n";
        send(client_socket, message.c_str(), message.length(), 0);
        cout << "Sent notification to client: " << userID << endl;
    } else {
        lock.unlock();
        std::cerr << "Client not found: " << userID << std::endl;
    }
}


void NotificationManager::handleRequestFromConnectedClient(int client_socket) {
    int userID = -1;
    while (running) {
        // Read the request from the client socket
        char buffer[1024] = {0};
        int valread = read(client_socket, buffer, 1024);
        if (valread == 0) {
            // client has disconnected
            close(client_socket);
            return;
        }
        std::string message(buffer);
        if (userID == -1) {
            userID = stoi(message);
            if (userID != -1) {
                std::unique_lock<std::mutex> lock(clientSocketsMutex);
                std::cout << userID << ": " << client_socket << endl;
                clientSockets[userID] = client_socket;
                lock.unlock();
                std::string payload;
                payload += "\r\n\r\n";
                send(client_socket, payload.c_str(), payload.length(), 0);
            } else {
                // Handle invalid client ID case
                close(client_socket);
                return;
            }
            std::string payload;
            payload += "\r\n\r\n";
            send(client_socket, payload.c_str(), payload.length(), 0);
        }
    }
    std::unique_lock<std::mutex> lock(clientSocketsMutex);
    clientSockets.erase(userID);
    lock.unlock();
}


void NotificationManager::start() {
    if (running) {
        return;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        throw ConnectionException("Listening failed");
    }

    running = true;

    std::cout << "Notification server has started and is open on port: " << port << std::endl;

    // Start a thread to handle incoming connections
    std::thread connection_thread(&NotificationManager::handleIncomingConnections, this);
    // Wait for the connection thread to finish
    connection_thread.join();
}


void NotificationManager::stop() {
    running = false;
    close(server_fd);
}