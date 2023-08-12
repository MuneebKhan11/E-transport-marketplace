//
// Created by Nathan Ellis on 10/04/2023.
//

#include "include/NotificationConnection.h"
#include <fcntl.h>
#include <unistd.h>
#include "thread"
#include "json/json.h"
#include <iostream>



NotificationConnection* NotificationConnection::instance = nullptr;

NotificationConnection::NotificationConnection()
{
    // Create the socket
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        throw ConnectionException("Socket creation error, is the server running?");
    }

    // Set the server address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(8081);
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
NotificationConnection::~NotificationConnection() {
    // Close the socket
    close(sock);
}

NotificationConnection* NotificationConnection::getInstance() {
    if (!instance) {
        instance = new NotificationConnection();
    }
    return instance;
}

void NotificationConnection::receiveNotifications()
{
    std::string notification;
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
            int valread = read(sock, buffer, 1024);
            if (valread <= 0) {
                break;
            }
            notification.append(buffer, valread);
            if (notification.find(delimiter) != std::string::npos) {
                break;
            }
        }
    }
    // Now have notification variable
    std::cout << notification << std::endl;
}

std::string NotificationConnection::sendInitalInfo(const std::string& userID) {
    // Send the request
    std::string request = userID;
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

std::vector<Notification> parseNotifications(const std::string& jsonData) {
    std::vector<Notification> notifications;

    Json::CharReaderBuilder builder;
    Json::Value root;
    std::string errs;
    std::istringstream iss(jsonData);
    if (!Json::parseFromStream(builder, iss, &root, &errs)) {
        return notifications;
    }

    for (const auto& key : root.getMemberNames()) {
        const Json::Value &notificationJson = root[key];
        Notification notification;

        notification.notificationID = notificationJson["notificationID"].asString();
        notification.notificationTitle = notificationJson["notificationTitle"].asString();
        notification.notificationBody = notificationJson["notificationBody"].asString();
        notification.notificationGroup = notificationJson["notificationGroup"].asString();
        notification.createdAt = notificationJson["createdAt"].asString();
        notifications.push_back(notification);
    }
    return notifications;
}
