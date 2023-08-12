//
// Created by Nathan Ellis on 04/04/2023.
//
#include "include/DatabaseQuery.h"
#include "include/Utils.h"
#include <iostream>
#include <string>
#include <json/json.h>
#include "sqlite3.h"
#include "include/encrypt.h"
#include "include/Exceptions.h"
#include <mutex>
#include "include/Database.h"
#include <queue>

using namespace std;

std::vector<std::string> orderKeys = {
        "orderID", "userID", "driverID", "cargoDimension", "cargoWeight", "cargoType", "source", "destination",
        "status", "totalCost", "driverComment", "cargoOwnerComment", "isAccepted", "isCompleted", "distance",
        "hasCargoOwnerLeftComment",
        "hasDriverLeftComment"
};
std::vector<std::string> notificationKeys = {
        "notificationID", "userID", "notificationTitle", "notificationBody", "createdAt", "notificationGroup"
};
std::vector<std::string> accountKeys = {
        "orderID", "userID", "driverID", "cargoDimension", "cargoWeight", "cargoType", "source", "destination",
        "status", "totalCost", "driverComment", "cargoOwnerComment", "isAccepted", "isCompleted", "distance",
        "hasCargoOwnerLeftComment",
        "hasDriverLeftComment"
};

std::vector<std::string> driverKeys = {
        "ID", "email", "password", "firstName", "lastName", "companyName", "accountType", "mobileNumber",
        "address", "NINumber", "drivingID", "regNumber", "lorryDimension", "lorryWeight", "lorryType",
        "cpcNumber", "isBusy"
};

int authenticateUser(const std::string &email, const std::string &password) {
    int id = 0;
    try {
        Database db("e-commerce.db");
        db.connect();
        std::string sql = "SELECT * FROM user WHERE Email = ?";
        auto results = db.query(sql, email);
        db.disconnect();
        if (results.first.empty() || results.first[0][2].empty()) { return id; }
        id = stoi(results.first[0][0]);
        string DBPassword = results.first[0][2];
        string encryptedPassword = encryptString(password);
        return DBPassword.compare(encryptedPassword) == 0 ? id : 0;
    } catch (DatabaseException *e) {
        std::cout << e->what() << std::endl;
        return 0;
    }
}

void registerUser(string &email, string &password, string &firstName, string &lastName, string &mobileNumber,
                  string &accountType) {

    std::string passwordEncrypted = encryptString(password);
    Database db("e-commerce.db");
    db.connect();
    std::string sql = "INSERT INTO USER (email, password, firstName, lastName, mobileNumber, accountType) VALUES (?, ?, ?, ?, ?, ?)";
    auto results = db.query(sql, email, passwordEncrypted, firstName, lastName, mobileNumber, accountType);
    db.disconnect();
}

void updateUser(string valueToBeUpdated, string userID, string value) {
    Database db("e-commerce.db");
    db.connect();
    string sql = "UPDATE USER SET " + valueToBeUpdated + " = ? WHERE ID = ?;";
    db.query(sql, value, userID);
    db.disconnect();
}

bool addFeedback(string userID, string orderID, string feedback, string feedbackFrom) {
    Database db("e-commerce.db");
    db.connect();
    string sqlCargo = "UPDATE USERORDERS SET cargoOwnerComment = ?, hasCargoOwnerLeftComment = 1 WHERE orderID = ?";
    string sqlDriver = "UPDATE USERORDERS SET driverComment = ?, hasDriverLeftComment = 1 WHERE orderID = ?";
    auto results = db.query(feedbackFrom == "Cargo" ? sqlCargo : sqlDriver, feedback, orderID).first;
    db.disconnect();
    if (results.empty()) return false;
    return true;
}

string getDriverName(string userID) {
    Database db("e-commerce.db");
    db.connect();
    string sql = "SELECT firstName FROM USER WHERE ID = ?";
    auto result = db.query(sql, userID).first;
    db.disconnect();
    if (result.empty()) return "Error";
    return result[0][0];
}


