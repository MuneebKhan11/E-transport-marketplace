//
// Created by Nathan Ellis on 04/04/2023.
//

#include "include/OrderTask.h"
#include <mutex>
#include <queue>
#include <sqlite3.h>
#include <iostream>
#include "include/Utils.h"
#include "include/DatabaseQuery.h"

using namespace std;

int orderTask(Order& order) {
    int orderID = createdNewOrderInDatabase(order);
    cout << orderID << endl;
    return orderID;
}

// Create a new entry in the Order table in the database and retrieve the generated orderID
//    int orderID = createOrderEntry(userIDPlacingOrder, cargoDescription, source, destination, cargoDimension, cargoWeight, cargoCondition);
//
//    // Periodically check if any driver has accepted the order
//    while (true) {
//        bool isAccepted = isOrderAccepted(orderID);
//        if (isAccepted) {
//            // If someone has accepted the order, continue the code
//            // ...
//            break;
//        } else {
//            // If no one has accepted the order, wait for a certain amount of time before checking again
//            this_thread::sleep_for(chrono::seconds(10));
//            // If the time waiting has passed 1 minute, return 0
//            // ...
//        }
//    }