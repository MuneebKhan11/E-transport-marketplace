//
// Created by Nathan Ellis on 12/04/2023.
//

#include "include/Driver.h"
#include "string"
#include <json/json.h>
#include "include/Utils.h"

std::vector<Driver> parseDrivers(const string& jsonData) {
    std::vector<Driver> drivers;

    Json::CharReaderBuilder builder;
    Json::Value root;
    std::string errs;
    std::istringstream iss(jsonData);
    if (!Json::parseFromStream(builder, iss, &root, &errs)) {
        return drivers;
    }

    for (const auto& key : root.getMemberNames()) {
        const Json::Value &orderJson = root[key];
        Driver driver;

        driver.driverID = orderJson["ID"].asString();
        driver.email = orderJson["email"].asString();
        driver.firstName = orderJson["firstName"].asString();
        driver.lastName = orderJson["lastName"].asString();
        driver.companyName = orderJson["companyName"].asString();
        driver.mobileNumber = orderJson["mobileNumber"].asString();
        driver.address = orderJson["address"].asString();
        driver.NINumber = orderJson["NINumber"].asString();
        driver.drivingID = orderJson["drivingID"].asString();
        driver.regNumber = orderJson["regNumber"].asString();
        driver.lorryDimension = orderJson["lorryDimension"].asString();
        driver.lorryWeight = orderJson["lorryWeight"].asString();
        driver.lorryType = orderJson["lorryType"].asString();
        driver.cpcNumber = orderJson["cpcNumber"].asString();
        driver.isBusy = orderJson["isBusy"].asString();
        drivers.push_back(driver);
    }
    return drivers;
}

Driver* findDriverByID(const std::vector<Driver>& drivers, const std::string& target_driver_id) {
    for (const auto& driver : drivers) {
        if (driver.driverID == target_driver_id) {
            return const_cast<Driver*>(&driver);
        }
    }
    return nullptr;
}
