//
// Created by Nathan Ellis on 10/04/2023.
//

#ifndef CLIENT_NOTIFICATIONCONNECTION_H
#define CLIENT_NOTIFICATIONCONNECTION_H

#include "include/Exceptions.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "thread"

/**
 * @brief Represents a notification in the system.
 *
 * This struct represents a notification in the system. It contains various fields for storing
 * information about the notification, such as its ID, user ID, title, body, and creation date.
 * The `notificationGroup` field is an optional field that can be used to group related notifications.
 */
struct Notification
{
    std::string notificationID;
    std::string userID;
    std::string notificationTitle;
    std::string notificationBody;
    std::string createdAt;
    std::string notificationGroup;
};

/**
 * @brief Represents a connection to the notification server.
 *
 * This class represents a connection to the notification server. It allows the client to send
 * an initial request to the server to retrieve any notifications that were generated while the
 * client was offline, and to receive live updates of new notifications as they are generated.
 * The connection is established using a TCP socket connection.
 */
class NotificationConnection {
private:
    int sock;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};
    static NotificationConnection* instance;

    NotificationConnection();
    ~NotificationConnection();

public:
    static NotificationConnection* getInstance();
    std::string sendInitalInfo(const std::string& userID);
    void receiveNotifications();
};

/**
 * @brief Parses a JSON string containing notification data and returns a vector of Notification objects.
 *
 * This function parses a JSON string containing notification data and returns a vector of Notification objects.
 * The input JSON string should be in a specific format, with each object containing fields for all
 * the necessary notification information. If the parsing is successful, a vector of Notification objects is returned.
 *
 * @param jsonData The JSON string containing notification data to parse.
 * @return A vector of Notification objects, parsed from the input JSON string.
 */
std::vector<Notification> parseNotifications(const std::string& jsonData);

/**
 * @brief An instance of the NotificationConnection class.
 *
 * This is a global variable representing an instance of the NotificationConnection class.
 * It is declared as extern so that it can be accessed from other files.
 */
extern NotificationConnection* NotificationConnection_instance;


#endif //CLIENT_NOTIFICATIONCONNECTION_H
