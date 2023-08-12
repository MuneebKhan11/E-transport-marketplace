//
// Created by Nathan Ellis on 26/02/2023.
//

#include "include/Utils.h"
#include <iostream>
#include <string>
#include <json/json.h>
#include "sqlite3.h"
#include "include/encrypt.h"
#include "include/Exceptions.h"
#include "include/cityCoordinates.h"
#include "include/Database.h"
#include "include/NotificationManager.h"
#include "include/DatabaseQuery.h"
#include<cmath>

using namespace std;


// Function to get the value associated with a given key in a JSON object
string getValueForKey(string strObject, string key) {
    Json::Value jsonObject;
    Json::Reader reader;
    bool success = reader.parse(strObject, jsonObject);

    if (!success) {
        std::cout << "Failed to parse JSON" << std::endl;
        return "error";
    }
    // Check if the key exists in the object
    if (!jsonObject.isMember(key)) {
        return "";
    }

    // Get the value associated with the key
    Json::Value value = jsonObject[key];

    // Check the type of the value
    if (value.isString()) {
        return value.asString();
    } else if (value.isInt()) {
        return std::to_string(value.asInt());
    } else if (value.isDouble()) {
        return std::to_string(value.asDouble());
    } else if (value.isBool()) {
        return value.asBool() ? "true" : "false";
    } else {
        return "";
    }
}

Json::Value parseJsonString(const string &payloadToParse) {
    Json::Value root;
    Json::Reader reader;
    reader.parse(payloadToParse, root);
    bool parsingSuccessful = reader.parse(payloadToParse, root);
    if (!parsingSuccessful) throw new JSONException("Could not parse JSON file");
    return root;
}

string getStringValueForKey(string strObject, Json::Value userJsonObject, string key) {
    Json::Value jsonObject;
    Json::Reader reader;

    if (strObject.empty() && (userJsonObject.empty() || !userJsonObject.isObject()))
        throw new JSONException(
                "Parameter 'strObject' or 'userJsonObject' must be given or 'userJsonObject' is not a valid JSON Object");
    if (key.empty()) throw new JSONException("Key not specified");


    if (!strObject.empty()) {
        if (reader.parse(strObject, jsonObject)) throw new JSONException("Could not parse the JSON object.");
        if (!jsonObject.isMember(key)) throw new JSONException("Could not find: " + key + " in JSON object.");
        Json::Value value = jsonObject[key];
        if (value.isString()) {
            return value.asString();
        } else if (value.isInt()) {
            return std::to_string(value.asInt());
        } else if (value.isDouble()) {
            return std::to_string(value.asDouble());
        } else if (value.isBool()) {
            return value.asBool() ? "true" : "false";
        } else {
            return "";
        }
    }

    if (!userJsonObject.isMember(key)) throw new JSONException("Could not find: " + key + " in JSON object.");
    Json::Value userJsonValue = userJsonObject.get(key, Json::Value::null);
    if (userJsonValue.isNull()) throw new JSONException("Value for " + key + " is null.");
    return userJsonValue.asString();
}

void
sendAndAddNotification(const string &userID, string notificationGroup, string notificationTitle,
                       string notificationMessage,
                       bool sendNotificationToClient) {
    Database db("e-commerce.db");
    NotificationManager *notificationManager = NotificationManager::getInstance(8081);
    db.connect();
    // If userID is available then the notification is meant for a specific user, else the notificationType
    // variable will be available to send a notification to all user's of a specific account type.
    if (userID.empty()) {
        // Notification is meant for a group of users of a specific account type
        string groupSQL = "INSERT INTO NOTIFICATIONS (notificationGroup, notificationTitle, notificationBody) VALUES (?, ?, ?);";
        db.query(groupSQL, notificationGroup, notificationTitle, notificationMessage);
        db.disconnect();
        if (!sendNotificationToClient) return;
        // Get all userID's that hold the accountType that the notification should be sent to:
        vector<string> users = getAllUsersOfAccountType(notificationGroup);
        if (users.empty()) {
            cout << "No users of that type" << endl;
            return;
        }
        for (const auto &userIDVar: users) {
            notificationManager->sendNotificationToClient(stoi(userIDVar), notificationTitle +
                                                                           " please see your notifications for more information.");
        }
    } else {
        // Notification is meant for a specific user
        string specificUserSQL = "INSERT INTO NOTIFICATIONS (userID, notificationTitle, notificationBody) VALUES (?, ?, ?);";
        db.query(specificUserSQL, userID, notificationTitle, notificationMessage);
        db.disconnect();
        if (!sendNotificationToClient) return;
        notificationManager->sendNotificationToClient(stoi(userID), notificationTitle +
                                                                    " please see your notifications for more information.");
    }
}

