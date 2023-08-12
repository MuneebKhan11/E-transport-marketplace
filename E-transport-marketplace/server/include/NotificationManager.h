//
// Created by Nathan Ellis on 10/04/2023.
//


#ifndef SERVER_NOTIFICATIONMANAGER_H
#define SERVER_NOTIFICATIONMANAGER_H

#include <iostream>
#include <map>


/**
 * @brief A class to connect and send notifications to connected clients.
 *
 * The Notification Manager class provides connection and the ability to send
 * notifications to specific clients currently connected to the server
 */
class NotificationManager {
public:
    /**
     * Constructor that takes a port number as input.
     *
     * @param port The port number to listen on for incoming connections.
     */
    NotificationManager(int port);

    /**
     * Destructor that cleans up any resources used by the notification manager.
     */
    ~NotificationManager();

    /**
     * Starts the notification manager, allowing it to begin listening for incoming connections.
     */
    void start();

    /**
     * Stops the notification manager, preventing it from accepting any new connections.
     */
    void stop();

    /**
     * Gets the instance of the notification manager for a given port number.
     *
     * @param port The port number for the instance.
     * @return NotificationManager* The instance of the notification manager.
     */
    static NotificationManager *getInstance(int port);

    /**
     * Sends a notification message to a connected client with a given user ID.
     *
     * @param userID The ID of the user to send the notification to.
     * @param message The notification message to send.
     */
    void sendNotificationToClient(int userID, std::string message);

private:
    /**
     * Handles a request from a client socket.
     *
     * @param client_socket The socket of the client making the request.
     */
    void handleRequest(int client_socket);

    /**
     * Handles incoming connections from clients.
     */
    void handleIncomingConnections();

    /**
     * Handles a request from a connected client.
     *
     * @param client_socket The socket of the connected client making the request.
     */
    void handleRequestFromConnectedClient(int client_socket);

    int server_fd; // The file descriptor for the server socket.
    int port; // The port number to listen on for incoming connections.
    std::atomic_bool running; // A flag indicating whether the notification manager is running.
    static NotificationManager *instance; // A singleton instance of the notification manager.
    static std::mutex instanceMutex; // A mutex used to synchronize access to the singleton instance.
    std::map<int, int> clientSockets; // A map of client sockets to user IDs.
    std::mutex clientSocketsMutex; // A mutex used to synchronize access to the client sockets map.
};

extern NotificationManager *NotificationManger_instance; // A global instance of the notification manager. (Note: typo in variable name)

#endif //SERVER_NOTIFICATIONMANAGER_H
