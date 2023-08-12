#pragma once
#include <string>
#include "json/value.h"
#include "json/value.h"
#include "include/SocketConnection.h"

/**
 * @file User.h
 * @brief This file contains the User class and private functions to update values.
 */


using namespace std;
/**
 * @brief The User class represents a user account and provides methods to update the user's profile and interact with the system.
 */
class User {
public:
    /**
     * @brief Default constructor for User class
     */
    User() {}

    /**
     * @brief Getter method for user ID
     * @return The user ID
     */
    int getUserID() const { return m_userID; }

    /**
     * @brief Getter method for account type
     * @return The account type
     */
    int getAccountType() const { return m_accountType; }

    /**
     * @brief Getter method for first name
     * @return The first name
     */
    std::string getFirstName() const { return m_first_name; }

    /**
     * @brief Getter method for last name
     * @return The last name
     */
    std::string getLastName() const { return m_last_name; }

    /**
     * @brief Getter method for email
     * @return The email
     */
    std::string getEmail() const { return m_email; }

    /**
     * @brief Getter method for mobile number
     * @return The mobile number
     */
    std::string getMobileNumber() const { return m_mobileNumber; }

    /**
     * @brief Getter method for national insurance number
     * @return The national insurance number
     */
    std::string getNINumber() const { return m_NINumber; }

    /**
     * @brief Getter method for address
     * @return The address
     */
    std::string getAddress() const { return m_address; }

    /**
     * @brief Getter method for company name
     * @return The company name
     */
    std::string getCompanyName() const { return m_companyName; }

    /**
     * @brief Getter method for driving ID
     * @return The driving ID
     */
    std::string getDrivingID() const { return m_drivingID; }

    /**
     * @brief Getter method for CPC number
     * @return The CPC number
     */
    int getCPCNumber() const { return m_cpcNumber; }

    /**
     * @brief Getter method for lorry type
     * @return The lorry type
     */
    std::string getLorryType() const { return m_lorryType; }

    /**
     * @brief Getter method for lorry weight
     * @return The lorry weight
     */
    double getLorryWeight() const { return m_lorryWeight; }

    /**
     * @brief Getter method for lorry dimensions
     * @return The lorry dimensions
     */
    std::string getLorryDimensions() const { return m_lorryDimensions; }

    /**
     * @brief Getter method for registration number
     * @return The registration number
     */
    std::string getRegNumber() const { return m_regNumber; }

    /**
     * @brief Checks if driver account information is complete
     * @return true if complete, false otherwise
     */
    bool isDriverAccountInfoComplete() const;

    /**
     * @brief Checks if transportation company account information is complete
     * @return true if complete, false otherwise
     */
    bool isTransportationCompanyAccountInfoComplete() const;

    /**
    * @brief Updates the first name
    * @param firstName The new first name
    */
    void updateFirstName(const std::string& firstName) {
        m_first_name = firstName;
        updateDatabase("firstName", firstName);
    }

    /**
  * @brief Update the user's last name.
  *
  * @param lastName The new last name to set.
  */
    void updateLastName(const string& lastName) {
        m_last_name = lastName;
        updateDatabase("lastName", lastName);
    }

    /**
     * @brief Update the user's email.
     *
     * @param email The new email to set.
     */
        void updateEmail(const string& email) {
            m_email = email;
            updateDatabase("email", email);
        }

    /**
     * @brief Update the user's mobile number.
     *
     * @param mobileNumber The new mobile number to set.
     */
        void updateMobileNumber(const string& mobileNumber) {
            m_mobileNumber = mobileNumber;
            updateDatabase("mobileNumber", mobileNumber);
        }

    /**
     * @brief Update the user's National Insurance Number.
     *
     * @param NINumber The new National Insurance Number to set.
     */
        void updateNINumber(const string& NINumber) {
            m_NINumber = NINumber;
            updateDatabase("NINumber", NINumber);
        }

    /**
     * @brief Update the user's address.
     *
     * @param address The new address to set.
     */
        void updateAddress(const string& address) {
            m_address = address;
            updateDatabase("address", address);
        }

    /**
     * @brief Update the company name associated with the user's account.
     *
     * @param companyName The new company name to set.
     */
        void updateCompanyName(const string& companyName) {
            m_companyName = companyName;
            updateDatabase("companyName", companyName);
        }

    /**
     * @brief Update the driving ID of the user.
     *
     * @param drivingID The new driving ID to set.
     */
        void updateDrivingID(const string& drivingID) {
            m_drivingID = drivingID;
            updateDatabase("drivingID", drivingID);
        }

    /**
     * @brief Update the CPC number of the user.
     *
     * @param cpcNumber The new CPC number to set.
     */
        void updateCPCNumber(int cpcNumber) {
            m_cpcNumber = cpcNumber;
            updateDatabase("cpcNumber", cpcNumber);
        }

    /**
     * @brief Update the lorry type associated with the user's account.
     *
     * @param lorryType The new lorry type to set.
     */
        void updateLorryType(const string& lorryType) {
            m_lorryType = lorryType;
            updateDatabase("lorryType", lorryType);
        }

