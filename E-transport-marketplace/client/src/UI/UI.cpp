//
// Created by Nathan Ellis on 17/02/2023.
//

#include "include/UI/UI.h"
#include "include/Utils.h"
#include <string>
#include "include/Exceptions.h"
#include "include/Order.h"
#include<algorithm>
#include "include/NotificationConnection.h"
#include "thread"

using namespace std;


User user;

void DisplayMainMenu()
{
    print("E-transport marketplace Application");
    print("MAIN MENU");
    print("----------");
    print("Please make your selection");
    print("1 - Login");
    print("2 - Register");
    print("3 - Quit");
}

void DisplayUserProfile() {
    string newFirstName = "";
    string newEmail = "";
    string newLastName = "";
    string newAddress = "";
    string newMobileNumber = "";
    string newCompanyName = "";
    string NewDrivingID = "";
    string newNINumber = "";
    string newCpcNumber= "";
    string newLorryWeight = "";
    string newLorryType, newLorryDimensions, newLorryRegNum;
    bool validEmail = false, validMobileNumber = false, validdriverID = false, validNINumber = false;
    int choice = 0;
    ClearScreen();
    do {
        print("User Profile");
        print("------------");
        print("");
        print("1. First Name: " + user.getFirstName());
        print("2. Last Name: " + user.getLastName());
        print("3. Email: " + user.getEmail());
        print("4. Mobile Number: " + user.getMobileNumber());
        if (user.getAccountType() == 0) {
            print("Account type: Cargo Owner");
        }
        if (user.getAccountType() == 1) {
            print("Account type: Transport Company");
            print("5. Address: " + user.getAddress());
            print("6. Company Name: " + user.getCompanyName());
        }
        if (user.getAccountType() == 2) {
            print("Account type: Driver");
            print("7. National Insurance number: " + user.getNINumber());
            print("8. Driving License number: " + user.getDrivingID());
            print("9. CPC number: " + to_string(user.getCPCNumber()));
            print("10. Lorry type: " + user.getLorryType());
            print("11. Lorry weight: " + to_string(user.getLorryWeight()));
            print("12. Lorry Dimensions: " + user.getLorryDimensions());
            print("13. Lorry Registration Number: " + user.getRegNumber());
        }
        print("");
        print("Enter '14' to go back to the main menu.");
        choice = GetInputInt("What detail would you like to edit? : ", 14);
        switch (choice) {
            case 1:
                // Edit First Name
                ClearScreen();
                do{
                    newFirstName = GetInputString("Enter new First Name: ");
                }while(newFirstName.empty());
                user.updateFirstName(newFirstName);
                break;
            case 2:
                // Edit Last Name
                ClearScreen();
                do{
                    newLastName = GetInputString("Enter new Last Name: ");
                }while(newLastName.empty());
                user.updateLastName(newLastName);
                break;
            case 3:
                // Edit Email
                ClearScreen();
                do {
                    newEmail = GetInputString("Enter new Email: ");
                    validEmail = isValidEmail(newEmail);
                    if (!validEmail) {
                        ClearScreen();
                        print("Your email was not valid, please try again");
                    }
                } while (!validEmail);
                user.updateEmail(newEmail);
                break;
            case 4:
                // Edit Mobile Number
                ClearScreen();
                do {
                    newMobileNumber = GetInputString("Enter new Mobile Number: ");
                    validMobileNumber = isValidMobileNumber(newMobileNumber);
                    if (!validMobileNumber) {
                        ClearScreen();
                        print("Your number was not valid, please try again");
                    }
                } while (!validMobileNumber);
                user.updateMobileNumber(newMobileNumber);
                break;
            case 5:
                // Edit Address
                ClearScreen();
                do{
                    newAddress = GetInputString("Enter new Address: ");
                }while(newAddress.empty());
                user.updateAddress(newAddress);
                break;
            case 6:
                // Edit Company Name
                ClearScreen();
                do{
                    newCompanyName = GetInputString("Enter new Company name: ");
                }while(newCompanyName.empty());
                user.updateCompanyName(newCompanyName);
                break;
            case 7:
                // Edit National Insurance Number
                ClearScreen();
                do {
                    newNINumber = GetInputString("Enter NINumber: ");
                    validNINumber = isValidNiNumber(newNINumber);
                    if (!validNINumber) {
                        ClearScreen();
                        print("Your NINumber was not valid, please try again");
                    }
                } while (!validNINumber);
                user.updateNINumber(newNINumber);
                break;
            case 8:
                // Edit Driving License Number
                ClearScreen();
                do {
                    NewDrivingID = GetInputString("Enter Driver Licence Number: ");
                    validdriverID = isValidDrivingLicenseNumber(NewDrivingID);
                    if (!validdriverID) {
                        ClearScreen();
                        print("Your Driver Licence Number was not valid, please try again");
                    }
                } while (!validdriverID);
                user.updateDrivingID(NewDrivingID);
                break;
            case 9:
                // Edit CPC Number
                ClearScreen();
                do{
                    newCpcNumber = GetInputString("Enter new CPC Number: ");
                }while(newCpcNumber.empty());
                user.updateCPCNumber(stoi(newCpcNumber));
                break;
            case 10:
                // Edit Lorry type
                ClearScreen();
                do{
                    newLorryType = GetInputString("Enter new Lorry Type: ");
                }while(newLorryType.empty());
                user.updateLorryType(newLorryType);
                break;
            case 11:
                // Edit Lorry Weight
                ClearScreen();
                do{
                    newLorryWeight = GetInputString("Enter new Lorry Weight: ");
                }while(!isValidWeight(newLorryWeight));
                user.updateLorryWeight(stod(newLorryWeight));
                break;
            case 12:
                // Edit Lorry Dimensions
                ClearScreen();
                do{
                    newLorryDimensions = GetInputString("Enter new Lorry Dimensions in the format 12x12x12: ");
                }while(!isValidDimensions(newLorryDimensions));
                user.updateLorryDimensions(newLorryDimensions);
                break;
            case 13:
                // Edit Lorry Reg Number
                ClearScreen();
                do{
                    newLorryRegNum = GetInputString("Enter new Lorry Registration Number: ");
                }while(newLorryRegNum.empty());
                user.updateRegNumber(newLorryRegNum);
                break;
            case 14:
                break;
            default:
                ClearScreen();
                print("Not a valid option");

        }
    }while (choice != 14);
}

