//
// Created by Nathan Ellis on 04/04/2023.
//

#ifndef SERVER_DATABASEQUERY_H
#define SERVER_DATABASEQUERY_H

#include <iostream>
#include <string>
#include "json/value.h"
#include "include/Utils.h"

using namespace std;

/**
 * @brief Authenticates a user with their email and password.
 *
 * @param[in] email    The email of the user.
 * @param[in] password The password of the user.
 *
 * @return An integer value indicating whether the authentication was successful or not.
 *         0 indicates  that the user with the given email does not exist in the database or the password is wrong,
 *         while anything above 0 indicates successful authentication and is there userID.
 */
int authenticateUser(const std::string &email, const std::string &password);

/**
 * @brief Retrieves the user details for a given user ID.
 *
 * @param[in] userID The ID of the user whose details are to be retrieved.
 *
 * @return The user details as a string.
 */
std::string getUser(const std::string &userID);

/**
 * @brief Checks if an email is already registered in the database.
 *
 * @param[in] email The email to be checked.
 *
 * @return A boolean value indicating whether the email is already in use (true) or not (false).
 */
bool isEmailInUse(const std::string &email);

/**
 * @brief Registers a new user with the given details.
 *
 * @param[in,out] email        The email of the new user.
 * @param[in,out] password     The password of the new user.
 * @param[in,out] firstName    The first name of the new user.
 * @param[in,out] lastName     The last name of the new user.
 * @param[in,out] mobileNumber The mobile number of the new user.
 * @param[in,out] accountType  The account type of the new user.
 */
void registerUser(std::string &email, std::string &password, std::string &firstName, std::string &lastName,
                  std::string &mobileNumber, std::string &accountType);

/**
 * @brief Creates a new order in the database.
 *
 * @param[in] order The order to be created.
 *
 * @return An integer value indicating the ID of the newly created order.
 */
int createdNewOrderInDatabase(Order order);

/**
 * @brief Checks if a driver has accepted an order.
 *
 * @param[in] orderID The ID of the order to check.
 *
 * @return A boolean value indicating whether the order has been accepted by a driver (true) or not (false).
 */
bool checkIfDriverHasAccepted(int orderID);

/**
 * @brief Updates the value of a given field for a given user ID.
 *
 * @param[in] valueToBeUpdated The name of the field to be updated.
 * @param[in] userID           The ID of the user whose field is to be updated.
 * @param[in] value            The new value of the field.
 */
void updateUser(std::string valueToBeUpdated, std::string userID, std::string value);

/**
 * Returns the active orders for a given user.
 *
 * @param userID The ID of the user.
 * @return Json::Value The active orders for the given user.
 */
Json::Value getUsersActiveOrders(string userID);

/**
 * Returns past orders for a given user.
 *
 * @param userID The ID of the user.
 * @param forCargo A boolean indicating whether the past orders are for a cargo.
 * @return Json::Value The past orders for the given user.
 */
Json::Value getPastOrders(string userID, bool forCargo);

/**
 * Returns all orders.
 *
 * @return Json::Value All orders.
 */
Json::Value getAllOrders();

/**
 * Returns the name of a driver with a given ID.
 *
 * @param userID The ID of the driver.
 * @return string The name of the driver.
 */
string getDriverName(string userID);

/**
 * Adds feedback for a given order and user.
 *
 * @param userID The ID of the user.
 * @param orderID The ID of the order.
 * @param feedback The feedback to add.
 * @param feedbackFrom The ID of the user providing the feedback.
 * @return bool True if the feedback was added successfully, false otherwise.
 */
bool addFeedback(string userID, string orderID, string feedback, string feedbackFrom);

/**
 * Returns all available orders for a given driver.
 *
 * @param driverID The ID of the driver.
 * @return Json::Value All available orders for the given driver.
 */
Json::Value getAllAvailableOrders(string driverID);

/**
 * Updates the status of an order with a given ID and driver ID.
 *
 * @param status The new status of the order.
 * @param orderID The ID of the order.
 * @param driverID The ID of the driver.
 * @return bool True if the status was updated successfully, false otherwise.
 */
bool updateOrderStatus(string status, string orderID, string driverID);

/**
 * Returns the active orders for a given driver.
 *
 * @param userID The ID of the driver.
 * @return Json::Value The active orders for the given driver.
 */
Json::Value getDriversActiveOrders(string userID);

/**
 * Returns all orders waiting for a driver.
 *
 * @return Json::Value All orders waiting for a driver.
 */
Json::Value getOrdersWaitingForDriver();

/**
 * Returns all available drivers in a given city.
 *
 * @param city The city to search for drivers in.
 * @return Json::Value All available drivers in the given city.
 */
Json::Value getAvailableDrivers(string city);

/**
 * Forwards an order to a given driver.
 *
 * @param orderID The ID of the order.
 * @param driverID The ID of the driver.
 * @return bool True if the order was forwarded successfully, false otherwise.
 */
bool forwardOrderToDriver(string orderID, string driverID);

/**
 * Declines an order with a given ID and driver ID.
 *
 * @param orderID The ID of the order.
 * @param driverID The ID of the driver.
 * @return bool True if the order was declined successfully, false otherwise.
 */
bool declineOrder(string orderID, string driverID);

/**
 * Updates the status of an order with a given ID.
 *
 * @param status The new status of the order.
 * @param orderID The ID of the order.
 * @return bool True if the status was updated successfully, false otherwise.
 */
bool updateOrderStatus(string status, string orderID);

/**
 * Returns all users of a given account type.
 *
 * @param accountType The type of account to search for.
 * @return vector<string> All users of the given account type.
 */
vector<string> getAllUsersOfAccountType(string accountType);

/**
 * Accepts an order with a given ID and driver ID.
 *
 * @param orderID The ID of the order.
 * @param driverID The ID of the driver.
 * @return bool True if the order was accepted successfully, false otherwise.
 */
bool acceptOrder(string orderID, string driverID);

/**
 * Returns notifications for a given user and account type.
 *
 * @param userID The ID of the user.
 * @param isTransportAccount A boolean indicating whether the user has a transport account.
 * @return Json::Value Notifications for the given user and account type.
 */
Json::Value getNotifications(string userID, bool isTransportAccount);


#endif //SERVER_DATABASEQUERY_H
