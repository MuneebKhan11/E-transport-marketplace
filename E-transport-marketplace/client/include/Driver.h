//
// Created by Nathan Ellis on 12/04/2023.
//

#ifndef CLIENT_DRIVER_H
#define CLIENT_DRIVER_H

#include <iostream>
#include <string>

using namespace std;


/**
 * @brief Represents a driver user in the system.
 *
 * This struct represents a driver user in the system. It contains various fields for storing
 * the driver's personal and professional information, such as their ID, email, name, address,
 * and details about their lorry.
 */
struct Driver {
    std::string driverID;
    std::string email;
    std::string firstName;
    std::string lastName;
    std::string companyName;
    std::string mobileNumber;
    std::string address;
    std::string NINumber;
    std::string drivingID;
    std::string regNumber;
    std::string lorryDimension;
    std::string lorryWeight;
    std::string lorryType;
    std::string cpcNumber;
    std::string isBusy;
};

/**
 * @brief Parses a JSON string containing driver data and returns a vector of Driver objects.
 *
 * This function parses a JSON string containing driver data and returns a vector of Driver objects.
 * The input JSON string should be in a specific format, with each object containing fields for all
 * the necessary driver information. If the parsing is successful, a vector of Driver objects is returned.
 *
 * @param jsonData The JSON string containing driver data to parse.
 * @return A vector of Driver objects, parsed from the input JSON string.
 */
std::vector<Driver> parseDrivers(const string& jsonData);

/**
 * @brief Finds a driver with the specified ID in the given vector of Driver objects.
 *
 * This function searches the given vector of Driver objects for a driver with the specified ID.
 * If a matching driver is found, a pointer to that driver is returned. If no matching driver is found,
 * a null pointer is returned instead.
 *
 * @param drivers The vector of Driver objects to search.
 * @param target_driver_id The ID of the driver to find.
 * @return A pointer to the matching Driver object, or nullptr if no matching driver is found.
 */
Driver* findDriverByID(const std::vector<Driver>& drivers, const std::string& target_driver_id);


#endif //CLIENT_DRIVER_H