Json::Value getOrdersWaitingForDriver() {
    Json::Value ordersWaiting;

    Database db("e-commerce.db");
    db.connect();
    string sql = "SELECT * FROM USERORDERS WHERE driverID = ? AND isAccepted = ?";
    auto results = db.query(sql, "0", "0").first;
    db.disconnect();
    if (results.empty()) return Json::Value::null;
    for (const auto &row: results) {
        Json::Value jsonObject;
        for (size_t i = 0; i < row.size(); ++i) {
            jsonObject[orderKeys[i]] = row[i];
        }
        ordersWaiting[row[0]] = jsonObject;
    }
    return ordersWaiting;
}

Json::Value getAvailableDrivers(string city) {
    Json::Value availableDrivers;

    Database db("e-commerce.db");
    db.connect();
    string sql = "SELECT * FROM USER WHERE accountType = 2 AND address = ? AND isBusy = 0";
    auto results = db.query(sql, city).first; // Just get the returned data, not the row ID
    db.disconnect();
    if (results.empty()) return Json::Value::null;
    for (const auto &row: results) {
        Json::Value jsonObject;
        for (size_t i = 0; i < row.size(); ++i) {
            jsonObject[driverKeys[i]] = row[i];
        }
        availableDrivers[row[0]] = jsonObject;
    }
    return availableDrivers;
}

Json::Value getUsersActiveOrders(string userID) {
    Json::Value activeOrders;

    Database db("e-commerce.db");
    db.connect();
    string sql = "SELECT * FROM USERORDERS WHERE userID = ? and isCompleted = 0";
    auto results = db.query(sql, userID).first; // Just get the returned data, not the row ID
    db.disconnect();
    if (results.empty()) return Json::Value::null;
    for (const auto &row: results) {
        Json::Value jsonObject;
        for (size_t i = 0; i < row.size(); ++i) {
            jsonObject[orderKeys[i]] = row[i];
        }
        activeOrders[row[0]] = jsonObject;
    }
    return activeOrders;
}

Json::Value getDriversActiveOrders(string userID) {
    Json::Value activeOrders;

    Database db("e-commerce.db");
    db.connect();
    string sql = "SELECT * FROM USERORDERS WHERE driverID = ? and isCompleted = 0";
    auto results = db.query(sql, userID).first; // Just get the returned data, not the row ID
    db.disconnect();
    if (results.empty()) return Json::Value::null;
    for (const auto &row: results) {
        Json::Value jsonObject;
        for (size_t i = 0; i < row.size(); ++i) {
            jsonObject[orderKeys[i]] = row[i];
        }
        activeOrders[row[0]] = jsonObject;
    }
    return activeOrders;
}

Json::Value getPastOrders(string userID, bool forCargo) {
    Json::Value pastOrders;
    Database db("e-commerce.db");
    db.connect();
    string sqlCargo = "SELECT * FROM USERORDERS WHERE userID = ? and isCompleted = 1";
    string sqlDriver = "SELECT * FROM USERORDERS WHERE driverID = ? and isCompleted = 1";
    auto results = db.query(forCargo ? sqlCargo : sqlDriver,
                            userID).first; // Just get the returned data, not the row ID
    db.disconnect();
    if (results.empty()) return Json::Value::null;
    for (const auto &row: results) {
        Json::Value jsonObject;
        for (size_t i = 0; i < row.size(); ++i) {
            jsonObject[orderKeys[i]] = row[i];
        }
        pastOrders[row[0]] = jsonObject;
    }
    return pastOrders;
}

Json::Value getAllAvailableOrders(string driverID) {
    Json::Value allOrders;
    Database db("e-commerce.db");
    db.connect();
    string sqlTransport = "SELECT * FROM USERORDERS WHERE driverID = ? AND isAccepted = ?";
    auto results = db.query(sqlTransport, driverID, "0").first;
    db.disconnect();
    if (results.empty()) {
        return Json::Value::null;
    }
    for (const auto &row: results) {
        Json::Value jsonObject;
        for (size_t i = 0; i < row.size(); ++i) {
            jsonObject[orderKeys[i]] = row[i];
        }
        allOrders[row[0]] = jsonObject;
    }
    return allOrders;
}

