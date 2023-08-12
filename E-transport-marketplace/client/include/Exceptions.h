//
// Created by Nathan Ellis on 21/03/2023.
//
/**
 * @file Exceptions.h
 * @brief This file contains Exception declarations.
 */

#include <stdexcept>
#include <string>
#ifndef SERVER_EXCEPTIONS_H
#define SERVER_EXCEPTIONS_H

/**
 * @brief An exception class representing an error in JSON parsing or encoding.
 *
 * This exception class is thrown when there is an error parsing or encoding JSON data.
 */
class JSONException : public std::exception {
public:
    /**
     * @brief Constructor for the JSONException class.
     *
     * @param message The error message associated with the exception.
     */
    JSONException(const std::string& message) : message_(message) {}

    /**
     * @brief Returns the error message associated with the exception.
     *
     * @return A pointer to the error message string.
     */
    virtual const char* what() const noexcept {
        return message_.c_str();
    }

private:
    std::string message_;   /**< The error message associated with the exception. */
};

/**
 * @brief An exception class representing an authentication error.
 *
 * This exception class is thrown when there is an error authenticating a user or service.
 */
class AuthenticationException : public std::exception {
public:
    /**
     * @brief Constructor for the AuthenticationException class.
     *
     * @param message The error message associated with the exception.
     */
    AuthenticationException(const std::string& message) : message_(message) {}

    /**
     * @brief Returns the error message associated with the exception.
     *
     * @return A pointer to the error message string.
     */
    virtual const char* what() const noexcept {
        return message_.c_str();
    }

private:
    std::string message_;   /**< The error message associated with the exception. */
};

/**
 * @brief An exception class representing a network connection error.
 *
 * This exception class is thrown when there is an error connecting to a remote service or server.
 */
class ConnectionException : public std::exception {
public:
    /**
     * @brief Constructor for the ConnectionException class.
     *
     * @param message The error message associated with the exception.
     */
    ConnectionException(const std::string& message) : message_(message) {}

    /**
     * @brief Returns the error message associated with the exception.
     *
     * @return A pointer to the error message string.
     */
    virtual const char* what() const noexcept {
        return message_.c_str();
    }

private:
    std::string message_;   /**< The error message associated with the exception. */
};



#endif //SERVER_EXCEPTIONS_H
