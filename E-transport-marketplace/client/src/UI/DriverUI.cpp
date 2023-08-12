//
// Created by Nathan Ellis on 09/04/2023.
//

#include "include/UI/UI.h"
#include "include/Utils.h"
#include "include/Order.h"
#include "include/NotificationConnection.h"


void CompleteDriverAccountInfo(User& user) {
    ClearScreen();
    print("Your account information is incomplete.");
    print("Please enter the following details to continue:");

    string licenseNumber, NINumber, vehicleDetails, lorryType, lorryDimension, regNumber;
    string cpcNumber, lorryWeight, city;

    // Add Driver license number
    do {
        licenseNumber = GetInputString("Enter your driver's license number: ");
        if(licenseNumber.empty())
        {
            print("Please enter a valid License number");
        }
    } while (licenseNumber.empty());

    // Add National Insurance number
    do {
        NINumber = GetInputString("Enter your National Insurance number: ");
        if(NINumber.empty())
        {
            print("Please enter a valid National Insurance number");
        }
    } while (NINumber.empty());

    // Add CPC number
    do {
        cpcNumber = GetInputString("Enter your CPC number: ");
        if(cpcNumber.empty())
        {
            print("Please enter a valid CPC number");
        }
    } while (cpcNumber.empty());

    // Add Lorry type
    do {

        lorryType = GetInputString("Enter your lorry type (R for Rigid, A for Articulated, D for Drawbar, T for Tipper, F for Flatbed, B for Box, C for Curtainsider):" );
        if(!isValidLorryType(lorryType))
        {
            print("Please enter a valid Lorry Type");
        }
    } while (!isValidLorryType(lorryType));

    // Add lorry weight

    do {
        lorryWeight = GetInputString("Enter your lorry weight: ");
        if(lorryWeight.empty())
        {
            print("Please enter a valid Weight");
        }
    } while (lorryWeight.empty());

    // Add lorry dimension
    do {
        lorryDimension = GetInputString("Enter your lorry dimensions: ");
        if(lorryDimension.empty())
        {
            print("Please enter a valid Lorry Dimension");
        }
    } while (lorryDimension.empty());

    // Add lorry reg number
    do {
        regNumber = GetInputString("Enter your lorry registration number: ");
        if(regNumber.empty())
        {
            print("Please enter a valid Registration number");
        }
    } while (regNumber.empty());

    // Add Driver City
    do {
        city = GetInputString("Enter the city where you are based (this will allow us to give orders close to you): ");
        if(!isValidPlace(city))
        {
            print("That is not a valid City in the UK");
        }
    }while(!isValidPlace(city));

    user.updateDrivingID(licenseNumber);
    user.updateNINumber(NINumber);
    user.updateCPCNumber(stoi(cpcNumber));
    user.updateLorryType(lorryType);
    user.updateLorryWeight(stod(lorryWeight));
    user.updateLorryDimensions(lorryDimension);
    user.updateRegNumber(regNumber);
    user.updateAddress(city);
    print("Thank you for your details, you may continue. Press enter when you are ready");
    pressEnterToContinue();
    DisplayDriverMenu();
}

