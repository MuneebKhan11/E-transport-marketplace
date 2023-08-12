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
#include "include/ServerManager.h"
#include "include/NotificationManager.h"
#include "include/ThreadPool.h"
#include "include/OrderTask.h"
#include "include/responses.h"


//
// Created by Nathan Ellis on 05/04/2023.
//

ServerManager *ServerManager::instance = nullptr;
std::mutex ServerManager::instanceMutex;

ServerManager::ServerManager(int port)
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

ServerManager::~ServerManager() {
    stop();
}

ServerManager *ServerManager::getInstance(int port) {
    if (!instance) {
        std::unique_lock<std::mutex> lock(instanceMutex);
        if (!instance) {
            instance = new ServerManager(port);
        }
    }
    return instance;
}

void ServerManager::handleIncomingConnections() {
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
        std::thread client_thread(&ServerManager::handleRequestFromConnectedClient, this, new_socket);
        client_thread.detach();
    }
}


void ServerManager::handleRequestFromConnectedClient(int client_socket) {
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

        std::cout << "Received message: " << message << std::endl;

        // Parse the request
        std::string method = message.substr(0, message.find(' '));
        std::string path = message.substr(message.find(' ') + 1,
                                          message.find(' ', message.find(' ') + 1) - message.find(' ') - 1);

        // POST
        if (method == "POST" && path == "/login") login(message, client_socket);
        if (method == "POST" && path == "/register") register_request(message, client_socket);
        if (method == "POST" && path == "/placeorder") placeorder_request(message, client_socket);
        if (method == "POST" && path == "/updateProfile") updateProfile_request(message, client_socket);
        if (method == "POST" && path == "/addFeedback") addFeedback_request(message, client_socket);
        if (method == "POST" && path == "/acceptOrder") acceptOrder_request(message, client_socket);
        if (method == "POST" && path == "/forwardOrder") forwardOrderToDriver_request(message, client_socket);
        if (method == "POST" && path == "/declineOrder") declineOrder_request(message, client_socket);
        if (method == "POST" && path == "/updateOrderStatus") updateOrderStatus_request(message, client_socket);

        // GET
        if (method == "GET" && path == "/activeOrders") getActiveOrders_request(message, client_socket);
        if (method == "GET" && path == "/driverActiveOrders") getDriverActiveOrders_request(message, client_socket);
        if (method == "GET" && path == "/pastOrders") getPastOrders_request(message, client_socket);
        if (method == "GET" && path == "/driverPastOrders") getPastOrdersDriver_request(message, client_socket);
        if (method == "GET" && path == "/driverName") getdriverName_request(message, client_socket);
        if (method == "GET" && path == "/allOrders") getAllOrders_request(message, client_socket);
        if (method == "GET" && path == "/availableOrders") getAllAvailableOrders_request(message, client_socket);
        if (method == "GET" && path == "/invoices") getInvoices_request(message, client_socket);
        if (method == "GET" && path == "/ordersWaitingForDriver")
            getOrdersWaitingForDriver_request(message, client_socket);
        if (method == "GET" && path == "/availableDrivers") getAvailableDrivers_request(message, client_socket);
        if (method == "GET" && path == "/notifications") getNotifications_request(message, client_socket);
    }
}


void ServerManager::start() {
    if (running) {
        return;
    }

    // Listen for incoming connections
    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        throw ConnectionException("Listening failed");
    }

    running = true;

    std::cout << "Server has started and is open on port: " << port << std::endl;

    // Start a thread to handle incoming connections
    std::thread connection_thread(&ServerManager::handleIncomingConnections, this);
    // Wait for the connection thread to finish
    connection_thread.join();
}


void ServerManager::stop() {
    running = false;
    close(server_fd);
}

void ServerManager::updateOrderStatus_request(string message, int client_socket) {
    std::string payload = message.substr(message.find("\r\n\r\n") + 4);
    string status = getValueForKey(payload, "status");
    string orderID = getValueForKey(payload, "orderID");
    string driverID = getValueForKey(payload, "driverID");
    if (updateOrderStatus(status, orderID, driverID)) {
        return sendOkResponse(client_socket);
    }
    return sendCustomResponse(client_socket, "500", "Could not accept order, maybe it has been taken?", "");
}

