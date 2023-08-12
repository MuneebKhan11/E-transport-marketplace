//
// Created by Nathan Ellis on 27/02/2023.
//

#include "include/responses.h"
#include <iostream>
#include <sys/socket.h>
#include "include/Utils.h"
#include <vector>
#include "json/json.h"


// 401 - Unauthorised
void sendUnauthorisedResponse(int client_socket) {
    Json::Value json;
    json["code"] = 403;
    json["message"] = "Invalid email or password entered";
    json["data"] = "";
    Json::StreamWriterBuilder builder;
    std::string payload = Json::writeString(builder, json);
    // end-of-message identifier
    payload += "\r\n\r\n";
    send(client_socket, payload.c_str(), payload.length(), 0);
}

// 404 - Not Found
void sendNotFoundResponse(int client_socket) {
    Json::Value json;
    json["code"] = 404;
    json["message"] = "Not found";
    json["data"] = "";
    Json::StreamWriterBuilder builder;
    std::string payload = Json::writeString(builder, json);
    // end-of-message identifier
    payload += "\r\n\r\n";
    send(client_socket, payload.c_str(), payload.length(), 0);
}

// 200 - OK
void sendOkResponse(int client_socket, string data) {
    Json::Value json;
    json["code"] = 200;
    json["message"] = "Success";
    json["data"] = data;
    Json::StreamWriterBuilder builder;
    std::string payload = Json::writeString(builder, json);
    // end-of-message identifier
    payload += "\r\n\r\n";
    send(client_socket, payload.c_str(), payload.length(), 0);
}

void sendCustomResponse(int client_socket, string responseCode, string message, string data) {
    Json::Value json;
    json["code"] = responseCode;
    json["message"] = message;
    json["data"] = data;
    Json::StreamWriterBuilder builder;
    std::string payload = Json::writeString(builder, json);
    // end-of-message identifier
    payload += "\r\n\r\n";
    send(client_socket, payload.c_str(), payload.length(), 0);
}




