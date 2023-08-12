//
// Created by Nathan Ellis on 16/02/2023.
//

#include "include/User.h"
#include "include/Utils.h"
#include <iostream>
#include <string>
#include "json/json.h"
#include "include/Exceptions.h"
#include "include/SocketConnection.h"

using namespace std;

int User::Login(string email, string password)
{
    // Send a POST request to /login with valid credentials
    string payload = "{\"email\":" + string("\"") + email + "\"," "\"password\":" + "\"" + password + "\"}";
    try {
        SocketConnection* connection = SocketConnection::getInstance();
        std::string response = connection->sendRequest("POST", "/login", payload);
        Json::Value responseCode = getJsonValueForKey(response, "code");
        if(responseCode.asString() != "200") return 1; // Server responded with unauthorised error code, wrong credentials.
        Json::Value userData = getJsonValueForKey(response, "data");
        setUserID(stoi(userData["id"].asString()));
        setAccountType(stoi(userData["accountType"].asString()));
        setFirstName(userData["firstName"].asString());
        setLastName(userData["lastName"].asString());
        setEmail(userData["email"].asString());
        setMobileNumber(userData["mobileNumber"].asString());
        setNINumber(userData["NINumber"].asString());  // Driver
        setAddress(userData["address"].asString()); // Transportation Company
        setCompanyName(userData["companyName"].asString()); // Transportation Company
        setDrivingID(userData["drivingID"].asString()); // Driver
        setCPCNumber(stoi(userData["cpcNumber"].asString()));  // Driver
        setLorryType(userData["lorryType"].asString());  // Driver
        setLorryWeight(stod(userData["lorryWeight"].asString()));  // Driver
        setLorryDimensions(userData["lorryDimensions"].asString());  // Driver
        setRegNumber(userData["regNumber"].asString());  // Driver
        return 0;
    }catch (const JSONException* e) {
       throw new JSONException("We've ran into an error logging you in, please try again." + string(e->what()));
    }catch (const ConnectionException* e){
        throw new ConnectionException("We could not connect to the server, is it running?");
    }
}

int User::Register(string email, string password, string userType, string firstName, string lastName, string mobileNumber)
{
    Json::Value payload;
    payload["email"] = email;
    payload["password"] = password;
    payload["accountType"] = userType;
    payload["firstName"] = firstName;
    payload["lastName"] = lastName;
    payload["mobileNumber"] = mobileNumber;
    try{
        SocketConnection* connection = SocketConnection::getInstance();
        std::string response = connection->sendRequest("POST", "/register", payload.toStyledString());
        ClearScreen();
    }catch (const ConnectionException* e){
        throw new ConnectionException("We could not connect to the server, is it running?");
    }
    return 0;
}

Json::Value User::getAllOrders()
{
    try{
        SocketConnection* connection = SocketConnection::getInstance();
        std::string response = connection->sendRequest("GET", "/allOrders", "");
        Json::Value responseCode = getJsonValueForKey(response, "code");
        Json::Value responseJson = convertStringToJson(response);
        if(responseCode.asString() != "200") {return Json::Value::null; }
        return responseJson["data"];
    }catch (const ConnectionException* e){
        throw new ConnectionException("We could not connect to the server, is it running?");
    }
}

bool User::placeOrder(string source, string destination, string cargoDimension, double cargoWeight, string cargoType)
{
    Json::Value orderPayload;
    orderPayload["userPlacingOrder"] = m_userID;
    orderPayload["source"] = source;
    orderPayload["destination"] = destination;
    orderPayload["cargoDimension"] = cargoDimension;
    orderPayload["cargoWeight"] = cargoWeight;
    orderPayload["cargoType"] = cargoType;

    if(!canPlaceOrder()) return false;

    try{
        SocketConnection* connection = SocketConnection::getInstance();
        std::string response = connection->sendRequest("POST", "/placeorder", orderPayload.toStyledString());
        Json::Value responseCode = getJsonValueForKey(response, "code");
        if(responseCode.asString() == "200") {return true; }
        return false;
    }catch (const ConnectionException* e){
        throw new ConnectionException("We could not connect to the server, is it running?");
    }
}

Json::Value User::getInvoices()
{
    Json::Value invoicesPayload;
    invoicesPayload["userID"] = m_userID;
    try
    {
        SocketConnection* connection = SocketConnection::getInstance();
        string responseString = connection->sendRequest("GET", "/invoices", invoicesPayload.toStyledString());
        Json::Value responseJson = convertStringToJson(responseString);
        if(responseJson["code"].asString() == "204") return Json::Value::null;
        return responseJson["data"];
    }catch(const ConnectionException* e)
    {
        throw ConnectionException("We could not fetch active orders at the moment");
    }catch (const JSONException* e){
        cout << e->what() <<endl;
    }
}