void ServerManager::getNotifications_request(string message, int client_socket) {
    std::string payload = message.substr(message.find("\r\n\r\n") + 4);
    string userID = getValueForKey(payload, "userID");
    bool isTransportAccount = getValueForKey(payload, "isTransportAccount") == "1" ? true : false;
    Json::Value data = getNotifications(userID, isTransportAccount);
    if (data.empty()) sendCustomResponse(client_socket, "404", "", "");
    Json::StreamWriterBuilder builder;
    std::string jsonString = Json::writeString(builder, data);
    return sendCustomResponse(client_socket, "200", "", jsonString);
}

void ServerManager::getAvailableDrivers_request(string message, int client_socket) {
    std::string payload = message.substr(message.find("\r\n\r\n") + 4);
    string source = getValueForKey(payload, "source");
    Json::Value data = getAvailableDrivers(source);
    if (data.empty()) sendCustomResponse(client_socket, "404", "", "");
    Json::StreamWriterBuilder builder;
    std::string jsonString = Json::writeString(builder, data);
    return sendCustomResponse(client_socket, "200", "", jsonString);
}

void ServerManager::getOrdersWaitingForDriver_request(string message, int client_socket) {
    Json::Value data = getOrdersWaitingForDriver();
    if (data.empty()) sendCustomResponse(client_socket, "404", "", "");
    Json::StreamWriterBuilder builder;
    std::string jsonString = Json::writeString(builder, data);
    return sendCustomResponse(client_socket, "200", "", jsonString);
}

void ServerManager::getInvoices_request(string message, int client_socket) {
    return;
}

void ServerManager::forwardOrderToDriver_request(string message, int client_socket) {
    std::string payload = message.substr(message.find("\r\n\r\n") + 4);
    string orderID = getValueForKey(payload, "orderID");
    string driverID = getValueForKey(payload, "driverID");
    if (forwardOrderToDriver(orderID, driverID)) {
        sendAndAddNotification(driverID, "", "You have received a new order",
                               "You have received a new order, please go to the orders screen to accept or decline.");
        return sendOkResponse(client_socket);
    }
    return sendCustomResponse(client_socket, "500", "Could not accept order, maybe it has been taken?", "");
}

void ServerManager::declineOrder_request(string message, int client_socket) {
    std::string payload = message.substr(message.find("\r\n\r\n") + 4);
    string orderID = getValueForKey(payload, "orderID");
    string driverID = getValueForKey(payload, "driverID");
    if (declineOrder(orderID, driverID)) {
        sendAndAddNotification("", "1", getDriverName(driverID) + " has declined an order",
                               "Driver: " + getDriverName(driverID) +
                               " has declined the order, please find a new driver in the orders screen.");
        return sendOkResponse(client_socket);
    }
    return sendCustomResponse(client_socket, "500", "Could not accept order, maybe it has been taken?", "");
}

void ServerManager::acceptOrder_request(string message, int client_socket) {
    std::string payload = message.substr(message.find("\r\n\r\n") + 4);
    string orderID = getValueForKey(payload, "orderID");
    string driverID = getValueForKey(payload, "driverID");
    string userID = getValueForKey(payload, "userID");
    if (acceptOrder(orderID, driverID)) {
        sendAndAddNotification(userID, "", "Your order has been accepted by a driver",
                               "Your order has been accepted by a driver, please view your active orders to see the current status.");
        return sendOkResponse(client_socket);
    }
    return sendCustomResponse(client_socket, "500", "Could not accept order, maybe it has been taken?", "");
}

void ServerManager::getAllAvailableOrders_request(string message, int client_socket) {
    std::string payload = message.substr(message.find("\r\n\r\n") + 4);
    string driverID = getValueForKey(payload, "driverID");
    Json::Value data = getAllAvailableOrders(driverID);
    Json::StreamWriterBuilder builder;
    std::string jsonString = Json::writeString(builder, data);
    return sendCustomResponse(client_socket, "200", "", jsonString);
}

