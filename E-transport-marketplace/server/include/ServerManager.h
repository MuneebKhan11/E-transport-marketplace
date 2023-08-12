//
// Created by Nathan Ellis on 05/04/2023.
//

#include <iostream>
#include <atomic>
#include <unordered_map>
#include <mutex>
#include<vector>

#ifndef SERVER_SERVERMANAGER_H
#define SERVER_SERVERMANAGER_H

/**
 * @brief A class to create and listen to incoming connections.
 *
 * The Server Manager class allows one instance to run to create a socket server.
 * It uses threads and mutex locks to allow new incoming connections and
 * creates a thread for each client that connects to not block the main thread.
 */
class ServerManager {
public:
    /**
     * Constructor that takes a port number as input.
     *
     * @param port The port number to listen on for incoming connections.
     */
    ServerManager(int port);

    /**
     * Destructor that cleans up any resources used by the server manager.
     */
    ~ServerManager();

    /**
     * Starts the server manager, allowing it to begin listening for incoming connections.
     */
    void start();

    /**
     * Stops the server manager, preventing it from accepting any new connections.
     */
    void stop();

    /**
     * Gets the instance of the server manager for a given port number.
     *
     * @param port The port number for the instance.
     * @return ServerManager* The instance of the server manager.
     */
    static ServerManager *getInstance(int port);

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

    /**
     * Handles a login request from a client.
     *
     * @param message The message containing the login request data.
     * @param client_socket The socket of the client making the request.
     */
    void login(std::string message, int client_socket);

    /**
     * Handles a registration request from a client.
     *
     * @param message The message containing the registration request data.
     * @param client_socket The socket of the client making the request.
     */
    void register_request(std::string message, int client_socket);

    /**
     * Handles a place order request from a client.
     *
     * @param message The message containing the place order request data.
     * @param client_socket The socket of the client making the request.
     */
    void placeorder_request(std::string message, int client_socket);

    /**
     * Handles an update profile request from a client.
     *
     * @param message The message containing the update profile request data.
     * @param client_socket The socket of the client making the request.
     */
    void updateProfile_request(std::string message, int client_socket);

    /**
     * Handles a get active orders request from a client.
     *
     * @param message The message containing the get active orders request data.
     * @param client_socket The socket of the client making the request.
     */
    void getActiveOrders_request(std::string message, int client_socket);

    /**
     * Handles a get driver name request from a client.
     *
     * @param message The message containing the get driver name request data.
     * @param client_socket The socket of the client making the request.
     */
    void getdriverName_request(std::string message, int client_socket);

    /**
     * Handles a get past orders request from a client.
     *
     * @param message The message containing the get past orders request data.
     * @param client_socket The socket of the client making the request.
     */
    void getPastOrders_request(std::string message, int client_socket);

    /**
 * Handles an add feedback request from a client.
 *
 * @param message The message containing the add feedback request data.
 * @param client_socket The socket of the client making the request.
 */
    void addFeedback_request(std::string message, int client_socket);

/**
 * Handles a get past orders for a driver request from a client.
 *
 * @param message The message containing the get past orders for a driver request data.
 * @param client_socket The socket of the client making the request.
 */
    void getPastOrdersDriver_request(std::string message, int client_socket);

/**
 * Handles a get all orders request from a client.
 *
 * @param message The message containing the get all orders request data.
 * @param client_socket The socket of the client making the request.
 */
    void getAllOrders_request(std::string message, int client_socket);

/**
 * Handles a get all available orders request from a client.
 *
 * @param message The message containing the get all available orders request data.
 * @param client_socket The socket of the client making the request.
 */
    void getAllAvailableOrders_request(std::string message, int client_socket);

/**
 * Handles an accept order request from a client.
 *
 * @param message The message containing the accept order request data.
 * @param client_socket The socket of the client making the request.
 */
    void acceptOrder_request(std::string message, int client_socket);

/**
 * Handles a get driver active orders request from a client.
 *
 * @param message The message containing the get driver active orders request data.
 * @param client_socket The socket of the client making the request.
 */
    void getDriverActiveOrders_request(std::string message, int client_socket);

/**
 * Handles a get invoices request from a client.
 *
 * @param message The message containing the get invoices request data.
 * @param client_socket The socket of the client making the request.
 */
    void getInvoices_request(std::string message, int client_socket);

/**
 * Handles a get orders waiting for driver request from a client.
 *
 * @param message The message containing the get orders waiting for driver request data.
 * @param client_socket The socket of the client making the request.
 */
    void getOrdersWaitingForDriver_request(std::string message, int client_socket);

/**
 * Handles a get available drivers request from a client.
 *
 * @param message The message containing the get available drivers request data.
 * @param client_socket The socket of the client making the request.
 */
    void getAvailableDrivers_request(std::string message, int client_socket);

/**
 * Handles a forward order to driver request from a client.
 *
 * @param message The message containing the forward order to driver request data.
 * @param client_socket The socket of the client making the request.
 */
    void forwardOrderToDriver_request(std::string message, int client_socket);

/**
 * Handles a decline order request from a client.
 *
 * @param message The message containing the decline order request data.
 * @param client_socket The socket of the client making the request.
 */
    void declineOrder_request(std::string message, int client_socket);

/**
 * Handles an update order status request from a client.
 *
 * @param message The message containing the update order status request data.
 * @param client_socket The socket of the client making the request.
 */
    void updateOrderStatus_request(std::string message, int client_socket);

/**
 * Handles a get notifications request from a client.
 *
 * @param message The message containing the get notifications request data.
 * @param client_socket The socket of the client making the request.
 */
    void getNotifications_request(std::string message, int client_socket);

    /**
 * The file descriptor for the server socket.
 */
    int server_fd;

/**
 * The port number that the server is listening on.
 */
    int port;

/**
 * A boolean value indicating whether the server is running.
 */
    std::atomic_bool running;

/**
 * A pointer to the instance of the ServerManager class.
 */
    static ServerManager *instance;

/**
 * A mutex used to synchronize access to the instance variable.
 */
    static std::mutex instanceMutex;

/**
 * A vector containing the client sockets of previously connected clients.
 */
    std::vector<int> previouslyConnectedClients;

/**
 * A map containing the client sockets of currently connected clients mapped to their user IDs.
 */
    std::unordered_map<int, int> connectedClients;

};

extern ServerManager *ServerManager_instance;

#endif //SERVER_SERVERMANAGER_H
