#include "include/Utils.h"
#include <string>
#include <iostream>
#include <regex>
#include <unistd.h>
#include "json/json.h"
#include "include/Exceptions.h"
#include<limits>
#include "include/cityCoordinates.h"

using namespace std;

string toLower(string textToConvert)
{
    for (int i = 0; i < textToConvert.length(); i++)
        textToConvert[i] = tolower(textToConvert[i]);
    return textToConvert;
}

int GetInputInt(string prompt, int maxOption)
{
    int choice;
    cout << prompt;
    for (;;) {
        if (cin >> choice && (choice <= maxOption && choice > 0)) {
            break;
        } else {
            cout << "Please enter a valid option (1-"+to_string(maxOption)+ "):" <<endl;
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }
    }
    return choice;
}

bool isValidDimensions(std::string input) {
    // Split the input string by 'x'
    size_t pos = 0;
    std::string token;
    std::string delimiter = "x";
    int count = 0;
    while ((pos = input.find(delimiter)) != std::string::npos) {
        token = input.substr(0, pos);
        try {
            std::stoi(token); // Check if the token is a valid integer
        } catch (const std::invalid_argument& ia) {
            return false;
        }
        input.erase(0, pos + delimiter.length());
        count++;
    }
    try {
        std::stoi(input); // Check the last token
    } catch (const std::invalid_argument& ia) {
        return false;
    }
    count++;
    return (count == 3); // Check if there are exactly 3 tokens
}

bool isValidWeight(const std::string& str) {
    if (str.empty()) {
        return false;
    }

    size_t pos = 0;
    bool hasDecimalPoint = false;

    if (str[pos] == '+' || str[pos] == '-') {
        ++pos;
    }

    if (pos == str.length()) {
        return false;
    }

    while (pos < str.length()) {
        if (std::isdigit(str[pos])) {
            ++pos;
            continue;
        }

        if (str[pos] == '.') {
            if (hasDecimalPoint) {
                return false;
            }
            hasDecimalPoint = true;
            ++pos;

            if (pos == str.length()) {
                return false;
            }

            continue;
        }

        return false;
    }

    return true;
}



bool isValidCargoType(const std::string& cargoType) {
    std::string lowerCaseCargoType = cargoType;
    std::transform(lowerCaseCargoType.begin(), lowerCaseCargoType.end(), lowerCaseCargoType.begin(), ::tolower);

    return (lowerCaseCargoType == "frozen" ||
            lowerCaseCargoType == "fragile" ||
            lowerCaseCargoType == "perishable" ||
            lowerCaseCargoType == "hazardous");
}

std::string toLowerCase(const std::string& input) {
    std::string lowerCaseString;
    for (char c : input) {
        lowerCaseString += std::tolower(c);
    }
    return lowerCaseString;
}

std::string convertCityName(const std::string& cityName)
{
    std::string lowerCaseCity = toLowerCase(cityName);
    lowerCaseCity[0] = std::toupper(lowerCaseCity[0]);
    return lowerCaseCity;
}

bool isValidPlace(const std::string& city) {
    std::string lowerCaseCity = convertCityName(city);
    return cityCoordinates.find(lowerCaseCity) != cityCoordinates.end();
}


string GetInputString(string prompt)
{
    string choice;
    cout << prompt;
    cin >> choice;
    return choice;
}
string GetInputSentence(const std::string& prompt)
{
    std::string choice;
    std::cout << prompt;
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::getline(std::cin, choice);
    return choice;
}
void pressEnterToContinue()
{
    print("");
    std::cout << "Press enter to continue ";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}
bool isValidEmail(const string& email)
{
    const regex pattern("(\\w+)(\\.|_)?(\\w*)@(\\w+)(\\.(\\w+))+");
    return regex_match(email, pattern);
}
bool isValidPassword(const string& password, const string& confirmPassword)
{
    const regex pattern("^(?=.*[a-z])(?=.*[A-Z])(?=.*\\d)(?=.*[^\\da-zA-Z]).{8,15}$");
    if(password != confirmPassword){return false;}
    return regex_match(password, pattern);
}
bool isValidNiNumber(const string& NINumber) {
    const regex pattern("^[A-CEGHJ-PR-TW-Z]{2}[0-9]{6}[ABCD\\s]{1}$");
    return regex_match(NINumber, pattern);
}
bool isValidDrivingLicenseNumber(const string& DrivingID) {
    const regex pattern("[A-Z]{5}[0-9]{6}[A-Z]{2}[0-9]{2}");
    return regex_match(DrivingID, pattern);
}
bool isValidCPCNumber(const string& CPCNumber) {
    const regex pattern("^[a-zA-Z]{3}[0-9]{6}$");
    return regex_match(CPCNumber, pattern);
}


void ClearScreen()
{
    cout << "\033[2J\033[1;1H";
}

void print(const string& text)
{
    std::cout << text << std::endl;
}

void printError(const string& text){
    std::cerr << text << std::endl;
}

