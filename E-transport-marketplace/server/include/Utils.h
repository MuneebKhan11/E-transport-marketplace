//
// Created by Nathan Ellis on 26/02/2023.
//
#include <iostream>
#include <string>
#include "json/value.h"

#ifndef SERVER_UTILS_H
#define SERVER_UTILS_H

using namespace std;


/**
 * Gets the value of a key from a JSON object represented as a string.
 *
 * @param strObject The JSON object represented as a string.
 * @param key The key to get the value of.
 * @return The value associated with the key, or an empty string if the key is not found.
 */
string getValueForKey(string strObject, string key);

/**
 * Parses a JSON string and returns a JSON value.
 *
 * @param payloadToParse The JSON string to parse.
 * @return A JSON value representing the parsed JSON string.
 */
Json::Value parseJsonString(const string &payloadToParse);

/**
 * Gets the value of a key from a JSON object.
 *
 * @param strObject The JSON object represented as a string.
 * @param userJsonObject The JSON object to get the value from.
 * @param key The key to get the value of.
 * @return The value associated with the key, or an empty string if the key is not found.
 */
string getStringValueForKey(string strObject, Json::Value userJsonObject, string key);

/**
 * Calculates the distance between two cities.
 *
 * @param source The name of the source city.
 * @param destination The name of the destination city.
 * @return The distance between the two cities in kilometers.
 */
string distanceBetweenCities(string source, string destination);

/**
 * Sets up the database, creating any necessary tables and indices.
 */
void setupDatabase();

/**
 * Sends a notification to a user and adds it to the database.
 *
 * @param userID The ID of the user to send the notification to.
 * @param notificationGroup The group the notification belongs to.
 * @param notificationTitle The title of the notification.
 * @param notificationMessage The message of the notification.
 * @param sendNotificationToClient Whether to send the notification to the client or not (defaults to true).
 */
void sendAndAddNotification(const string &userID, string notificationGroup, string notificationTitle,
                            string notificationMessage,
                            bool sendNotificationToClient = true);

/**
 * A struct representing an order placed by a user.
 */
struct Order {
    int userIDPlacingOrder; // The ID of the user placing the order.
    string source; // The source location of the order.
    string destination; // The destination location of the order.
    string cargoDimension; // The dimension of the cargo.
    double cargoWeight; // The weight of the cargo.
    string cargoType; // The type of the cargo.
    double totalCost; // The total cost of the order.
};


#endif //SERVER_UTILS_H