Json::Value User::getActiveOrders()
{
    Json::Value activeOrderPayload;
    activeOrderPayload["userID"] = m_userID;
    try
    {
        SocketConnection* connection = SocketConnection::getInstance();
        string responseString = connection->sendRequest("GET", "/activeOrders", activeOrderPayload.toStyledString());
        Json::Value responseJson = convertStringToJson(responseString);
        if(responseJson["code"].asString() == "204") return Json::Value::null;
        return responseJson["data"];
    }catch(const ConnectionException* e)
    {
        throw ConnectionException("We could not fetch active orders at the moment");
    }catch (const JSONException* e){
        cout << e->what() <<endl;
    }
}

Json::Value User::getDriverActiveOrders()
{
    Json::Value activeOrderPayload;
    activeOrderPayload["userID"] = m_userID;
    try
    {
        SocketConnection* connection = SocketConnection::getInstance();
        string responseString = connection->sendRequest("GET", "/driverActiveOrders", activeOrderPayload.toStyledString());
        Json::Value responseJson = convertStringToJson(responseString);
        if(responseJson["code"].asString() == "204") return Json::Value::null;
        return responseJson["data"];
    }catch(const ConnectionException* e)
    {
        throw ConnectionException("We could not fetch active orders at the moment");
    }catch (const JSONException* e){
        cout << e->what() <<endl;
    }
}

Json::Value User::getNotifications()
{
    Json::Value notifications;
    notifications["userID"] = m_userID;
    notifications["isTransportAccount"] = "0";
    if(m_accountType == 1){ notifications["isTransportAccount"] = "1";}
    try
    {
        SocketConnection* connection = SocketConnection::getInstance();
        string responseString = connection->sendRequest("GET", "/notifications", notifications.toStyledString());
        Json::Value responseJson = convertStringToJson(responseString);
        if(responseJson["code"].asString() != "200") return Json::Value::null;
        return responseJson["data"];
    }catch(const ConnectionException* e)
    {
        throw ConnectionException("We could not update the order status at the moment");
    }catch (const JSONException* e){
        cout << e->what() <<endl;
    }
}

bool User::updateOrderStatus(const string& status, const string& orderID, const string& driverID)
{
    Json::Value orderStatus;
    orderStatus["status"] = status;
    orderStatus["orderID"] = orderID;
    orderStatus["driverID"] =driverID;
    try
    {
        SocketConnection* connection = SocketConnection::getInstance();
        string responseString = connection->sendRequest("POST", "/updateOrderStatus", orderStatus.toStyledString());
        Json::Value responseJson = convertStringToJson(responseString);
        if(responseJson["code"].asString() != "200") return false;
        return true;
    }catch(const ConnectionException* e)
    {
        throw ConnectionException("We could not update the order status at the moment");
    }catch (const JSONException* e){
        cout << e->what() <<endl;
    }
}

Json::Value User::getPastOrders()
{
    Json::Value pastOrderPayload;
    pastOrderPayload["userID"] = m_userID;
    try
    {
        SocketConnection* connection = SocketConnection::getInstance();
        string responseString = connection->sendRequest("GET", "/pastOrders", pastOrderPayload.toStyledString());
        Json::Value responseJson = convertStringToJson(responseString);
        if(responseJson["code"].asString() == "204") return Json::Value::null;
        return responseJson["data"];
    }catch(const ConnectionException* e)
    {
        throw ConnectionException("We could not fetch past orders at the moment");
    }catch (const JSONException* e){
        cout << e->what() <<endl;
    }
}

bool User::acceptOrder(string orderID, string userID)
{
    Json::Value acceptOrder;
    acceptOrder["orderID"] = orderID;
    acceptOrder["driverID"] = m_userID;
    acceptOrder["userID"] = userID;
    if(m_accountType != 2) return false;
    try
    {
        SocketConnection* connection = SocketConnection::getInstance();
        string responseString = connection->sendRequest("POST", "/acceptOrder", acceptOrder.toStyledString());
        Json::Value responseJson = convertStringToJson(responseString);
        if(responseJson["code"].asString() == "200") return true;
        return false;
    }catch(const ConnectionException* e)
    {
        throw ConnectionException("We could not accept this order for you at the moment.");
    }catch (const JSONException* e){
        cout << e->what() <<endl;
    }
}

bool User::declineOrder(string orderID)
{
    Json::Value declineOrder;
    declineOrder["orderID"] = orderID;
    declineOrder["driverID"] = m_userID;
    if(m_accountType != 2) return false;
    try
    {
        SocketConnection* connection = SocketConnection::getInstance();
        string responseString = connection->sendRequest("POST", "/declineOrder", declineOrder.toStyledString());
        Json::Value responseJson = convertStringToJson(responseString);
        if(responseJson["code"].asString() == "200") return true;
        return false;
    }catch(const ConnectionException* e)
    {
        throw ConnectionException("We could not accept this order for you at the moment.");
    }catch (const JSONException* e){
        cout << e->what() <<endl;
    }
}

