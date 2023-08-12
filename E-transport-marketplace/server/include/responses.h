//
// Created by Nathan Ellis on 27/02/2023.
//
#include <iostream>
#include <string>
#include <vector>

#ifndef SERVER_RESPONSES_H
#define SERVER_RESPONSES_H
using namespace std;

/**
 * Sends a 200 OK response to a client with optional data.
 *
 * @param client_socket The socket of the client to send the response to.
 * @param data The optional data to send along with the response.
 */
void sendOkResponse(int client_socket, string data = "");

/**
 * Sends a 401 Unauthorized response to a client.
 *
 * @param client_socket The socket of the client to send the response to.
 */
void sendUnauthorisedResponse(int client_socket);

/**
 * Sends a 404 Not Found response to a client.
 *
 * @param client_socket The socket of the client to send the response to.
 */
void sendNotFoundResponse(int client_socket);

/**
 * Sends a custom response to a client with a given response code, message, and data.
 *
 * @param client_socket The socket of the client to send the response to.
 * @param responseCode The response code to send.
 * @param message The message to send.
 * @param data The optional data to send along with the response.
 */
void sendCustomResponse(int client_socket, string responseCode, string message, string data);


#endif //SERVER_RESPONSES_H