    /**
     * @brief Update the weight of the lorry associated with the user's account.
     *
     * @param lorryWeight The new weight to set.
     */
        void updateLorryWeight(double lorryWeight) {
            m_lorryWeight = lorryWeight;
            updateDatabase("lorryWeight", lorryWeight);
        }

    /**
     * @brief Update the dimensions of the lorry associated with the user's account.
     *
     * @param lorryDimensions The new dimensions to set.
     */
        void updateLorryDimensions(const string& lorryDimensions) {
            m_lorryDimensions = lorryDimensions;
            updateDatabase("lorryDimension", lorryDimensions);
        }

    /**
     * @brief Update the registration number of the lorry associated with the user's account.
     *
     * @param regNumber The new registration number to set.
     */
        void updateRegNumber(const string& regNumber) {
            m_regNumber = regNumber;
            updateDatabase("regNumber", regNumber);
        }

        /**
     * @brief Checks if the account type is equal to 0.
     *
     * @return Returns true if the account type is 0, otherwise false.
     */
        bool canPlaceOrder() const{ return m_accountType == 0;}

    /**
     * @brief Logs in the user with the provided email and password.
     *
     * @param email The email of the user.
     * @param password The password of the user.
     *
     * @return Returns an integer value based on the result of the login process.
     */
        int Login(string email, string password);

    /**
     * @brief Registers a new user with the provided details.
     *
     * @param email The email of the user.
     * @param password The password of the user.
     * @param userType The type of user being registered.
     * @param firstName The first name of the user.
     * @param lastName The last name of the user.
     * @param mobileNumber The mobile number of the user.
     *
     * @return Returns an integer value based on the result of the registration process.
     */
        int Register(string email, string password, string userType, string firstName, string lastName, string mobileNumber);

    /**
     * @brief Places an order with the provided details.
     *
     * @param source The source of the order.
     * @param destination The destination of the order.
     * @param cargoDimension The dimension of the cargo in the order.
     * @param cargoWeight The weight of the cargo in the order.
     * @param cargoType The type of cargo in the order.
     *
     * @return Returns true if the order was successfully placed, otherwise false.
     */
        bool placeOrder(string source, string destination, string cargoDimension, double cargoWeight, string cargoType);

    /**
     * @brief Gets all the active orders.
     *
     * @return Returns a JSON object containing all the active orders.
     */
        Json::Value getActiveOrders();

    /**
     * @brief Gets all the active orders for a driver.
     *
     * @return Returns a JSON object containing all the active orders for the driver.
     */
        Json::Value getDriverActiveOrders();

    /**
     * @brief Gets all the past orders.
     *
     * @return Returns a JSON object containing all the past orders.
     */
        Json::Value getPastOrders();

    /**
     * @brief Gets all the orders.
     *
     * @return Returns a JSON object containing all the orders.
     */
        Json::Value getAllOrders();

    /**
     * @brief Gets the name of the user by the user ID.
     *
     * @param userID The ID of the user.
     *
     * @return Returns the name of the user.
     */
        string getNameByUserID(string userID);

    /**
     * @brief Gets the shipment history.
     *
     * @return Returns a JSON object containing the shipment history.
     */
        Json::Value getShipmentHistory();

    /**
     * @brief Gets all the available orders.
     *
     * @return Returns a JSON object containing all the available orders.
     */
        Json::Value getAvailableOrders();

    /**
     * @brief Accepts an order with the provided order ID and user ID.
     *
     * @param orderID The ID of the order.
     * @param userID The ID of the user.
     *
     * @return Returns true if the order was successfully accepted, otherwise false.
     */
        bool acceptOrder(string orderID, string userID);

    /**
     * @brief Declines an order with the provided order ID.
     *
     * @param orderID The ID of the order.
     *
     * @return Returns true if the order was successfully declined, otherwise false.
     */
        bool declineOrder(string orderID);

    /**
     * @brief Gets all the invoices.
     *
     * @return Returns a JSON object containing all the invoices.
     */
        /**
     * @brief Gets all the invoices.
     *
     * @return Returns a JSON object containing all the invoices.
     */
        Json::Value getInvoices();

    /**
     * @brief Gets all the orders waiting for a driver.
     *
     * @return Returns a JSON object containing all the orders waiting for a driver.
     */
        Json::Value getOrdersWaitingForDriver();

    /**
     * @brief Gets all the available drivers for the provided city.
     *
     * @param city The city for which the available drivers are required.
     *
     * @return Returns a JSON object containing all the available drivers for the city.
     */
        Json::Value getAvailableDrivers(const string& city);

    /**
     * @brief Forwards an order with the provided order ID to the provided driver ID.
     *
     * @param orderID The ID of the order.
     * @param driverID The ID of the driver.
     *
     * @return Returns true if the order was successfully forwarded, otherwise false.
     */
        bool forwardOrder(string orderID, string driverID);