bool forwardOrderToDriver(string orderID, string driverID) {
    Database db("e-commerce.db");
    db.connect();
    string sql = "SELECT * FROM USERORDERS WHERE orderID = ? AND isAccepted = 0 AND isCompleted = 0";
    auto results = db.query(sql, orderID).first;
    if (results.empty()) {
        // There is no order with that ID that is available to accept
        db.disconnect();
        return false;
    }
    string acceptSQL = "UPDATE USERORDERS SET driverID = ?, isAccepted = 0 WHERE orderID = ?";
    db.query(acceptSQL, driverID, orderID);
    db.disconnect();
    if (results.empty()) return false;
    // Create an Invoice
    return true;
}

bool updateOrderStatus(string status, string orderID, string driverID) {
    Database db("e-commerce.db");
    db.connect();
    string statusSQL = "UPDATE USERORDERS SET status = ? WHERE orderID = ?";
    auto results = db.query(statusSQL, status, orderID).first;
    if (status == "Delivered") {
        string statusFinishSQL = "UPDATE USERORDERS SET isCompleted = ? WHERE orderID = ?";
        db.query(statusFinishSQL, "1", orderID);
        string acceptSQLUser = "UPDATE USER SET isBusy = ? WHERE ID = ?";
        db.query(acceptSQLUser, "0", driverID);
    }
    db.disconnect();
    if (results.empty()) return true;
    return true;
}

bool acceptOrder(string orderID, string driverID) {
    Database db("e-commerce.db");
    db.connect();
    string sql = "SELECT * FROM USERORDERS WHERE orderID = ? AND isAccepted = 0 AND isCompleted = 0 AND driverID = ?";
    auto results = db.query(sql, orderID, driverID).first;
    if (results.empty()) {
        // There is no order with that ID that is available to accept
        db.disconnect();
        return false;
    }
    string acceptSQL = "UPDATE USERORDERS SET isAccepted = 1, status = ? WHERE orderID = ?";
    db.query(acceptSQL, "Accepted by driver", orderID);
    string acceptSQLUser = "UPDATE USER SET isBusy = ?";
    db.query(acceptSQLUser, "1");
    db.disconnect();
    if (results.empty()) return false;
    // Create an Invoice
    return true;
}

bool declineOrder(string orderID, string driverID) {
    Database db("e-commerce.db");
    db.connect();
    string sql = "SELECT * FROM USERORDERS WHERE orderID = ? AND driverID = ?";
    auto results = db.query(sql, orderID, driverID).first;
    if (results.empty()) {
        // There is no order with that ID that is available to accept
        db.disconnect();
        return false;
    }
    string declineSQL = "UPDATE USERORDERS SET driverID = 0, isAccepted = 0 WHERE orderID = ?";
    db.query(declineSQL, orderID);
    db.disconnect();
    if (results.empty()) return false;
    return true;
}

vector<string> getAllUsersOfAccountType(string accountType) {
    vector<string> users;
    Database db("e-commerce.db");
    db.connect();
    string sql = "SELECT ID FROM USER WHERE accountType = ?";
    auto results = db.query(sql, accountType).first;
    db.disconnect();
    for (const auto &row: results) {
        for (const auto &str: row) {
            users.push_back(str);
        }
    }
    return users;
};

Json::Value getAllOrders() {
    Json::Value allOrders;
    Database db("e-commerce.db");
    db.connect();
    string sqlTransport = "SELECT * FROM USERORDERS WHERE isCompleted = ?";
    auto results = db.query(sqlTransport, "1").first;
    db.disconnect();
    if (results.empty()) {
        return Json::Value::null;
    }
    for (const auto &row: results) {
        Json::Value jsonObject;
        for (size_t i = 0; i < row.size(); ++i) {
            jsonObject[orderKeys[i]] = row[i];
        }
        allOrders[row[0]] = jsonObject;
    }
    return allOrders;
}