void DisplayDriverShipmentHistory(User& user) {
    string choice, comment, feedback;
    bool isOrderNotFound = false;
    ClearScreen();
    print("Driver Shipment History");
    print("------------------------");
    Json::Value data = user.getShipmentHistory();
    if (data.isNull()) {
        print("You currently have no past shipments");
        pressEnterToContinue();
        ClearScreen();
        return;
    }
    std::vector<Order> orders = parseOrders(data.asString());
    if (orders.empty()) {
        print("We ran into an error getting your shipments, try again later");
        GetInputString("Press any key to go back");
        return;
    }
    for (const auto &order : orders) {
        print("Order #" + order.orderID + " - " + order.source + "->" + order.destination);
    }
    while (true) {
        ClearScreen();
        print("Driver Shipment History");
        print("------------------------");
        for (const auto &order : orders) {
            print("Order #" + order.orderID + " - " + order.source + "->" + order.destination);
        }
        print("");
        if (isOrderNotFound) {
            print("Order: " + choice + " was not found, please try again.");
            isOrderNotFound = false;
        }
        choice = GetInputString("Enter order ID to get more details or enter 'quit' to go back: ");
        if (checkForQuit(choice)) {
            return;
        }
        Order *found_order = findOrderByID(orders, choice);
        if (found_order) {
            string userCreatedOrder = user.getNameByUserID(found_order->userID);
            string cargo = user.getNameByUserID(found_order->userID);
            ClearScreen();
            print("Order " + found_order->orderID + " currently has status of: " + found_order->status);
            print("------------");
            print("Order created by: " + userCreatedOrder);
            print("Order was picked up from: " + found_order->source);
            print("Order is going to: " + found_order->destination);
            print("Distance travelled for order: " + found_order->distance + " miles");
            print("Order is being delivered by: You");
            print(found_order->isAccepted == "0" ? "Order has not been accepted by a driver yet"
                                                 : "Order has been accepted");
            print("Cargo Dimension: " + found_order->cargoDimension);
            print("Cargo Type: " + found_order->cargoType);
            print("Cargo Weight (kg): " + found_order->cargoWeight);
            print("Total cost: " + found_order->totalCost);
            print("Cargo Owner Feedback: " + found_order->cargoOwnerComment);
            print("Driver Feedback: " + found_order->driverComment);
            cout << "Completed: " << (found_order->isCompleted == "0" ? "No" : "Yes") << endl;
            print("------------");
            print("");
            if (found_order->hasDriverLeftComment)
            {
                print("You cannot add further feedback to this order. Press enter to continue. ");
                pressEnterToContinue();
                ClearScreen();
                return;
            }
            comment = GetInputString("Enter 1 to share a comment about the cargo owner or 'quit' to go back: ");
            if (checkForQuit(comment)) { return; }
            if (comment == "1") {
                ClearScreen();
                print("Leave feedback for client: " + cargo + " for order ID: " + found_order->orderID);
                print("------------");
                do{
                    feedback = GetInputSentence("Enter your feedback: ");
                }while(feedback.empty());
                // Update the order with the provided feedback
                found_order->driverComment = feedback;
                saveFeedback(userCreatedOrder, found_order->orderID, feedback, "driver");
                ClearScreen();
                print("Leave feedback for client: " + cargo + " for order ID: " + found_order->orderID);
                print("------------");
                print("");
                print("Thank you for your feedback! Press enter to continue. ");
                pressEnterToContinue();
                ClearScreen();
                return;
            }
        }else{
            isOrderNotFound = true;
        }
    }
}

void DisplayAvailableOrders(User& user)
{
    string choice, accept, acceptConfirm;
    bool isOrderNotFound = false;
    ClearScreen();
    print("Available orders for you to accept");
    print("------------------------");
    Json::Value data = user.getAvailableOrders();
    if (data.isNull()) {
        print("There's currently no orders available for you to take on.");
        pressEnterToContinue();
        ClearScreen();
        return;
    }
    std::vector<Order> orders = parseOrders(data.asString());
    if (orders.empty()) {
        print("We ran into an error getting available orders, try again later");
        print("Press enter to go back");
        pressEnterToContinue();
        return;
    }
    while (true) {
        ClearScreen();
        print("Available orders for you to accept");
        print("------------------------");
        for (const auto &order : orders) {
            print("Order #" + order.orderID + " - " + order.source + "->" + order.destination + " - " + order.cargoWeight + "kg");
        }
        print("");
        if (isOrderNotFound) {
            print("Order: " + choice + " was not found, please try again.");
            isOrderNotFound = false;
        }
        choice = GetInputString("Enter order ID to get more details and accept or enter 'quit' to go back: ");
        if (checkForQuit(choice)) { return;}
        Order *found_order = findOrderByID(orders, choice);
        if (found_order) {
            string userCreatedOrder = user.getNameByUserID(found_order->userID);
            string cargo = user.getNameByUserID(found_order->userID);
            ClearScreen();
            print("Order " + found_order->orderID + " currently has status of: " + found_order->status);
            print("------------");
            print("Order created by: " + userCreatedOrder);
            print("Order should be picked up from: " + found_order->source);
            print("Order needs to go to: " + found_order->destination);
            print("Distance: " + found_order->distance + " miles");
            print("Cargo Dimension: " + found_order->cargoDimension);
            print("Cargo Type: " + found_order->cargoType);
            print("Cargo Weight (kg): " + found_order->cargoWeight);
            print("------------");
            print("");
            do {
                accept = GetInputString("Would you like to accept this order? Y/N: ");
                if (accept == "Y" || accept == "y") {
                    if (user.acceptOrder(found_order->orderID, found_order->userID)) {
                        ClearScreen();
                        print("Available orders for you to accept");
                        print("------------------------");
                        print("");
                        print("You've successfully accepted that order, an update has been sent to the cargo owner");
                        pressEnterToContinue();
                        return;
                    }
                    ClearScreen();
                    print("Available orders for you to accept");
                    print("------------------------");
                    print("");
                    print("We could not accept that order for you. Either the order has already been taken or your account does not support acceptance of orders.");
                    pressEnterToContinue();
                    return;
                }
                if(accept == "N" || accept == "n")
                {
                    if(user.declineOrder(found_order->orderID))
                    {
                        ClearScreen();
                        print("Available orders for you to accept");
                        print("------------------------");
                        print("");
                        print("You've successfully declined that order, an update has been sent to the transportation company");
                        pressEnterToContinue();
                        return;
                    }
                    ClearScreen();
                    print("Available orders for you to accept");
                    print("------------------------");
                    print("");
                    print("We could not decline that order for you. Please try again later.");
                    pressEnterToContinue();
                    return;
                }
                print("Please enter a valid option Y/N");
            }while(accept != "Y" || accept != "y" || accept != "N" || accept != "n");
            continue;
        }else{
            isOrderNotFound = true;
        }
    }
}

