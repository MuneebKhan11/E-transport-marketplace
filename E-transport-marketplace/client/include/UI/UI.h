#pragma once

/**
 * @brief Displays the main menu of the program.
 *
 * This function displays the main menu of the program, which allows the user to choose
 * between logging in, registering a new account, or quitting the program.
 */
#include <string>
#include "CargoUI.h"
#include "DriverUI.h"
#include "TransportUI.h"

void DisplayMainMenu();

/**
 * @brief Displays the login menu of the program.
 *
 * This function displays the login menu of the program, which prompts the user to enter
 * their email and password to log in to an existing account.
 */
void DisplayLoginMenu();

/**
 * @brief Displays the registration menu of the program.
 *
 * This function displays the registration menu of the program, which prompts the user to
 * enter their personal details and create a new account.
 */
void DisplayRegisterMenu();

/**
 * @brief Displays the main menu for drivers.
 *
 * This function displays the main menu for drivers, which allows them to view and manage
 * their current trips and cargo assignments.
 */
void DisplayDriverMenu();

/**
 * @brief Displays the main menu for transportation companies.
 *
 * This function displays the main menu for transportation companies, which allows them
 * to manage their fleet of vehicles, view and assign trips, and track cargo shipments.
 */
void DisplayTransportMenu();

/**
 * @brief Displays the main menu for cargo owners.
 *
 * This function displays the main menu for cargo owners, which allows them to create and
 * manage shipments, track their cargo, and view delivery information.
 */
void DisplayCargoMenu();

/**
 * @brief Displays the screen that shows all available orders.
 *
 * This function displays a screen that shows all available orders. The user will be able to view
 * details such as the pickup and delivery locations, package size and weight, and the payment offered
 * for each delivery.
 */
void DisplayAllOrdersScreen();

/**
 * @brief Displays the notifications screen for the current user.
 *
 * This function displays the notifications screen for the current user. The user will be able to view
 * notifications such as new messages, updates to their orders, and other relevant information.
 */
void DisplayNotifications();

/**
 * @brief Determines if a given lorry type is valid.
 *
 * This function determines if a given lorry type is valid based on predefined criteria. The function
 * returns true if the given lorry type is valid, and false otherwise.
 *
 * @param lorryType The lorry type to check for validity.
 * @return true if the given lorry type is valid, false otherwise.
 */
bool isValidLorryType(const std::string &lorryType);


