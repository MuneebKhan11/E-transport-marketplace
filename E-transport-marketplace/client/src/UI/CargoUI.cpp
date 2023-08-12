//
// Created by Nathan Ellis on 09/04/2023.
//

#include "include/UI/UI.h"
#include "include/Utils.h"
#include "include/Order.h"



void DisplayOrderPlacingScreen(User& user)
{
    string source, destination, dimension, weight, cargoType;
    ClearScreen();
    do{
        print("Place an order");
        print("------------");
        print("enter 'quit' to cancel");
        print("");
        print("Where is your order coming from?");
        print("");
        source = GetInputString("Enter the source: ");
        if(checkForQuit(source)) return;
        if(!isValidPlace(source))
        {
            ClearScreen();
            print("You need to enter a valid city within the UK");
        }
    } while (!isValidPlace(source));
    ClearScreen();
    do{
        print("Place an order");
        print("------------");
        print("enter 'quit' to cancel");
        print("");
        print("Where is your order going to?");
        print("");
        destination = GetInputString("Enter the destination: ");
        if(checkForQuit(destination)) return;
        if(!isValidPlace(destination))
        {
            ClearScreen();
            print("You need to enter a valid city within the UK");
        }
    } while (!isValidPlace(destination));
    ClearScreen();
    do{
        print("Place an order");
        print("------------");
        print("enter 'quit' to cancel");
        print("");
        print("What is the dimension of the cargo you'd like delivered?");
        print("");
        dimension = GetInputString("Enter the dimension: ");
        if(checkForQuit(dimension)) return;
        if(dimension.empty())
        {
            ClearScreen();
            print("You need to enter a dimension");
        }
    } while (dimension.empty());
    ClearScreen();
    do{
        print("Place an order");
        print("------------");
        print("enter 'quit' to cancel");
        print("");
        print("How much does your cargo weigh?");
        print("");
        weight = GetInputString("Enter weight (in kg): ");
        if(checkForQuit(weight)) return;
        if(!isValidWeight(weight))
        {
            ClearScreen();
            print("You need to enter a valid weight");
        }
    } while (!isValidWeight(weight));
    ClearScreen();
    do{
        print("Place an order");
        print("------------");
        print("enter 'quit' to cancel");
        print("");
        print("What type is your cargo?");
        print("");
        cargoType = GetInputString("Enter type (frozen/fragile/perishable/hazardous) : ");
        if(checkForQuit(cargoType)) return;
        if(!isValidCargoType(cargoType))
        {
            ClearScreen();
            print("You need to enter a valid type");
        }
    } while (!isValidCargoType(cargoType));

    bool response = user.placeOrder(source, destination, dimension, stod(weight), cargoType);
    if(response){
        ClearScreen();
        print("Place an order");
        print("------------");
        print("Order created");
        print("Success, your order has been created and we're looking for a driver.");
        pressEnterToContinue();
        return;
    }
    ClearScreen();
    print("Place an order");
    print("");
    print("We could not process your order, try again later.");
    pressEnterToContinue();
    return;
}

void DisplayCalculateShipping()
{
    Order order;
    ClearScreen();
    do{
        print("Calculate how much it would cost to ship");
        print("------------");
        print("enter 'quit' to cancel");
        print("");
        print("Where is your order coming from?");
        print("");
        order.source = GetInputString("Enter the source: ");
        if(checkForQuit(order.source)) return;
        if(!isValidPlace(order.source))
        {
            ClearScreen();
            print("You need to enter a valid city within the UK");
        }
    } while (!isValidPlace(order.source));
    order.source = convertCityName(order.source);
    ClearScreen();
    do{
        print("Calculate how much it would cost to ship");
        print("------------");
        print("enter 'quit' to cancel");
        print("");
        print("Where is your order going to?");
        print("");
        order.destination = GetInputString("Enter the destination: ");
        if(checkForQuit(order.destination)) return;
        if(!isValidPlace(order.destination))
        {
            ClearScreen();
            print("You need to enter a valid city within the UK");
        }
    } while (!isValidPlace(order.destination));
    order.destination = convertCityName(order.destination);
    ClearScreen();
    do{
        print("Calculate how much it would cost to ship");
        print("------------");
        print("enter 'quit' to cancel");
        print("");
        print("What is the dimension of the cargo you'd like delivered?");
        print("");
        order.cargoDimension = GetInputString("Enter the dimension: ");
        if(checkForQuit(order.cargoDimension)) return;
        if(order.cargoDimension.empty())
        {
            ClearScreen();
            print("You need to enter a dimension");
        }
    } while (order.cargoDimension.empty());
    ClearScreen();
    do{
        print("Calculate how much it would cost to ship");
        print("------------");
        print("enter 'quit' to cancel");
        print("");
        print("How much does your cargo weigh?");
        print("");
        order.cargoWeight = GetInputString("Enter weight (in kg): ");
        if(checkForQuit(order.cargoWeight)) return;
        if(!isValidWeight(order.cargoWeight))
        {
            ClearScreen();
            print("You need to enter a valid weight");
        }
    } while (!isValidWeight(order.cargoWeight));
    ClearScreen();
    do{
        print("Calculate how much it would cost to ship");
        print("------------");
        print("enter 'quit' to cancel");
        print("");
        print("What type is your cargo?");
        print("");
        order.cargoType = GetInputString("Enter type (frozen/fragile/perishable/hazardous) : ");
        if(checkForQuit(order.cargoType)) return;
        if(!isValidCargoType(order.cargoType))
        {
            ClearScreen();
            print("You need to enter a valid type");
        }
    } while (!isValidCargoType(order.cargoType));
    order.distance = distanceBetweenCities(order.source, order.destination);
    double rate = CalculateShippingRate(order);
    print("The shipping rate from " + order.source+" to"+ order.destination +" will cost you £" + to_string(rate));
    pressEnterToContinue();
}

void DisplayCargoInvoices(User& user)
{
    string choice, comment, feedback;
    bool isOrderNotFound = false;
    ClearScreen();
    print("Your Invoices");
    print("------------------------");
    Json::Value data = user.getActiveOrders();
    if (data.isNull()) {
        print("You currently have no Invoices");
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

int CalculateShippingRate(Order& order)
{
    double pencePerMile = 0.03;
    double pencePerKg = 0.25;
    double distance, cargoWeight;

    try
    {
        distance = std::stod(order.distance);
        cargoWeight = std::stod(order.cargoWeight);
    }
    catch (const std::invalid_argument& e)
    {
        // Handle the error, e.g., return -1 or print an error message and exit.
        std::cerr << "Error: Invalid input for distance or cargo weight." << std::endl;
    }

    double shippingRate = (distance * pencePerMile) + (cargoWeight * pencePerKg);
    int roundedShippingRate = static_cast<int>(std::ceil(shippingRate));

    return roundedShippingRate;
}