void DisplayDriverActiveOrders(User& user) {
    string choice, comment, feedback, statusChoice;
    bool isOrderNotFound = false;
    ClearScreen();
    print("Your active orders");
    print("------------------------");
    Json::Value data = user.getDriverActiveOrders();
    if (data.isNull()) {
        print("You currently have no active orders");
        pressEnterToContinue();
        ClearScreen();
        return;
    }
    std::vector<Order> orders = parseOrders(data.asString());
    if (orders.empty()) {
        print("We ran into an error getting your shipments, try again later");
        GetInputString("Press any key to go back");
        return;
    }
    for (const auto &order : orders) {
        print("Order #" + order.orderID + " - " + order.source + "->" + order.destination);
    }
    while (true) {
        ClearScreen();
        print("Your active orders");
        print("------------------------");
        for (const auto &order : orders) {
            print("Order #" + order.orderID + " - " + order.source + "->" + order.destination);
        }
        print("");
        if (isOrderNotFound) {
            print("Order: " + choice + " was not found, please try again.");
            isOrderNotFound = false;
        }
        choice = GetInputString("Enter order ID to get more details or enter 'quit' to go back: ");
        if (checkForQuit(choice)) {
            return;
        }
        Order *found_order = findOrderByID(orders, choice);
        if (found_order) {
            string userCreatedOrder = user.getNameByUserID(found_order->userID);
            string cargo = user.getNameByUserID(found_order->userID);
            ClearScreen();
            print("Order " + found_order->orderID + " currently has status of: " + found_order->status);
            print("------------");
            print("Order created by: " + userCreatedOrder);
            print("Order was picked up from: " + found_order->source);
            print("Order is going to: " + found_order->destination);
            print("Distance travelled for order: " + found_order->distance + " miles");
            print("Order is being delivered by: You");
            print(found_order->isAccepted == "0" ? "Order has not been accepted by a driver yet"
                                                 : "Order has been accepted");
            print("Cargo Dimension: " + found_order->cargoDimension);
            print("Cargo Type: " + found_order->cargoType);
            print("Cargo Weight (kg): " + found_order->cargoWeight);
            print("Total cost: " + found_order->totalCost);
            print("Cargo Owner Feedback: " + found_order->cargoOwnerComment);
            print("Driver Feedback: " + found_order->driverComment);
            cout << "Completed: " << (found_order->isCompleted == "0" ? "No" : "Yes") << endl;
            print("------------");
            print("");
            if (found_order->hasDriverLeftComment)
            {
                print("You cannot add further feedback to this order. Press enter to continue. ");
                pressEnterToContinue();
                ClearScreen();
                return;
            }
            comment = GetInputString("Enter 1 to share a comment about the cargo owner, 2 to update the status of the order or 'quit' to go back: ");
            if (checkForQuit(comment)) { return; }
            if (comment == "1") {
                ClearScreen();
                print("Leave feedback for client: " + cargo + " for order ID: " + found_order->orderID);
                print("------------");
                do{
                    feedback = GetInputSentence("Enter your feedback: ");
                }while(feedback.empty());
                // Update the order with the provided feedback
                found_order->driverComment = feedback;
                saveFeedback(userCreatedOrder, found_order->orderID, feedback, "driver");
                ClearScreen();
                print("Leave feedback for client: " + cargo + " for order ID: " + found_order->orderID);
                print("------------");
                print("");
                print("Thank you for your feedback! Press enter to continue. ");
                pressEnterToContinue();
                ClearScreen();
                return;
            }else if (comment == "2")
            {
                ClearScreen();
                print("Update the status of the order for client: " + cargo + " of order ID: " + found_order->orderID);
                print("------------");
                if(found_order->status == "Accepted by driver" || found_order->status == "Created")
                {
                    print("1. Loading onto vehicle");
                    print("2. In transit");
                    print("3. Delivered");
                    do{
                        statusChoice = GetInputString("Please choose a new status out of the list: ");
                        if(statusChoice != "1" || statusChoice != "2" || statusChoice != "3")
                        {
                            print("Please enter a valid status");
                        }
                        if(statusChoice == "1")
                        {
                            if(user.updateOrderStatus("Loading onto vehicle", found_order->orderID, found_order->driverID))
                            {
                                ClearScreen();
                                print("Updated order status to 'Loading onto vehicle' successfully");
                                return;
                            }
                            ClearScreen();
                            print("Could not update order status to 'Loading onto vehicle', try again later");
                            pressEnterToContinue();
                            return;
                        }else if(statusChoice == "2")
                        {
                            if(user.updateOrderStatus("In transit", found_order->orderID, found_order->driverID))
                            {
                                ClearScreen();
                                print("Updated order status to 'In transit' successfully");
                                return;
                            }
                            ClearScreen();
                            print("Could not update order status to 'In transit', try again later");
                        }else if(statusChoice == "3")
                        {
                            if(user.updateOrderStatus("Delivered", found_order->orderID, found_order->driverID))
                            {
                                ClearScreen();
                                print("Updated order status to 'Delivered' successfully");
                                return;
                            }else{
                                ClearScreen();
                                print("Could not update order status to 'Delivered', try again later");
                                pressEnterToContinue();
                                return;
                            }
                        }
                    } while (statusChoice != "1" || statusChoice != "2" || statusChoice != "3");
                }else if(found_order->status == "Loading onto vehicle")
                {
                    print("1. In transit");
                    print("2. Delivered");
                    do{
                        statusChoice = GetInputString("Please choose a new status out of the list: ");
                        if(statusChoice != "1" || statusChoice != "2")
                        {
                            print("Please enter a valid status");
                        }

                        if(statusChoice == "1")
                        {
                            if(user.updateOrderStatus("In transit", found_order->orderID, found_order->driverID))
                            {
                                ClearScreen();
                                print("Updated order status to 'In transit' successfully");
                                return;
                            }
                            ClearScreen();
                            print("Could not update order status to 'In transit', try again later");
                            return;
                        }else if(statusChoice == "2")
                        {
                            if(user.updateOrderStatus("Delivered", found_order->orderID, found_order->driverID))
                            {
                                ClearScreen();
                                print("Updated order status to 'Delivered' successfully");
                                return;
                            }else{
                                ClearScreen();
                                print("Could not update order status to 'Delivered', try again later");
                                pressEnterToContinue();
                                return;
                            }
                        }
                    } while (statusChoice != "1" || statusChoice != "2");
                }else if(found_order->status == "In transit")
                {
                    print("1. Delivered");
                    do{
                        statusChoice = GetInputString("Please choose a new status out of the list: ");
                        if(statusChoice != "1")
                        {
                            print("Please enter a valid status");
                        }
                        if(statusChoice == "1")
                        {
                            if(user.updateOrderStatus("Delivered", found_order->orderID, found_order->driverID))
                            {
                                ClearScreen();
                                print("Updated order status to 'Delivered' successfully");
                                return;
                            }else{
                                ClearScreen();
                                print("Could not update order status to 'Delivered', try again later");
                                pressEnterToContinue();
                                return;
                            }
                        }
                    } while (statusChoice != "1");
                }
            }
        }else{
            isOrderNotFound = true;
        }
    }
}