void setupDatabase() {
    std::string usersTable = "CREATE TABLE IF NOT EXISTS USER (\n"
                             "\t\"ID\"\tINTEGER NOT NULL,\n"
                             "\t\"email\"\tTEXT NOT NULL,\n"
                             "\t\"password\"\tBLOB NOT NULL,\n"
                             "\t\"firstName\"\tTEXT NOT NULL,\n"
                             "\t\"lastName\"\tTEXT NOT NULL,\n"
                             "\t\"companyName\"\tTEXT,\n"
                             "\t\"accountType\"\tINTEGER NOT NULL,\n"
                             "\t\"mobileNumber\"\tINTEGER NOT NULL,\n"
                             "\t\"address\"\tTEXT,\n"
                             "\t\"NINumber\"\tTEXT,\n"
                             "\t\"drivingID\"\tTEXT,\n"
                             "\t\"regNumber\"\tTEXT,\n"
                             "\t\"lorryDimension\"\tTEXT,\n"
                             "\t\"lorryWeight\"\tREAL DEFAULT 0,\n"
                             "\t\"lorryType\"\tTEXT,\n"
                             "\t\"cpcNumber\"\tINTEGER DEFAULT 0,\n"
                             "\t\"isBusy\"\tINTEGER DEFAULT 0,\n"
                             "\tPRIMARY KEY(\"ID\" AUTOINCREMENT)\n"
                             ");";
    std::string notificationTable = "CREATE TABLE IF NOT EXISTS \"NOTIFICATIONS\" (\n"
                                    "\t\"notificationID\"\tINTEGER NOT NULL,\n"
                                    "\t\"userID\"\tINTEGER DEFAULT 0,\n"
                                    "\t\"notificationTitle\"\tTEXT NOT NULL,\n"
                                    "\t\"notificationBody\"\tTEXT NOT NULL,\n"
                                    "\t\"createdAt\"\tTIMESTAMP DEFAULT CURRENT_TIMESTAMP,\n"
                                    "\t\"notificationGroup\"\tINTEGER DEFAULT 0,\n"
                                    "\tPRIMARY KEY(\"notificationID\" AUTOINCREMENT)\n"
                                    ");";
    std::string ordersTable = "CREATE TABLE IF NOT EXISTS \"USERORDERS\" (\n"
                              "\t\"orderID\"\tINTEGER NOT NULL,\n"
                              "\t\"userID\"\tINTEGER NOT NULL,\n"
                              "\t\"driverID\"\tINTEGER DEFAULT 0,\n"
                              "\t\"cargoDimension\"\tTEXT NOT NULL,\n"
                              "\t\"cargoWeight\"\tREAL NOT NULL,\n"
                              "\t\"cargoType\"\tTEXT NOT NULL,\n"
                              "\t\"source\"\tTEXT NOT NULL,\n"
                              "\t\"destination\"\tTEXT NOT NULL,\n"
                              "\t\"status\"\tTEXT NOT NULL DEFAULT 'Created',\n"
                              "\t\"totalCost\"\tREAL NOT NULL DEFAULT 0,\n"
                              "\t\"driverComment\"\tTEXT NOT NULL DEFAULT 'No feedback given yet',\n"
                              "\t\"cargoOwnerComment\"\tTEXT NOT NULL DEFAULT 'No feedback given yet',\n"
                              "\t\"isAccepted\"\tINTEGER NOT NULL DEFAULT 0,\n"
                              "\t\"isCompleted\"\tINTEGER NOT NULL DEFAULT 0,\n"
                              "\t\"distance\"\tREAL NOT NULL DEFAULT 0,\n"
                              "\t\"hasCargoOwnerLeftComment\"\tINTEGER DEFAULT 0,\n"
                              "\t\"hasDriverLeftComment\"\tINTEGER DEFAULT 0,\n"
                              "\tFOREIGN KEY(\"driverID\") REFERENCES \"USER\"(\"ID\"),\n"
                              "\tFOREIGN KEY(\"userID\") REFERENCES \"USER\"(\"ID\"),\n"
                              "\tPRIMARY KEY(\"orderID\" AUTOINCREMENT)\n"
                              ");";
    Database db("e-commerce.db");
    db.connect();
    db.query(usersTable, "USER");
    db.query(ordersTable, "USERORDERS");
    db.query(notificationTable, "NOTIFICATIONS");
    db.disconnect();
}

double toRadians(double degree) {
    return degree * (M_PI / 180.0);
}

double haversineDistance(double lat1, double lon1, double lat2, double lon2) {
    double dLat = toRadians(lat2 - lat1);
    double dLon = toRadians(lon2 - lon1);

    lat1 = toRadians(lat1);
    lat2 = toRadians(lat2);

    double a = pow(sin(dLat / 2.0), 2) +
               cos(lat1) * cos(lat2) * pow(sin(dLon / 2.0), 2);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));

    return 6371.0 * c;
}

std::string toTitleCase(const std::string &str) {
    std::string result = str;
    bool newWord = true;

    for (auto &ch: result) {
        if (newWord && std::isalpha(ch)) {
            ch = std::toupper(ch);
            newWord = false;
        } else if (std::isspace(ch)) {
            newWord = true;
        } else {
            ch = std::tolower(ch);
        }
    }

    return result;
}

string distanceBetweenCities(string source, string destination) {
    source = toTitleCase(source);
    destination = toTitleCase(destination);
    auto sourceCoords = cityCoordinates.find(source);
    auto destinationCoords = cityCoordinates.find(destination);

    if (sourceCoords == cityCoordinates.end() || destinationCoords == cityCoordinates.end()) {
        cerr << "One or both of the cities not found in the list." << endl;
        return "";
    }
    return to_string(haversineDistance(sourceCoords->second.first, sourceCoords->second.second,
                                       destinationCoords->second.first, destinationCoords->second.second));
}