void DisplayActiveOrders()
{
    string choice;
    bool isOrderNotFound = false;
    ClearScreen();
    print("Active Orders");
    print("------------");
    Json::Value data = user.getActiveOrders();
    if(data.isNull()){
        print("You currently have no active orders");
        pressEnterToContinue();
        return;
    }
    std::vector<Order> orders = parseOrders(data.asString());
    if(orders.empty()) {
        print("We ran into an error getting your orders, try again later");
        GetInputString("Press any key to go back");
        return;
    }
    for (const auto& order : orders) {
        print("Order #"+order.orderID+ " - "+order.source +"->"+order.destination);
    }
    while(true){
        ClearScreen();
        print("Active Orders");
        print("------------");
        for (const auto& order : orders) {
            print("Order #"+order.orderID+ " - "+order.source +"->"+order.destination);
        }
        print("");
        if(isOrderNotFound) {
            print("Order: " + choice + " was not found, please try again.");
            isOrderNotFound = false;
        }
        choice = GetInputString("Enter order ID to get more details or enter 'quit' to go back: ");
        if(checkForQuit(choice)){ return;}
        Order* found_order = findOrderByID(orders, choice);
        if (found_order) {
            string userCreatedOrder = user.getNameByUserID(found_order->userID);
            string driver = user.getNameByUserID(found_order->driverID);
            ClearScreen();
            print("Order " + found_order->orderID + " currently has status of: " + found_order->status);
            print("------------");
            print("Order created by: " + userCreatedOrder);
            print("Order was picked up from: " + found_order->source);
            print("Order is going to: " + found_order->destination);
            print("Distance travelled for order: " + found_order->distance + " miles");
            print("Order is being delivered by: " + (found_order->driverID == "0" ? "Driver has not been assigned yet" : driver));
            print(found_order->isAccepted == "0" ? "Order has not been accepted by a driver yet" : "Order has been accepted");
            print("Cargo Dimension: " + found_order->cargoDimension);
            print("Cargo Type: " + found_order->cargoType);
            print("Cargo Weight (kg): " + found_order->cargoWeight);
            print("Total cost: " + found_order->totalCost);
            print("Cargo Owner Feedback: " + found_order->cargoOwnerComment);
            print("Driver Feedback: " + found_order->driverComment);
            cout << "Completed: " << (found_order->isCompleted == "0" ? "No" : "Yes") << endl;
            print("------------");
            pressEnterToContinue();
        }else{
            isOrderNotFound = true;
        }
    }
}
void DisplayAllOrdersScreen() {

    bool isOrderNotFound = false;
    string choice;
    ClearScreen();
    print("All Orders");
    print("-----------");

    Json::Value data = user.getAllOrders();
    if (data.isNull()) {
        print("There are no orders to display.");
        pressEnterToContinue();
        return;
    }

    std::vector<Order> orders = parseOrders(data.asString());
    if (orders.empty()) {
        print("There was an error retrieving orders. Please try again later.");
        pressEnterToContinue();
        return;
    }

    for (const auto &order: orders) {
        print("Order #" + order.orderID + " - " + order.source + "->" + order.destination);
    }
    while (true) {
        ClearScreen();
        print("All past orders");
        print("------------");
        for (const auto &order: orders) {
            print("Order #" + order.orderID + " - " + order.source + "->" + order.destination);
        }
        print("");
        if (isOrderNotFound) {
            print("Order: " + choice + " was not found, please try again.");
            isOrderNotFound = false;
        }
        choice = GetInputString("Enter order ID to get more details or enter 'quit' to go back: ");
        cout << choice << endl;
        if (checkForQuit(choice)) { return; }
        Order *found_order = findOrderByID(orders, choice);
        if (found_order) {
            string userCreatedOrder = user.getNameByUserID(found_order->userID);
            string driver = user.getNameByUserID(found_order->driverID);
            ClearScreen();
            print("Order " + found_order->orderID + " currently has status of: " + found_order->status);
            print("------------");
            print("Order created by: " + userCreatedOrder);
            print("Order was picked up from: " + found_order->source);
            print("Order is going to: " + found_order->destination);
            print("Distance travelled for order: " + found_order->distance + " miles");
            print("Order is being delivered by: " +
                  (found_order->driverID == "0" ? "Driver has not been assigned yet" : driver));
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
            pressEnterToContinue();
        }else{
            isOrderNotFound = true;
        }
    }
}

