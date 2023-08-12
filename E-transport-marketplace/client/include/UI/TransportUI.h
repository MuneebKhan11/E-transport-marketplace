//
// Created by Nathan Ellis on 09/04/2023.
//

#ifndef CLIENT_TRANSPORTUI_H
#define CLIENT_TRANSPORTUI_H

#include "include/Driver.h"

/**
 * @brief Completes the account information for the specified transport company user.
 *
 * This function allows the specified transport company user to complete their account information.
 * The user will be prompted to enter details such as their company name, address, and contact information.
 *
 * @param user The transport company user for whom to complete the account information.
 */
void CompleteTransportCompanyAccountInfo(User& user);

/**
 * @brief Displays the orders waiting for a driver to accept for the specified transport company user.
 *
 * This function displays the orders waiting for a driver to accept for the specified transport company user.
 * The user will be able to view details such as the pickup and delivery locations, package size and weight,
 * and the payment offered for the delivery.
 *
 * @param user The transport company user for whom to display the orders waiting for a driver.
 */
void DisplayOrdersWaitingForDriver(User& user);


#endif //CLIENT_TRANSPORTUI_H
