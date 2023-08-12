//
// Created by Nathan Ellis on 09/04/2023.
//

#ifndef CLIENT_DRIVERUI_H
#define CLIENT_DRIVERUI_H

#include "include/User.h"

/**
 * @brief Completes the account information for the specified driver user.
 *
 * This function allows the specified driver user to complete their account information.
 * The user will be prompted to enter details such as their license number, vehicle type,
 * and insurance information.
 *
 * @param user The driver user for whom to complete the account information.
 */
void CompleteDriverAccountInfo(User& user);

/**
 * @brief Displays the shipment history for the specified driver user.
 *
 * This function displays the shipment history for the specified driver user. The user's
 * past shipments, delivery dates, and any issues will be shown on the screen.
 *
 * @param user The driver user for whom to display the shipment history.
 */
void DisplayDriverShipmentHistory(User& user);

/**
 * @brief Displays the available orders for the specified driver user.
 *
 * This function displays the available orders for the specified driver user. The user will
 * be able to view details such as the pickup and delivery locations, package size and weight,
 * and the payment offered for the delivery.
 *
 * @param user The driver user for whom to display the available orders.
 */
void DisplayAvailableOrders(User& user);

/**
 * @brief Displays the active orders for the specified driver user.
 *
 * This function displays the active orders for the specified driver user. The user will be
 * able to view details such as the pickup and delivery locations, package size and weight,
 * and the current status of each active order.
 *
 * @param user The driver user for whom to display the active orders.
 */
void DisplayDriverActiveOrders(User& user);


#endif //CLIENT_DRIVERUI_H