void DisplayPastOrdersScreen() {
    string choice, comment, feedback;
    bool isOrderNotFound = false;
    ClearScreen();
    print("Past Orders");
    print("------------");
    Json::Value data = user.getPastOrders();
    if (data.isNull()) {
        print("You currently have no past orders");
        pressEnterToContinue();
        return;
    }
    std::vector<Order> orders = parseOrders(data.asString());
    if (orders.empty()) {
        print("We ran into an error getting your orders, try again later");
        GetInputString("Press any key to go back");
        return;
    }
    for (const auto &order: orders) {
        print("Order #" + order.orderID + " - " + order.source + "->" + order.destination);
    }
    while (true) {
        ClearScreen();
        print("Past Orders");
        print("------------");
        for (const auto &order: orders) {
            print("Order #" + order.orderID + " - " + order.source + "->" + order.destination);
        }
        print("");
        if (isOrderNotFound) {
            print("Order: " + choice + " was not found, please try again.");
            isOrderNotFound = false;
        }
        choice = GetInputString("Enter order ID to get more details or enter 'quit' to go back: ");
        if (checkForQuit(choice)) { return; }
        Order *found_order = findOrderByID(orders, choice);
        if (found_order) {
            string userCreatedOrder = user.getNameByUserID(found_order->userID);
            string driver = user.getNameByUserID(found_order->driverID);
            ClearScreen();
            print("Order " + found_order->orderID + " currently has status of: " + found_order->status);
            print("------------");
            print("Order created by: " + userCreatedOrder);
            print("Order was picked up from: " + found_order->source);
            print("Order is going to: " + found_order->destination);
            print("Distance travelled for order: " + found_order->distance + " miles");
            print("Order is being delivered by: " +
                  (found_order->driverID == "0" ? "Driver has not been assigned yet" : driver));
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
            if (found_order->hasCargoOwnerLeftComment)
            {
                print("You cannot add further feedback to this order. Press enter to continue. ");
                pressEnterToContinue();
                return;
            }
            comment = GetInputString("Enter 1 to share a comment about the driver or 'quit' to go back: ");
            if (checkForQuit(comment)) { return; }
            if (comment == "1") {
                ClearScreen();
                print("Leave feedback about " + driver + " for order ID: " + found_order->orderID);
                print("------------");
                do{
                    feedback = GetInputSentence("Enter your feedback: ");
                }while(feedback.empty());
                // Update the order with the provided feedback
                found_order->cargoOwnerComment = feedback;
                saveFeedback(userCreatedOrder, found_order->orderID, feedback, "Cargo");
                ClearScreen();
                print("Leave feedback about " + driver + " for order ID: " + found_order->orderID);
                print("------------");
                print("");
                print("Thank you for your feedback! Press enter to continue. ");
                pressEnterToContinue();
                return;
            }
        }else{
            isOrderNotFound = true;
        }
    }
}

