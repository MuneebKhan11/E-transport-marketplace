//
// Created by Nathan Ellis on 09/04/2023.
//

#ifndef CLIENT_CARGOUI_H
#define CLIENT_CARGOUI_H


#include "include/User.h"
#include "include/Order.h"


/**
 * @brief Displays the order placing screen for the specified user.
 *
 * This function displays the order placing screen for the specified user. The user's
 * details and order history will be shown on the screen.
 *
 * @param user The User class for whom to display the order placing screen.
 */
void DisplayOrderPlacingScreen(User& user);

/**
 * @brief Calculates the shipping rate for the specified order.
 *
 * This function calculates the shipping rate for the specified order based on the
 * order details such as the shipping address, package weight and dimensions, etc.
 *
 * @param order The order for which to calculate the shipping rate.
 * @return The calculated shipping rate.
 */
int CalculateShippingRate(Order& order);

/**
 * @brief Displays the screen for calculating the shipping rate.
 *
 * This function displays the screen for calculating the shipping rate. The user will
 * be prompted to enter the necessary details for calculating the shipping rate.
 */
void DisplayCalculateShipping();

/**
 * @brief Displays the cargo invoices for the specified user.
 *
 * This function displays the cargo invoices for the specified user. The user's order
 * history and shipping details will be shown on the screen.
 *
 * @param user The user for whom to display the cargo invoices.
 */
void DisplayCargoInvoices(User& user);


#endif //CLIENT_CARGOUI_H