void ServerManager::getAllOrders_request(string message, int client_socket) {
    Json::Value data = getAllOrders();
    Json::StreamWriterBuilder builder;
    std::string jsonString = Json::writeString(builder, data);
    return sendCustomResponse(client_socket, "200", "", jsonString);
}

void ServerManager::getdriverName_request(string message, int client_socket) {
    std::string payload = message.substr(message.find("\r\n\r\n") + 4);
    string userID = getValueForKey(payload, "userID");
    string driverName = getDriverName(userID);
    return sendCustomResponse(client_socket, "200", "", driverName);
}

void ServerManager::getActiveOrders_request(string message, int client_socket) {
    std::string payload = message.substr(message.find("\r\n\r\n") + 4);
    string userID = getValueForKey(payload, "userID");
    Json::Value activeOrders = getUsersActiveOrders(userID);
    if (activeOrders.isNull()) return sendCustomResponse(client_socket, "204", "No orders were found", "");
    Json::StreamWriterBuilder builder;
    std::string jsonString = Json::writeString(builder, activeOrders);
    return sendCustomResponse(client_socket, "200", "Orders returned", jsonString);
}

void ServerManager::getDriverActiveOrders_request(string message, int client_socket) {
    std::string payload = message.substr(message.find("\r\n\r\n") + 4);
    string userID = getValueForKey(payload, "userID");
    Json::Value activeOrders = getDriversActiveOrders(userID);
    if (activeOrders.isNull()) return sendCustomResponse(client_socket, "204", "No orders were found", "");
    Json::StreamWriterBuilder builder;
    std::string jsonString = Json::writeString(builder, activeOrders);
    return sendCustomResponse(client_socket, "200", "Orders returned", jsonString);
}

void ServerManager::getPastOrders_request(string message, int client_socket) {
    std::string payload = message.substr(message.find("\r\n\r\n") + 4);
    string userID = getValueForKey(payload, "userID");
    Json::Value pastOrders = getPastOrders(userID, true);
    if (pastOrders.isNull()) return sendCustomResponse(client_socket, "204", "No orders were found", "");
    Json::StreamWriterBuilder builder;
    std::string jsonString = Json::writeString(builder, pastOrders);
    return sendCustomResponse(client_socket, "200", "Orders returned", jsonString);
}

void ServerManager::getPastOrdersDriver_request(string message, int client_socket) {
    std::string payload = message.substr(message.find("\r\n\r\n") + 4);
    string driverID = getValueForKey(payload, "driverID");
    Json::Value pastOrders = getPastOrders(driverID, false);
    if (pastOrders.isNull()) return sendCustomResponse(client_socket, "204", "No orders were found", "");
    Json::StreamWriterBuilder builder;
    std::string jsonString = Json::writeString(builder, pastOrders);
    return sendCustomResponse(client_socket, "200", "Orders returned", jsonString);
}

void ServerManager::updateProfile_request(string message, int client_socket) {
    std::string payload = message.substr(message.find("\r\n\r\n") + 4);
    string userID = getValueForKey(payload, "userID");
    string valueToBeUpdated = getValueForKey(payload, "valueToBeUpdated");
    string value = getValueForKey(payload, "value");
    updateUser(valueToBeUpdated, userID, value);
    return sendOkResponse(client_socket);
}

void ServerManager::addFeedback_request(string message, int client_socket) {
    std::string payload = message.substr(message.find("\r\n\r\n") + 4);
    string userID = getValueForKey(payload, "userID");
    string orderID = getValueForKey(payload, "orderID");
    string feedback = getValueForKey(payload, "feedbackToBeSaved");
    string feedbackFrom = getValueForKey(payload, "feedbackFrom");
    if (addFeedback(userID, orderID, feedback, feedbackFrom)) {
        return sendCustomResponse(client_socket, "200", "Added feedback", "");
    }
    return sendCustomResponse(client_socket, "500", "Error adding feedback", "");
}