void DisplayTransportMenu(){
    if (!user.isTransportationCompanyAccountInfoComplete()) {
        CompleteTransportCompanyAccountInfo(user);
        return;
    }
    int choice = 0;
    ClearScreen();
    do
    {
        print("TRANSPORT MENU");
        print("------------");
        print("Welcome, " + user.getFirstName() + ". Please select a function.");
        print("");
        print("1 - View orders waiting to be forwarded to a driver");
        print("2 - View/Manage Profile");
        print("3 - View Order History");
        print("4 - Notifications");
        print("5 - Exit");
        print("");
        choice = GetInputInt("Function: ", 6);
        switch(choice) {
            case 1:
                DisplayOrdersWaitingForDriver(user);
                ClearScreen();
                break;
            case 2:
                DisplayUserProfile();
                ClearScreen();
                break;
            case 3:
                DisplayAllOrdersScreen();
                ClearScreen();
                break;
            case 4:
                DisplayNotifications();
                ClearScreen();
                break;
            case 5:
                ClearScreen();
                choice = 5;
                break;
            default:
                break;
        }
    } while(choice!=5);
    return;
}

void DisplayCargoMenu(){
    int choice = 0;
    ClearScreen();
    do
    {
        print("CARGO MENU");
        print("------------");
        print("Welcome, " + user.getFirstName() + ". Please select a function.");
        print("");
        print("1 - Place Order");
        print("2 - View/Manage Profile");
        print("3 - View active orders");
        print("4 - View Notifications");
        print("5 - View Invoices");
        print("6 - View Past Orders");
        print("7 - Calculate Shipping rates");
        print("8 - Exit");
        print("");
        choice = GetInputInt("Function: ", 8);
        switch(choice) {
            case 1:
                DisplayOrderPlacingScreen(user);
                ClearScreen();
                break;
            case 2:
                DisplayUserProfile();
                ClearScreen();
                break;
            case 3:
                DisplayActiveOrders();
                ClearScreen();
                break;
            case 4:
                DisplayNotifications();
                break;
            case 5:
                DisplayCargoInvoices(user);
                ClearScreen();
                break;
            case 6:
                DisplayPastOrdersScreen();
                ClearScreen();
                break;
            case 7:
                DisplayCalculateShipping();
                ClearScreen();
                break;
            case 8:
                ClearScreen();
                choice = 8;
                break;
            default:
                break;
        }
    } while(choice!=8);
    return;
}

void DisplayDriverMenu(){
    if (!user.isDriverAccountInfoComplete()) {
        CompleteDriverAccountInfo(user);
        return;
    }
    int choice = 0;
    ClearScreen();
    do
    {
        print("DRIVER MENU");
        print("------------");
        print("Welcome, " + user.getFirstName() + ". Please select a function.");
        print("");
        print("1 - View Available Orders");
        print("2 - View/Manage Profile");
        print("3 - View your active orders");
        print("4 - View Shipment History");
        print("5 - View Notifications");
        print("6 - Exit");
        print("");
        choice = GetInputInt("Function: ", 6);
        switch(choice) {
            case 1:
                DisplayAvailableOrders(user);
                ClearScreen();
                break;
            case 2:
                DisplayUserProfile();
                ClearScreen();
                break;
            case 3:
                DisplayDriverActiveOrders(user);
                break;
            case 4:
                DisplayDriverShipmentHistory(user);
                ClearScreen();
                break;
            case 5:
                DisplayNotifications();
                ClearScreen();
                break;
            case 6:
                ClearScreen();
                choice = 6;
                break;
            default:
                break;
        }
    } while(choice!=6);
    return;
}

void DisplayNotifications()
{
    ClearScreen();
    print("Notifications");
    print("------------------------");
    Json::Value data = user.getNotifications();
    if (data.isNull()) {
        print("There's currently no notifications.");
        pressEnterToContinue();
        ClearScreen();
        return;
    }
    std::vector<Notification> notifications = parseNotifications(data.asString());
    if (notifications.empty()) {
        print("We ran into an error getting notifications, try again later");
        print("Press enter to go back");
        pressEnterToContinue();
        return;
    }
    print("");
    print("");
    for (const auto &notification : notifications) {
        print(notification.notificationTitle);
        print("-----------------");
        print(notification.notificationBody);
        print("sent at " + notification.createdAt);
    }
    print("");
    pressEnterToContinue();
}