Json::Value getNotifications(string userID, bool isTransportAccount) {
    Json::Value notifications;
    Database db("e-commerce.db");
    db.connect();

    if (isTransportAccount) {
        string sqlTransport = "SELECT * FROM NOTIFICATIONS WHERE userID = 15 OR notificationGroup = 1 ORDER BY createdAt DESC;";
        auto results = db.query(sqlTransport, userID, userID).first;
        db.disconnect();
        if (results.empty()) {
            return Json::Value::null;
        }
        for (const auto &row: results) {
            Json::Value jsonObject;
            for (size_t i = 0; i < row.size(); ++i) {
                jsonObject[notificationKeys[i]] = row[i];
            }
            notifications[row[0]] = jsonObject;
        }
        return notifications;
    }
    string sqlNotifications = "SELECT * FROM NOTIFICATIONS WHERE userID = ?;";
    auto results = db.query(sqlNotifications, userID).first;
    db.disconnect();
    if (results.empty()) {
        return Json::Value::null;
    }
    for (const auto &row: results) {
        Json::Value jsonObject;
        for (size_t i = 0; i < row.size(); ++i) {
            jsonObject[notificationKeys[i]] = row[i];
        }
        notifications[row[0]] = jsonObject;
    }
    return notifications;
}

string getUser(const std::string &userID) {
    Database db("e-commerce.db");
    db.connect();
    std::string sql = "SELECT * FROM user WHERE ID = ?";
    auto results = db.query(sql, userID);
    db.disconnect();
    if (results.first.empty() || results.first[0][0].empty()) { return ""; }
    // create JSON object
    Json::Value json;
    json["id"] = results.first[0][0];
    json["email"] = results.first[0][1];
    json["firstName"] = results.first[0][3];
    json["lastName"] = results.first[0][4];
    json["companyName"] = results.first[0][5];
    json["accountType"] = results.first[0][6];
    json["mobileNumber"] = results.first[0][7];
    json["address"] = results.first[0][8];
    json["NINumber"] = results.first[0][9];
    json["drivingID"] = results.first[0][10];
    json["regNumber"] = results.first[0][11];
    json["lorryDimensions"] = results.first[0][12];
    json["lorryWeight"] = results.first[0][13];
    json["lorryType"] = results.first[0][14];
    json["cpcNumber"] = results.first[0][15];
    json["isBusy"] = results.first[0][16];

    // serialize JSON object to string
    Json::StreamWriterBuilder builder;
    std::string jsonString = Json::writeString(builder, json);
    return jsonString;
}

bool isEmailInUse(const string &email) {

    Database db("e-commerce.db");
    db.connect();
    std::string sql = "SELECT email FROM USER WHERE email = ?";
    auto results = db.query(sql, email);
    db.disconnect();
    return !results.first.empty();

}

int CalculateShippingRate(string distance, string cargoWeight) {
    double pencePerMile = 0.03;
    double pencePerKg = 0.25;
    double dis, we;

    try {
        dis = std::stod(distance);
        we = std::stod(cargoWeight);
    }
    catch (const std::invalid_argument &e) {
        // Handle the error, e.g., return -1 or print an error message and exit.
        std::cerr << "Error: Invalid input for distance or cargo weight." << std::endl;
    }

    double shippingRate = (dis * pencePerMile) + (we * pencePerKg);
    int roundedShippingRate = static_cast<int>(std::ceil(shippingRate));

    return roundedShippingRate;
}

int createdNewOrderInDatabase(Order order) {
    int orderID = -1;
    int totalCost = CalculateShippingRate(distanceBetweenCities(order.source, order.destination),
                                          to_string(order.cargoWeight));
    string distance = distanceBetweenCities(order.source, order.destination);
    cout << distance << endl;
    order.source[0] = toupper(order.source[0]);
    Database db("e-commerce.db");
    db.connect();
    std::string sql = "INSERT INTO USERORDERS (userID, cargoDimension, cargoWeight, cargoType, source, destination, distance, totalCost) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
    auto results = db.query(sql, order.userIDPlacingOrder, order.cargoDimension, order.cargoWeight, order.cargoType,
                            order.source, order.destination,
                            distance, to_string(totalCost));
    db.disconnect();
    orderID = results.second;
    return orderID; // Return the generated orderID of the created column
}


bool checkIfDriverHasAccepted(int orderID) {
    Database db("e-commerce.db");
    db.connect();
    std::string sql = "SELECT isAccepted FROM USERORDERS WHERE orderID = ?;";
    auto results = db.query(sql, orderID);
    db.disconnect();
    if (results.first.empty()) return false;
    return results.first[0][0] == "1" ? true : false;
}