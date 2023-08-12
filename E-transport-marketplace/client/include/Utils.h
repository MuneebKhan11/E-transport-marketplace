//
// Created by Nathan Ellis on 16/02/2023.
//

/**
 * @file Utils.h
 * @brief This file contains utility functions for the project.
 */


#pragma once
#include <string>
#include <json/value.h>
#include "User.h"

using namespace std;

#ifndef HEADER_H
#define HEADER_H

/**
 * @brief Get an integer input from the user.
 *
 * @param prompt The prompt to display to the user.
 * @param maxOptions The maximum value the user can input.
 * @return The integer value entered by the user.
 */
int GetInputInt(string prompt, int maxOptions);

/**
 * @brief Convert a string to lowercase.
 *
 * @param textToConvert The string to convert.
 * @return The converted string in lowercase.
 */
string toLower(string textToConvert);

/**
 * @brief Get a string input from the user.
 *
 * @param prompt The prompt to display to the user.
 * @return The string entered by the user.
 */
string GetInputString(string prompt);

string GetInputSentence(const std::string& prompt);

/**
 * @brief Check if an email address is valid.
 *
 * @param email The email address to check.
 * @return True if the email address is valid, false otherwise.
 */
bool isValidEmail(const string& email);

/**
 * @brief Check if a password is valid.
 *
 * @param password The password to check.
 * @param confirmPassword The confirm password to check.
 * @return True if the password is valid, false otherwise.
 */
bool isValidPassword(const string& password, const string& confirmPassword);

/**
 * @brief Clear the console screen.
 */
void ClearScreen();

/**
 * @brief Print text to the console.
 *
 * @param text The text to print.
 */
void print(const string& text);

/**
 * @brief Print an error message to the console.
 *
 * @param text The error message to print.
 */
void printError(const string& text);

/**
 * @brief Get the JSON value for a given key in a JSON object.
 *
 * @param strObject The JSON object to search.
 * @param key The key to search for.
 * @return The JSON value for the given key.
 */
Json::Value getJsonValueForKey(string strObject, string key);

/**
 * @brief Check if input string contains a valid dimension.
 *
 * @param input The input string to check.
 * @return True if the input string contains a valid dimension, false otherwise.
 */
bool isValidDimensions(std::string input);

/**
 * @brief Check if input string contains a valid weight.
 *
 * @param str The input string to check.
 * @return True if the input string contains a valid weight, false otherwise.
 */
bool isValidWeight(const std::string& str);

/**
 * @brief Check if input string contains a valid cargo type.
 *
 * @param cargoType The input string to check.
 * @return True if the input string contains a valid cargo type, false otherwise.
 */
bool isValidCargoType(const std::string& cargoType);

bool isValidCPCNumber(const string& CPCNumber);

void pressEnterToContinue();

/**
 * @brief Get the string value for a given key in a JSON object.
 *
 * @param strObject The JSON object to search.
 * @param userJsonObject The user JSON object to search.
 * @param key The key to search for.
 * @return The string value for the given key.
 */
string getStringValueForKey(string strObject = "", Json::Value userJsonObject = Json::Value::null, string key = "");

/**
 * @brief Check if input string contains a valid mobile number.
 *
 * @param mobileNumber The input string to check.
 * @return True if the input string contains a valid mobile number, false otherwise.
 */
bool isValidMobileNumber(const string& mobileNumber);

/**
 * @brief Checks if the given account type is valid
 *
 * @param accountType The account type to validate
 * @return true if the account type is valid, false otherwise
 */
bool isValidAccountType(const string& accountType);

/**
 * @brief Checks if the given NI number is valid
 *
 * @param NINumber The NI number to validate
 * @return true if the NI number is valid, false otherwise
 */
bool isValidNiNumber(const string& NINumber);

/**
 * @brief Checks if the given driving license number is valid
 *
 * @param DrivingID The driving license number to validate
 * @return true if the driving license number is valid, false otherwise
 */
bool isValidDrivingLicenseNumber(const string& DrivingID);

/**
 * @brief Validates the login credentials of the user
 *
 * @param email The email of the user
 * @param password The password of the user
 * @param user The user object to fill with data if the login is successful
 * @return true if the login is successful, false otherwise
 */
bool ValidateLogin(const string& email, const string& password, User& user);

/**
 * @brief Checks if the given text is a quit command
 *
 * @param text The text to check for a quit command
 * @return true if the text is a quit command, false otherwise
 */
bool checkForQuit(string text);

/**
 * @brief Converts a string to a JSON object.
 *
 * @param stringToConvert The string to convert to JSON.
 * @return The resulting JSON object.
 */
Json::Value convertStringToJson(string stringToConvert);

/**
 * @brief Checks if a given lorry type is valid.
 *
 * @param lorryType The lorry type to check.
 * @return True if the lorry type is valid, false otherwise.
 */
bool isValidLorryType(const std::string &lorryType);

/**
 * @brief Checks if a given city name is valid.
 *
 * @param city The name of the city to check.
 * @return True if the city name is valid, false otherwise.
 */
bool isValidPlace(const string& city);

/**
 * @brief Calculates the distance between two cities.
 *
 * @param source The name of the source city.
 * @param destination The name of the destination city.
 * @return A string representing the distance between the two cities.
 */
string distanceBetweenCities(string source, string destination);

/**
 * @brief Converts a city name to a standardized format.
 *
 * @param cityName The name of the city to convert.
 * @return The standardized city name.
 */
std::string convertCityName(const std::string& cityName);



#endif
