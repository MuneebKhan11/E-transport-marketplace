//
// Created by Nathan Ellis on 05/04/2023.
//

/**
 * @file SocketConnection.h
 * @brief This file contains connection logic to connect to the server.
 */

#ifndef CLIENT_SOCKETCONNECTION_H
#define CLIENT_SOCKETCONNECTION_H

#include "include/Exceptions.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>


/**
 * @brief A class representing a socket connection to a remote server.
 *
 * This class provides a simple interface for sending HTTP requests and receiving
 * server notifications over a socket connection.
 */
class SocketConnection {
private:
    int sock;                    /**< The socket file descriptor. */
    struct sockaddr_in serv_addr;/**< The server address information. */
    char buffer[1024] = {0};     /**< The receive buffer for incoming data. */
    static SocketConnection* instance; /**< The singleton instance of this class. */

    /**
     * @brief Constructor for the SocketConnection class.
     *
     * This constructor initializes the socket connection to the server.
     */
    SocketConnection();

    /**
     * @brief Destructor for the SocketConnection class.
     *
     * This destructor closes the socket connection and frees any associated resources.
     */
    ~SocketConnection();

public:
    /**
     * @brief Returns the singleton instance of the SocketConnection class.
     *
     * @return A pointer to the singleton instance of this class.
     */
    static SocketConnection* getInstance();

    /**
     * @brief Sends an HTTP request to the remote server.
     *
     * @param method The HTTP method to use (e.g. GET, POST, PUT).
     * @param path The path portion of the URL.
     * @param payload Optional payload data to include in the request.
     * @return The HTTP response received from the server.
     */
    std::string sendRequest(const std::string& method, const std::string& path, const std::string& payload);
};


extern SocketConnection* SocketConnection_instance;


#endif //CLIENT_SOCKETCONNECTION_H