void ServerManager::login(std::string message, int client_socket) {
    // Authenticate the user
    std::string payload = message.substr(message.find("\r\n\r\n") + 4);
    std::string email = getValueForKey(payload, "email");
    std::string password = getValueForKey(payload, "password");
    int userID = authenticateUser(email, password);
    if (userID != 0) {
        connectedClients[userID] = client_socket;
        string data = getUser(to_string(userID));
        sendCustomResponse(client_socket, "200", "success", data);
    } else {
        // Return an error message
        sendUnauthorisedResponse(client_socket);
    }
}

void ServerManager::register_request(std::string message, int client_socket) {
    std::string payload = message.substr(message.find("\r\n\r\n") + 4);
    Json::Value JsonPayload;
    bool isEmailInUseBool;
    try {
        JsonPayload = parseJsonString(payload);
    } catch (JSONException *e) {
        cout << "Error parsing JSON file" << e->what() << endl;
        return sendCustomResponse(client_socket, "500", string("Error parsing payload: ") + e->what(), "");
    }

    string email = getStringValueForKey("", JsonPayload, "email");
    string password = getStringValueForKey("", JsonPayload, "password");
    string accountType = getStringValueForKey("", JsonPayload, "accountType");
    string firstName = getStringValueForKey("", JsonPayload, "firstName");
    string lastName = getStringValueForKey("", JsonPayload, "lastName");
    string mobileNumber = getStringValueForKey("", JsonPayload, "mobileNumber");

    // Test if email is in use or not
    try {
        isEmailInUseBool = isEmailInUse(email);
    } catch (DatabaseException *e) {
        cout << "Error accessing the database to check if email is in use: " << e->what() << endl;
        return sendCustomResponse(client_socket, "500", "Error accessing the database to check for email", "");
    }
    if (isEmailInUseBool) return sendCustomResponse(client_socket, "422", "Email is already in use", "");

    try {
        registerUser(email, password, firstName, lastName, mobileNumber, accountType);
    } catch (DatabaseException *e) {
        return sendCustomResponse(client_socket, "500", e->what(), "");
    }
    sendCustomResponse(client_socket, "200", "User creation success", "");
}

void ServerManager::placeorder_request(std::string message, int client_socket) {
    bool isOrderAccepted = false;
    bool isTimeoutExpired = false;
    int timeoutDuration = 60;
    Order order;
    string payload = message.substr(message.find("\r\n\r\n") + 4);
    Json::Value jsonPayload;
    try {
        jsonPayload = parseJsonString(payload);
    } catch (JSONException *e) {
        cout << "Error parsing json file" << e->what() << endl;
        return sendCustomResponse(client_socket, "500", string("Error parsing payload ") + e->what(), "");
    }

    order.userIDPlacingOrder = stoi(getStringValueForKey("", jsonPayload, "userPlacingOrder"));
    order.source = getStringValueForKey("", jsonPayload, "source");
    order.destination = getStringValueForKey("", jsonPayload, "destination");
    order.cargoDimension = getStringValueForKey("", jsonPayload, "cargoDimension");
    order.cargoWeight = stod(getStringValueForKey("", jsonPayload, "cargoWeight"));
    order.cargoType = getStringValueForKey("", jsonPayload, "cargoType");

    orderTask(order);

    // Send notification to user's notification inbox, NOT to their client as well
    sendAndAddNotification(to_string(order.userIDPlacingOrder), "", "Order placed",
                           "Order going to " + order.destination + " has been successfully placed", false);
    // Send notification to ALL transport accounts and send a realtime notification.
    sendAndAddNotification("", "1", "Order received",
                           "An Order has been received due to be delivered to " + order.destination);

    return sendCustomResponse(client_socket, "200", "Success in creating order", "");
}

//'0' - Create a Cargo Owner account
//'1' - Create a Transportation Company account
//'2' - Create a Driver account