void DisplayLoginMenu()
{
    ClearScreen();
    string email, password;
    do {
        print("-----------LOGIN-----------");
        print("enter 'quit' to go back\n");
        email = GetInputString("Enter your Email: ");
        if(checkForQuit(email)) return;
        password = GetInputString("Enter your Password: ");
        if(checkForQuit(password)) return;
    } while (!ValidateLogin(email, password, user));
    ClearScreen();

    std::condition_variable notificationFinishedCond;
    std::mutex notificationMutex;
    bool notificationFinished = false;
    std::unique_lock<std::mutex> lock(notificationMutex);

    std::thread notificationThread([&](){
        NotificationConnection* notificationConnection = NotificationConnection::getInstance();
        notificationConnection->sendInitalInfo(to_string(user.getUserID()));
        notificationConnection->receiveNotifications();
        notificationFinished = true;
        notificationFinishedCond.notify_one();
    });
    switch (user.getAccountType()) {
        case 0:
            DisplayCargoMenu();
            break;
        case 1:
            DisplayTransportMenu();
            break;
        case 2:
            DisplayDriverMenu();
            break;
        default:
            break;
    }
    notificationFinishedCond.wait(lock, [&](){ return notificationFinished; });
    notificationThread.join();

}

void DisplayRegisterMenu()
{
    ClearScreen();
    string email, password, confirmPassword, userType, firstName, lastName, mobileNumber;
    bool validEmail = false, validPassword = false, validAccountType = false, validMobileNumber = false;
    do{
        print("----------REGISTER----------");
        print("enter 'quit' to cancel");
        print("");
        print("To start, we need your email.");
        print("");
        email = GetInputString("Enter your email: ");
        if(checkForQuit(email)) return;
        validEmail = isValidEmail(email);
        if(!validEmail)
        {
            ClearScreen();
            print("Your email was not valid, please try again");
        }

    } while (!validEmail);

    // Password
    ClearScreen();
    do{
        print("----------REGISTER----------");
        print("enter 'quit' to cancel");
        print("");
        print("Next, your password. It must include 7 or more characters, a special character, and numbers.");
        print("");
        password = GetInputString("Enter a new Password: ");
        confirmPassword = GetInputString("Enter the password again: ");
        if(checkForQuit(password)) return;
        validPassword = isValidPassword(password, confirmPassword);
        if(!validPassword)
        {
            ClearScreen();
            print("Your password did not meet the required requirements, please try again.");
        }
    }while (!validPassword);

    // Account type
    ClearScreen();
    do{
        print("----------REGISTER----------");
        print("enter 'quit' to cancel");
        print("");
        print("Now, please enter out of the following three options, the type of account you'd like to create:");
        print("");
        print("'0' - Create a Cargo Owner account");
        print("'1' - Create a Transportation Company account");
        print("'2' - Create a Driver account");
        userType = GetInputString("Enter your select account type: ");
        if(checkForQuit(userType)) return;
        validAccountType = isValidAccountType(userType);
        if(!validAccountType)
        {
            ClearScreen();
            print("Please select a valid account type");
        }
    }while(!validAccountType);

    // First and Last Name
    ClearScreen();
    print("----------REGISTER----------");
    print("enter 'quit' to cancel");
    print("");
    print("Next, we need your first and last name");
    print("");
    do {
        firstName = GetInputString("Enter your first name: ");
        if(checkForQuit(firstName)) return;
    }while (firstName.empty());
    do {
        lastName = GetInputString("Enter your last name: ");
        if(checkForQuit(lastName)) return;
    }while (lastName.empty());

    // Mobile number
    ClearScreen();
    print("----------REGISTER----------");
    print("enter 'quit' to cancel");
    print("");
    print("Next, we need your mobile number");
    print("");
    do {
        mobileNumber = GetInputString("Enter your mobile number: ");
        if(checkForQuit(mobileNumber)) return;
        validMobileNumber = isValidMobileNumber(mobileNumber);
        if(!validMobileNumber){
            ClearScreen();
            print("Please enter a valid mobile number");
        }
    }while (!validMobileNumber);

    // We now have an email, password and userType.
    user.Register(email, password, userType, firstName, lastName, mobileNumber);
}
