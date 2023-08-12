//
// Created by Nathan Ellis on 21/03/2023.
//
#include <stdexcept>
#include <string>

#ifndef SERVER_EXCEPTIONS_H
#define SERVER_EXCEPTIONS_H

/**
 * Exception thrown when there is an issue with a network connection.
 */
class ConnectionException : public std::exception {
public:
    /**
     * Constructor that takes a message as input.
     *
     * @param message The message to be associated with the exception.
     */
    ConnectionException(const std::string &message) : message_(message) {}

    /**
     * Returns the message associated with the exception.
     *
     * @return const char* The message associated with the exception.
     */
    virtual const char *what() const noexcept {
        return message_.c_str();
    }

private:
    std::string message_;
};

/**
 * Exception thrown when there is an issue with JSON parsing or generation.
 */
class JSONException : public std::exception {
public:
    /**
     * Constructor that takes a message as input.
     *
     * @param message The message to be associated with the exception.
     */
    JSONException(const std::string &message) : message_(message) {}

    /**
     * Returns the message associated with the exception.
     *
     * @return const char* The message associated with the exception.
     */
    virtual const char *what() const noexcept {
        return message_.c_str();
    }

private:
    std::string message_;
};

/**
 * @brief A class to handle database exceptions.
 *
 *  Exception thrown when there is an issue with a database operation.
 */
class DatabaseException : public std::exception {
public:
    /**
     * Constructor that takes a message as input.
     *
     * @param message The message to be associated with the exception.
     */
    DatabaseException(const std::string &message) : message_(message) {}

    /**
     * Returns the message associated with the exception.
     *
     * @return const char* The message associated with the exception.
     */
    virtual const char *what() const noexcept {
        return message_.c_str();
    }

private:
    std::string message_;
};


#endif //SERVER_EXCEPTIONS_H
