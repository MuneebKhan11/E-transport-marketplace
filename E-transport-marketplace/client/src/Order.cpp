//
// Created by Nathan Ellis on 06/04/2023.
//
#include "include/Order.h"
#include <iostream>
#include "string"
#include <json/json.h>
#include "include/SocketConnection.h"
#include "include/Utils.h"

using namespace std;

std::vector<Order> parseOrders(const string& jsonData) {
    std::vector<Order> orders;

    Json::CharReaderBuilder builder;
    Json::Value root;
    std::string errs;
    std::istringstream iss(jsonData);
    if (!Json::parseFromStream(builder, iss, &root, &errs)) {
        return orders;
    }

    for (const auto& key : root.getMemberNames()) {
        const Json::Value &orderJson = root[key];
        Order order;

        order.cargoDimension = orderJson["cargoDimension"].asString();
        order.cargoOwnerComment = orderJson["cargoOwnerComment"].asString();
        order.cargoType = orderJson["cargoType"].asString();
        order.cargoWeight = orderJson["cargoWeight"].asString();
        order.destination = orderJson["destination"].asString();
        order.driverComment = orderJson["driverComment"].asString();
        order.driverID = orderJson["driverID"].asString();
        order.isAccepted = orderJson["isAccepted"].asString();
        order.isCompleted = orderJson["isCompleted"].asString();
        order.orderID = orderJson["orderID"].asString();
        order.source = orderJson["source"].asString();
        order.status = orderJson["status"].asString();
        order.totalCost = orderJson["totalCost"].asString();
        order.distance = orderJson["distance"].asString();
        order.userID = orderJson["userID"].asString();
        order.hasCargoOwnerLeftComment = orderJson["hasCargoOwnerLeftComment"].asString() == "1";
        order.hasDriverLeftComment = orderJson["hasDriverLeftComment"].asString() == "1";

        orders.push_back(order);
    }
    return orders;
}

bool saveFeedback(string userID, string orderID, string feedbackToBeSaved, string feedbackFrom)
{
    Json::Value feedbackPayload;
    feedbackPayload["userID"] = userID;
    feedbackPayload["orderID"] = orderID;
    feedbackPayload["feedbackToBeSaved"] = feedbackToBeSaved;
    feedbackPayload["feedbackFrom"] = feedbackFrom;

    try
    {
        SocketConnection* connection = SocketConnection::getInstance();
        string responseString = connection->sendRequest("POST", "/addFeedback", feedbackPayload.toStyledString());
        Json::Value responseJson = convertStringToJson(responseString);
        if(responseJson["code"].asString() != "200") return false;
        return false;
    }catch(const ConnectionException* e)
    {
        throw ConnectionException("We could not save your feedback at the moment");
    }catch (const JSONException* e){
        cout << e->what() <<endl;
    }
}

Order* findOrderByID(const std::vector<Order>& orders, const std::string& target_order_id) {
    for (const auto& order : orders) {
        if (order.orderID == target_order_id) {
            return const_cast<Order*>(&order);
        }
    }
    return nullptr;
}