Json::Value getJsonValueForKey(string strObject, string key){
    Json::Value jsonObject;
    Json::Reader reader;
    if (strObject.empty()) throw new JSONException("JSON object is empty");
    if (!reader.parse(strObject, jsonObject)) throw new JSONException("Could not parse the JSON object");
    if (!jsonObject.isMember(key)) throw new JSONException("Could not find: " + key +  " in json object.");
    Json::Value value = jsonObject[key];
    if (value.isString()) {
        if (!reader.parse(value.asString(), value)) {
            throw new JSONException("Could not parse the JSON string value for key: " + key);
        }
    }
    return value;
}

Json::Value convertStringToJson(string stringToConvert)
{
    std::istringstream iss(stringToConvert);
    Json::CharReaderBuilder readerBuilder;
    Json::Value jsonData;
    std::string errs;
    if (!Json::parseFromStream(readerBuilder, iss, &jsonData, &errs)) {
        throw JSONException("Could not parse Json:" + errs);
    }
    return jsonData;
}

bool isValidMobileNumber(const std::string& mobileNumber)
{
    for (char c : mobileNumber)
    {
        if (!std::isdigit(c))
        {
            return false;
        }
    }
    return true;
}
bool isValidAccountType(const string& accountType)
{
    if(accountType == "0" || accountType == "1" || accountType == "2") return true;
    return false;
}



string getStringValueForKey(string strObject, Json::Value userJsonObject, string key) {
    Json::Value jsonObject;
    Json::Reader reader;

    if(strObject.empty() && (userJsonObject.empty() || !userJsonObject.isObject())) throw new JSONException("Parameter 'strObject' or 'userJsonObject' must be given or 'userJsonObject' is not a valid JSON Object");
    if(key.empty()) throw new JSONException("Key not specified");


    if(!strObject.empty()){
        if (reader.parse(strObject, jsonObject)) throw new JSONException("Could not parse the JSON object.");
        if (!jsonObject.isMember(key)) throw new JSONException("Could not find: " + key +  " in JSON object.");
        Json::Value value = jsonObject[key];
        if (value.isString()) {
            return value.asString();
        } else if (value.isInt()) {
            return std::to_string(value.asInt());
        } else if (value.isDouble()) {
            return std::to_string(value.asDouble());
        } else if (value.isBool()) {
            return value.asBool() ? "true" : "false";
        } else {
            return "";
        }
    }

    if (!userJsonObject.isMember(key)) throw new JSONException("Could not find: " + key + " in JSON object.");
    Json::Value userJsonValue = userJsonObject.get(key, Json::Value::null);
    if (userJsonValue.isNull()) throw new JSONException("Value for " + key + " is null.");
    return userJsonValue.asString();
}

bool isValidLorryType(const std::string &lorryType)
{
    // List of valid lorry types in the UK
    std::vector<std::string> validLorryTypes = {
            "R", // Rigid
            "A", // Articulated
            "D", // Drawbar
            "T", // Tipper
            "F", // Flatbed
            "B", // Box
            "C", // Curtainsider
    };

    return std::find(validLorryTypes.begin(), validLorryTypes.end(), lorryType) != validLorryTypes.end();
}

bool ValidateLogin(const string& email, const string& password, User& user)
{
    try {
        int loginResult = user.Login(email, password);
        if(loginResult != 0){
            ClearScreen();
            print("Invalid Email or Password, try again.");
            return false;
        }
        return true;
    } catch (const AuthenticationException& e) {
        ClearScreen();
        print("We ran into an error logging you in");
        return false;
    } catch (const ConnectionException& e){
        ClearScreen();
        print("We could not connect to the server. Is it running?");
        return false;
    } catch (const JSONException& e){
        ClearScreen();
        print("We ran into a JSON error");
        return false;
    }
}
bool checkForQuit(string text){
    if(toLower(text) == "quit"){
        ClearScreen();
        return true;
    }
    return false;
}


double toRadians(double degree) {
    return degree * (M_PI / 180.0);
}

double haversineDistance(double lat1, double lon1, double lat2, double lon2) {
    double dLat = toRadians(lat2 - lat1);
    double dLon = toRadians(lon2 - lon1);

    lat1 = toRadians(lat1);
    lat2 = toRadians(lat2);

    double a = pow(sin(dLat / 2.0), 2) +
               cos(lat1) * cos(lat2) * pow(sin(dLon / 2.0), 2);
    double c = 2.0 * atan2(sqrt(a), sqrt(1.0 - a));

    return 6371.0 * c;
}

string distanceBetweenCities(string source, string destination) {
    auto sourceCoords = cityCoordinates.find(source);
    auto destinationCoords = cityCoordinates.find(destination);

    if (sourceCoords == cityCoordinates.end() || destinationCoords == cityCoordinates.end()) {
        cerr << "One or both of the cities not found in the list." << endl;
        return "";
    }
    return to_string(haversineDistance(sourceCoords->second.first, sourceCoords->second.second,
                                       destinationCoords->second.first, destinationCoords->second.second));
}