    /**
     * @brief Updates the status of the order with the provided status, order ID, and driver ID.
     *
     * @param status The new status of the order.
     * @param orderID The ID of the order.
     * @param driverID The ID of the driver.
     *
     * @return Returns true if the status was successfully updated, otherwise false.
     */
        bool updateOrderStatus(const string& status, const string& orderID, const string& driverID);

    /**
     * @brief Gets all the notifications.
     *
     * @return Returns a JSON object containing all the notifications.
     */
        Json::Value getNotifications();



private:
        /**
     * @brief The user ID.
     */
        int m_userID;

    /**
     * @brief The account type.
     */
        int m_accountType;

    /**
     * @brief The first name.
     */
        string m_first_name;

    /**
     * @brief The last name.
     */
        string m_last_name;

    /**
     * @brief The email.
     */
        string m_email;

    /**
     * @brief The mobile number.
     */
        string m_mobileNumber;

    /**
     * @brief The NI number.
     */
        string m_NINumber;

    /**
     * @brief The address.
     */
        string m_address;

    /**
     * @brief The company name.
     */
        string m_companyName;

    /**
     * @brief The driving ID.
     */
        string m_drivingID;

    /**
     * @brief The CPC number.
     */
        int m_cpcNumber;

    /**
     * @brief The lorry type.
     */
        string m_lorryType;

    /**
     * @brief The lorry weight.
     */
        double m_lorryWeight;

    /**
     * @brief The lorry dimensions.
     */
        string m_lorryDimensions;

    /**
     * @brief The registration number.
     */
        string m_regNumber;

    /**
     * @brief Sets the user ID.
     *
     * @param userID The user ID.
     */
        void setUserID(int userID) { m_userID = userID; }

    /**
     * @brief Sets the account type.
     *
     * @param accountType The account type.
     */
        void setAccountType(int accountType) { m_accountType = accountType; }

    /**
     * @brief Sets the first name.
     *
     * @param firstName The first name.
     */
        void setFirstName(const string& firstName) { m_first_name = firstName; }

    /**
     * @brief Sets the last name.
     *
     * @param lastName The last name.
     */
        void setLastName(const string& lastName) { m_last_name = lastName; }

    /**
     * @brief Sets the email.
     *
     * @param email The email.
     */
        void setEmail(const string& email) { m_email = email; }

    /**
     * @brief Sets the mobile number.
     *
     * @param mobileNumber The mobile number.
     */
        void setMobileNumber(const string& mobileNumber) { m_mobileNumber = mobileNumber; }

    /**
     * @brief Sets the NI number.
     *
     * @param NINumber The NI number.
     */
        void setNINumber(const string& NINumber) { m_NINumber = NINumber; }

    /**
     * @brief Sets the address.
     *
     * @param address The address.
     */
        void setAddress(const string& address) { m_address = address; }

    /**
     * @brief Sets the company name.
     *
     * @param companyName The company name.
     */
        void setCompanyName(const string& companyName) { m_companyName = companyName; }

    /**
     * @brief Sets the driving ID.
     *
     * @param drivingID The driving ID.
     */
        void setDrivingID(const string& drivingID) { m_drivingID = drivingID; }

    /**
     * @brief Sets the CPC number.
     *
     * @param cpcNumber The CPC number.
     */
        void setCPCNumber(int cpcNumber) { m_cpcNumber = cpcNumber; }

    /**
     * @brief Sets the lorry type.
     *
     * @param lorryType The lorry type.
     */
        void setLorryType(const string& lorryType) { m_lorryType = lorryType; }

    /**
     * @brief Sets the lorry weight.
     *
     * @param lorryWeight The lorry weight.
     */
        void setLorryWeight(double lorryWeight) { m_lorryWeight = lorryWeight; }

    /**
     * @brief Sets the lorry dimensions.
     *
     * @param lorryDimensions The lorry dimensions.
     */
        void setLorryDimensions(const string& lorryDimensions) { m_lorryDimensions = lorryDimensions; }

    /**
     * @brief Sets the lorry's Registration number.
     *
     * @param regNumber The lorry's Registration number.
     */
        void setRegNumber(const string& regNumber) { m_regNumber = regNumber; }

    SocketConnection* connection = SocketConnection::getInstance();

    /**
  * @brief Updates a value in the user's profile in the database.
  *
  * @param columnName The name of the column to update.
  * @param value The new value for the specified column.
  */
    void updateDatabase(const string& columnName, const string& value) {
        Json::Value updatePayload;
        updatePayload["userID"] = m_userID;
        updatePayload["valueToBeUpdated"] = columnName;
        updatePayload["value"] = value;
        connection->sendRequest("POST", "/updateProfile", updatePayload.toStyledString());
    }

    /**
     * @brief Updates an integer value in the user's profile in the database.
     *
     * @param columnName The name of the column to update.
     * @param value The new integer value for the specified column.
     */
        void updateDatabase(const string& columnName, int value) {
            Json::Value updatePayload;
            updatePayload["userID"] = m_userID;
            updatePayload["valueToBeUpdated"] = columnName;
            updatePayload["value"] = value;
            connection->sendRequest("POST", "/updateProfile", updatePayload.toStyledString());
        }

};