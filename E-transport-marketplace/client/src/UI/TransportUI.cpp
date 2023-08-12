//
// Created by Nathan Ellis on 09/04/2023.
//

#include "include/UI/UI.h"
#include "include/Utils.h"


void CompleteTransportCompanyAccountInfo(User& user) {
    ClearScreen();
    print("Your account information is incomplete.");
    print("Please enter the following details to continue:");

    string companyName, companyAddress;

    do {
        companyName = GetInputString("Enter your company name: ");
        if(companyName.empty())
        {
            print("Please enter a valid Company name");
        }
    } while (companyName.empty());

    do {
        companyAddress = GetInputString("Enter your company address: ");
        if(!isValidPlace(companyAddress))
        {
            print("Please enter a valid city within the UK");
        }
    } while (!isValidPlace(companyAddress));

    user.updateCompanyName(companyName);
    user.updateAddress(companyAddress);
    print("Thank you for your details, you may continue. Press enter when you are ready");
    pressEnterToContinue();
    DisplayTransportMenu();
}

void DisplayAvailableDrivers(User& user, Order *found_order)
{
    string driverChoice, acceptChoice;
    Driver *found_driver;
    Json::Value driversAvailable = user.getAvailableDrivers(found_order->source);
    if (driversAvailable.isNull()) {
        print("There's currently no drivers available to pickup this order - would you like to expand the search?.");
        pressEnterToContinue();
        ClearScreen();
        return;
    }
    std::vector<Driver> drivers = parseDrivers(driversAvailable.asString());
    if (drivers.empty()) {
        print("We ran into an error getting available orders, try again later");
        print("Press enter to go back");
        pressEnterToContinue();
        return;
    }
    for (const auto &driver : drivers) {
        print(" * Driver (#" +driver.driverID+ ") " + user.getNameByUserID(driver.driverID) + " is available to pickup from "+ found_order->source + ".");
    }
    print("");
    do{
        driverChoice = GetInputString("Select a driver out of the list, or enter 'quit' to go back: ");
        if(checkForQuit(driverChoice)) { return; }
        found_driver = findDriverByID(drivers, driverChoice);
        if(found_driver)
        {
            ClearScreen();
            print("Order placed by " + user.getNameByUserID(found_order->userID) + " is awaiting pickup from " + found_order->source + " and needs to be delivered to " + found_order->destination);
            print("The cargo to be delivered is " + found_order->cargoWeight + "KG");
            print("The cargo dimension is " + found_order->cargoDimension + " and the drivers lorry's dimension is " + found_driver->lorryDimension);
            print("");
            acceptChoice = GetInputString("Do you want to forward this order to " + found_driver->firstName + "? Y/N: ");
            if(acceptChoice == "Y" || acceptChoice == "y")
            {
                if(user.forwardOrder(found_order->orderID, found_driver->driverID))
                {
                    ClearScreen();
                    print("Order has been forwarded to driver " + found_driver->firstName + " successfully.");
                    pressEnterToContinue();
                    return;
                }
                print("We could not forward the order to the driver, maybe the driver has taken up an order or the order no longer exists.");
                pressEnterToContinue();
                return;
            }
            return;
        }else{
            print("Please enter a valid Driver ID to continue.");
        }
    }while(!found_driver);
}

void DisplayOrdersWaitingForDriver(User& user) {
    string choice;
    print("");
    while(true) {
        ClearScreen();
        print("Orders waiting for a driver");
        print("------------------------");
        print("Please choose an order and select a free driver to take up the job");
        print("Note: Orders that are rejected by drivers will reappear here until a driver accepts it");
        print("------------------------");
        print("");
        Json::Value data = user.getOrdersWaitingForDriver();
        if (data.isNull()) {
            print("There's currently no orders available for you to manage.");
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
        if (checkForQuit(choice)) { return; }
        for (const auto &order: orders) {
            print("Order #" + order.orderID + " - " + order.source + "->" + order.destination);
        }
        string driverChoice;
        Order *found_order;
        do {
            choice = GetInputString("Please select an order (by ID) to continue or enter 'quit' to go back: ");
            if (checkForQuit(choice)) { return; }
            found_order = findOrderByID(orders, choice);
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
                print("Cargo Dimension: " + found_order->cargoDimension);
                print("Cargo Type: " + found_order->cargoType);
                print("Cargo Weight (kg): " + found_order->cargoWeight);
                print("Total cost: " + found_order->totalCost);
                print("------------");
                print("");
                print("");
                print("Below is a list of drivers that are free and near the pickup location: ");
                print("");
                DisplayAvailableDrivers(user, found_order);
            } else {
                print("Please enter a valid Order ID to continue.");
            }
        } while (!found_order);
    }
}