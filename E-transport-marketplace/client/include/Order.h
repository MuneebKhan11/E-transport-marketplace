//
// Created by Nathan Ellis on 06/04/2023.
//

#ifndef CLIENT_ORDER_H
#define CLIENT_ORDER_H

#include <iostream>
#include "json/value.h"
#include "string"

using namespace std;

/**
 * @brief Represents an order in the system.
 *
 * This struct represents an order in the system. It contains various fields for storing
 * information about the order, such as its ID, source and destination, cargo details,
 * driver details, and order status.
 */
struct Order {
    std::string cargoDimension;
    std::string cargoOwnerComment;
    std::string cargoType;
    std::string cargoWeight;
    std::string destination;
    std::string driverComment;
    std::string driverID;
    std::string distance;
    std::string isAccepted;
    std::string isCompleted;
    std::string orderID;
    std::string source;
    std::string status;
    std::string totalCost;
    std::string userID;
    bool hasCargoOwnerLeftComment;
    bool hasDriverLeftComment;
};

/**
 * @brief Parses a JSON string containing order data and returns a vector of Order objects.
 *
 * This function parses a JSON string containing order data and returns a vector of Order objects.
 * The input JSON string should be in a specific format, with each object containing fields for all
 * the necessary order information. If the parsing is successful, a vector of Order objects is returned.
 *
 * @param jsonData The JSON string containing order data to parse.
 * @return A vector of Order objects, parsed from the input JSON string.
 */
std::vector<Order> parseOrders(const string& jsonData);

/**
 * @brief Finds an order with the specified ID in the given vector of Order objects.
 *
 * This function searches the given vector of Order objects for an order with the specified ID.
 * If a matching order is found, a pointer to that order is returned. If no matching order is found,
 * a null pointer is returned instead.
 *
 * @param orders The vector of Order objects to search.
 * @param target_order_id The ID of the order to find.
 * @return A pointer to the matching Order object, or nullptr if no matching order is found.
 */
Order* findOrderByID(const std::vector<Order>& orders, const std::string& target_order_id);

/**
 * @brief Saves feedback for a specified order and user.
 *
 * This function saves feedback for a specified order and user. The `feedbackToBeSaved` parameter
 * contains the actual feedback message, while the `feedbackFrom` parameter indicates whether the
 * feedback is from the cargo owner or the driver. The function returns `true` if the feedback was
 * successfully saved, and `false` otherwise.
 *
 * @param userID The ID of the user submitting the feedback.
 * @param orderID The ID of the order the feedback is for.
 * @param feedbackToBeSaved The feedback message to be saved.
 * @param feedbackFrom Indicates whether the feedback is from the cargo owner or the driver.
 * @return `true` if the feedback was successfully saved, and `false` otherwise.
 */
bool saveFeedback(string userID, string orderID, string feedbackToBeSaved, string feedbackFrom);


#endif //CLIENT_ORDER_H
