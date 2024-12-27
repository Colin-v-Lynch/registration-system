// UserFactory.cpp
// This file implements the UserFactory class, which is responsible for creating user objects
// based on their role (admin or student). The decision is made dynamically based on data 
// retrieved from the database.

#include "Student.h"
#include "Admin.h"
#include "UserFactory.h"
#include "DatabaseConnection.h"
#include <stdexcept>
#include <sqlite3.h>
#include <iostream>

// Creates a user object (either Admin or Student) based on the user ID provided.
Person* UserFactory::createUser(const std::string& userID) {
    // Initialize database connection
    // Retrieve a singleton instance of the DatabaseConnection class.
    DatabaseConnection* dbConnection = DatabaseConnection::getInstance();
    sqlite3* db = dbConnection->getDB();
    sqlite3_stmt* stmt;

    // SQL query to determine if the user exists and whether they are an admin.
    std::string query = "SELECT is_admin FROM User WHERE user_id = ?";
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        // If the query fails to prepare, throw an exception with an appropriate error message.
        throw std::runtime_error("Failed to prepare User query");
    }

    // Bind the userID parameter to the query.
    sqlite3_bind_text(stmt, 1, userID.c_str(), -1, SQLITE_STATIC);

    bool isAdmin = false;   // Variable to hold whether the user is an admin (1) or not (0).
    bool userFound = false; // Tracks if the user exists in the database.
    
    // Execute the query and check if a row is returned.
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // Extract the `is_admin` field value from the returned row.
        isAdmin = sqlite3_column_int(stmt, 0);  // 0 represents false, 1 represents true.
        userFound = true;                      // User exists in the database.
    }
    // Finalize the prepared statement to release resources.
    sqlite3_finalize(stmt);

    // Create the appropriate user object based on the retrieved data.
    if (userFound) {
        if (isAdmin) {
            // If the user is an admin, create and return an Admin object.
            Admin* admin = new Admin();
            admin->setID(userID); // Set the user ID for the Admin object.
            return admin;
        } else {
            // If the user is not an admin, create and return a Student object.
            Student* student = new Student(userID); // Use the constructor for initialization.
            student->setID(userID);                 // Set the user ID for the Student object.
            return student;
        }
    } else {
        // If the user was not found, throw an exception indicating the issue.
        throw std::runtime_error("User not found in the database");
    }
}
