#include <mutex>
#include <sqlite3.h>
#include <vector>
#include "include/Exceptions.h"

/**
 * @brief A class to interact with an SQLite database.
 *
 * The Database class provides functions to connect and disconnect to a database,
 * as well as to execute SQL queries and bind parameters to them.
 */
class Database {
private:
    std::string db_file; /**< The name of the database file. */
    sqlite3* db; /**< The pointer to the database instance. */
    std::mutex mtx; /**< A mutex to ensure thread-safety. */

public:
    /**
     * @brief Constructor for the Database class.
     *
     * @param file The name of the SQLite database file.
     */
    Database(std::string file) : db_file(file), db(nullptr) {}

    /**
     * @brief Destructor for the Database class.
     *
     * Closes the connection to the database, if it is still open.
     */
    ~Database() {
        if (db != nullptr) {
            sqlite3_close(db);
        }
    }

    /**
     * @brief Connects to the database.
     *
     * Throws a DatabaseException if the connection fails.
     */
    void connect() {
        int rc = sqlite3_open(db_file.c_str(), &db);
        if (rc != SQLITE_OK) {
            throw DatabaseException(string("Error Opening database: ") + sqlite3_errmsg(db));
        }
    }

    /**
     * @brief Disconnects from the database.
     *
     * Closes the connection to the database, if it is still open.
     */
    void disconnect() {
        if (db != nullptr) {
            sqlite3_close(db);
            db = nullptr;
        }
    }

    /**
     * @brief Executes an SQL query with optional parameters and returns the results.
     *
     * @tparam Args The types of the query parameters.
     * @param sql The SQL query string.
     * @param args The optional query parameters.
     * @return A pair containing the query results as a vector of vectors of strings,
     *         and the last inserted row ID.
     * @throw DatabaseException if the query preparation or execution fails.
     */
    template <typename... Args>
    std::pair<std::vector<std::vector<std::string>>, int> query(std::string sql, Args... args) {
        std::lock_guard<std::mutex> guard(mtx);

        if (db == nullptr) {
            throw DatabaseException(string("Database is not connected: ") + sqlite3_errmsg(db));
        }

        sqlite3_stmt* stmt;
        int rc = sqlite3_prepare_v2(db, sql.c_str(), -1, &stmt, nullptr);
        if (rc != SQLITE_OK) {
            throw DatabaseException(string("Error preparing statement") + sqlite3_errmsg(db));
        }

        bind_params(stmt, 1, args...);

        std::vector<std::vector<std::string>> results;

        while (true) {
            rc = sqlite3_step(stmt);
            if (rc == SQLITE_DONE) {
                break;
            }
            if (rc != SQLITE_ROW) {
                throw DatabaseException(string("Error executing SQL statement: ") + sqlite3_errmsg(db));
            }

            int num_cols = sqlite3_column_count(stmt);
            std::vector<std::string> row(num_cols);

            for (int i = 0; i < num_cols; i++) {
                const char* col_val = (const char*)sqlite3_column_text(stmt, i);
                row[i] = (col_val ? col_val : "");
            }

            results.push_back(row);
        }

        int last_insert_rowid = sqlite3_last_insert_rowid(db);

        sqlite3_finalize(stmt);

        return std::make_pair(results, last_insert_rowid);
    }

    /**
     * @brief Binds parameters to a prepared SQL statement.
     * This function template recursively binds a variable number of parameters to
     * a prepared SQL statement. It uses the bind_param function to bind each
     * parameter to a specific index in the statement.
     * @tparam T The type of the first parameter to bind.
     * @tparam Args The types of the remaining parameters to bind.
     * @param stmt The prepared SQL statement to bind parameters to.
     * @param idx The index of the first parameter to bind.
     * @param first The first parameter to bind.
     * @param args The remaining parameters to bind.
     */
    template <typename T, typename... Args>
    void bind_params(sqlite3_stmt* stmt, int idx, T first, Args... args) {
    bind_param(stmt, idx, first);
    bind_params(stmt, idx + 1, args...);
    }

    /**
     * @brief Binds a single parameter to a prepared SQL statement.
     * This function template binds a single parameter of a specific type to a
     * prepared SQL statement at a specific index. It uses the appropriate SQLite
     * function (sqlite3_bind_int, sqlite3_bind_double, sqlite3_bind_text,
     * sqlite3_bind_blob, or sqlite3_bind_null) to bind the parameter.
     * @tparam T The type of the parameter to bind.
     * @param stmt The prepared SQL statement to bind the parameter to.
     * @param idx The index of the parameter to bind.
     * @param arg The parameter to bind.
     */
    template <typename T>
    void bind_params(sqlite3_stmt* stmt, int idx, T arg) {
        bind_param(stmt, idx, arg);
    }

    /**
 * @brief Bind an integer value to a prepared SQLite statement.
 *
 * @param[in] stmt  Pointer to the prepared SQLite statement.
 * @param[in] idx   Index of the parameter to bind the value to.
 * @param[in] value The integer value to be bound.
 */
    void bind_param(sqlite3_stmt* stmt, int idx, int value) {
        sqlite3_bind_int(stmt, idx, value);
    }

/**
 * @brief Bind a double value to a prepared SQLite statement.
 *
 * @param[in] stmt  Pointer to the prepared SQLite statement.
 * @param[in] idx   Index of the parameter to bind the value to.
 * @param[in] value The double value to be bound.
 */
    void bind_param(sqlite3_stmt* stmt, int idx, double value) {
        sqlite3_bind_double(stmt, idx, value);
    }

/**
 * @brief Bind a string value to a prepared SQLite statement.
 *
 * @param[in] stmt  Pointer to the prepared SQLite statement.
 * @param[in] idx   Index of the parameter to bind the value to.
 * @param[in] value The string value to be bound.
 */
    void bind_param(sqlite3_stmt* stmt, int idx, const std::string& value) {
        sqlite3_bind_text(stmt, idx, value.c_str(), -1, SQLITE_TRANSIENT);
    }

/**
 * @brief Bind a null-terminated C string value to a prepared SQLite statement.
 *
 * @param[in] stmt  Pointer to the prepared SQLite statement.
 * @param[in] idx   Index of the parameter to bind the value to.
 * @param[in] value The null-terminated C string value to be bound.
 */
    void bind_param(sqlite3_stmt* stmt, int idx, const char* value) {
        sqlite3_bind_text(stmt, idx, value, -1, SQLITE_TRANSIENT);
    }

/**
 * @brief Bind a blob value to a prepared SQLite statement.
 *
 * @param[in] stmt  Pointer to the prepared SQLite statement.
 * @param[in] idx   Index of the parameter to bind the value to.
 * @param[in] value The vector of characters representing the blob value to be bound.
 */
    void bind_param(sqlite3_stmt* stmt, int idx, const std::vector<char>& value) {
        sqlite3_bind_blob(stmt, idx, value.data(), value.size(), SQLITE_TRANSIENT);
    }

/**
 * @brief Bind a null value to a prepared SQLite statement.
 *
 * @param[in] stmt  Pointer to the prepared SQLite statement.
 * @param[in] idx   Index of the parameter to bind the value to.
 * @param[in] value A null pointer representing the null value to be bound.
 */
    void bind_param(sqlite3_stmt* stmt, int idx, std::nullptr_t) {
        sqlite3_bind_null(stmt, idx);
    }

};