bool User::forwardOrder(string orderID, string driverID)
{
    Json::Value fordwardOrder;
    fordwardOrder["orderID"] = orderID;
    fordwardOrder["driverID"] = driverID;
    if(m_accountType != 1) return false;
    try
    {
        SocketConnection* connection = SocketConnection::getInstance();
        string responseString = connection->sendRequest("POST", "/forwardOrder", fordwardOrder.toStyledString());
        Json::Value responseJson = convertStringToJson(responseString);
        if(responseJson["code"].asString() == "200") return true;
        return false;
    }catch(const ConnectionException* e)
    {
        throw ConnectionException("We could not accept this order for you at the moment.");
    }catch (const JSONException* e){
        cout << e->what() <<endl;
    }
}

Json::Value User::getAvailableOrders()
{
    Json::Value availableOrders;
    availableOrders["driverID"] = m_userID;
    if(m_accountType != 2) return Json::Value::null;
    try
    {
        SocketConnection* connection = SocketConnection::getInstance();
        string responseString = connection->sendRequest("GET", "/availableOrders", availableOrders.toStyledString());
        Json::Value responseJson = convertStringToJson(responseString);
        if(responseJson["code"].asString() == "204") return Json::Value::null;
        return responseJson["data"];
    }catch(const ConnectionException* e)
    {
        throw ConnectionException("We could not fetch any available orders at the moment.");
    }catch (const JSONException* e){
        cout << e->what() <<endl;
    }
}

Json::Value User::getOrdersWaitingForDriver()
{
    Json::Value ordersWaiting;
    if(m_accountType != 1) return Json::Value::null; // Only transportation accounts can use this function
    try
    {
        SocketConnection* connection = SocketConnection::getInstance();
        string responseString = connection->sendRequest("GET", "/ordersWaitingForDriver", ordersWaiting.toStyledString());
        Json::Value responseJson = convertStringToJson(responseString);
        if(responseJson["code"].asString() != "200") return Json::Value::null;
        return responseJson["data"];
    }catch(const ConnectionException* e)
    {
        throw ConnectionException("We could not fetch any available orders at the moment.");
    }catch (const JSONException* e){
        cout << e->what() <<endl;
    }
}

Json::Value User::getAvailableDrivers(const string& city)
{
    Json::Value availableDrivers;
    availableDrivers["source"] = city;
    if(m_accountType != 1) return Json::Value::null; // Only transportation accounts can use this function
    try
    {
        SocketConnection* connection = SocketConnection::getInstance();
        string responseString = connection->sendRequest("GET", "/availableDrivers", availableDrivers.toStyledString());
        Json::Value responseJson = convertStringToJson(responseString);
        if(responseJson["code"].asString() != "200") return Json::Value::null;
        return responseJson["data"];
    }catch(const ConnectionException* e)
    {
        throw ConnectionException("We could not fetch any available orders at the moment.");
    }catch (const JSONException* e){
        cout << e->what() <<endl;
    }
}

Json::Value User::getShipmentHistory()
{
    Json::Value shipmentHistoryPayload;
    shipmentHistoryPayload["driverID"] = m_userID;
    if(m_accountType != 2) return Json::Value::null;
    try
    {
        SocketConnection* connection = SocketConnection::getInstance();
        string responseString = connection->sendRequest("GET", "/driverPastOrders", shipmentHistoryPayload.toStyledString());
        Json::Value responseJson = convertStringToJson(responseString);
        if(responseJson["code"].asString() == "204") return Json::Value::null;
        return responseJson["data"];
    }catch(const ConnectionException* e)
    {
        throw ConnectionException("We could not fetch past orders at the moment");
    }catch (const JSONException* e){
        cout << e->what() <<endl;
    }
}

string User::getNameByUserID(string userID)
{
    Json::Value driverNamePayload;
    driverNamePayload["userID"] = userID;
    try
    {
        SocketConnection* connection = SocketConnection::getInstance();
        string responseString = connection->sendRequest("GET", "/driverName", driverNamePayload.toStyledString());
        Json::CharReaderBuilder builder;
        Json::Value value;
        std::string errs;
        std::istringstream stream(responseString);
        Json::parseFromStream(builder, stream, &value, &errs);

        if(value["code"] != "200") return "Error";
        return value["data"].asString();
    }catch(const ConnectionException* e)
    {
        throw ConnectionException("We could not fetch your driver name at the moment");
    }catch (const JSONException* e){
        cout << e->what() <<endl;
    }
}

bool User::isDriverAccountInfoComplete() const {
    return !m_NINumber.empty() &&
       !m_drivingID.empty() &&
       !m_lorryType.empty() &&
       m_lorryWeight > 0 &&
       !m_lorryDimensions.empty() &&
       !m_regNumber.empty() &&
       m_cpcNumber > 0;
}
bool User::isTransportationCompanyAccountInfoComplete() const {
    return !m_address.empty() && !m_companyName.empty();
}