// DatabaseConnection.cpp
// This file implements the DatabaseConnection class, which follows the Singleton pattern
// to provide a single, shared connection to the database used in the course registration system.

#include "DatabaseConnection.h"
#include <iostream>
#include <stdexcept>

// Initialize the static instance to nullptr, ensuring only one instance is created.
DatabaseConnection* DatabaseConnection::instance = nullptr;

// Constructor: Initializes the SQLite database connection and ensures the schema is up-to-date.
DatabaseConnection::DatabaseConnection() {
    // Open the database connection. If it fails, throw an exception with the error message.
    if (sqlite3_open("registration_system.db", &db)) {
        throw std::runtime_error("Failed to open database: " + std::string(sqlite3_errmsg(db)));
    }

    // Ensure that all required tables exist by calling initializeSchema().
    initializeSchema();
}

// Destructor: Closes the database connection to release resources.
DatabaseConnection::~DatabaseConnection() {
    sqlite3_close(db);
}

// Returns the singleton instance of DatabaseConnection. Creates it if it doesn't exist.
DatabaseConnection* DatabaseConnection::getInstance() {
    if (!instance) {
        instance = new DatabaseConnection(); // Lazy initialization of the Singleton instance.
    }
    return instance;
}

// Provides access to the underlying SQLite database object.
sqlite3* DatabaseConnection::getDB() {
    return db;
}

// Initializes the database schema by creating tables if they don't already exist.
void DatabaseConnection::initializeSchema() {
    // SQL commands to create the required tables for the registration system.
    const char* createTablesSQL = R"(
        CREATE TABLE IF NOT EXISTS User (
            user_id VARCHAR(100) PRIMARY KEY,       -- Unique identifier for users
            is_admin BOOLEAN,                       -- Indicates whether the user is an admin
            department VARCHAR(100)                -- Department associated with the user
        );
        
        CREATE TABLE IF NOT EXISTS Course (
            course_id VARCHAR(100) PRIMARY KEY,     -- Unique identifier for courses
            course_description TEXT,               -- Detailed description of the course
            department VARCHAR(100),               -- Department offering the course
            seats INT,                              -- Total number of seats available
            course_time VARCHAR(50)                -- Scheduled time for the course
        );
        
        CREATE TABLE IF NOT EXISTS Prerequisite (
            course_id VARCHAR(100),                -- Course that has a prerequisite
            prereq_course_id VARCHAR(100),         -- Prerequisite course
            PRIMARY KEY (course_id, prereq_course_id),
            FOREIGN KEY (course_id) REFERENCES Course(course_id),
            FOREIGN KEY (prereq_course_id) REFERENCES Course(course_id)
        );
        
        CREATE TABLE IF NOT EXISTS Antirequisite (
            course_id VARCHAR(100),                -- Course that has an antirequisite
            antireq_course_id VARCHAR(100),        -- Antirequisite course
            PRIMARY KEY (course_id, antireq_course_id),
            FOREIGN KEY (course_id) REFERENCES Course(course_id),
            FOREIGN KEY (antireq_course_id) REFERENCES Course(course_id)
        );
        
        CREATE TABLE IF NOT EXISTS Enrolled (
            course_id VARCHAR(100),                -- Course a user is enrolled in
            user_id VARCHAR(100),                  -- User enrolled in the course
            PRIMARY KEY (course_id, user_id),
            FOREIGN KEY (course_id) REFERENCES Course(course_id),
            FOREIGN KEY (user_id) REFERENCES User(user_id)
        );
        
        CREATE TABLE IF NOT EXISTS Taken (
            course_id VARCHAR(100),                -- Course a user has completed
            user_id VARCHAR(100),                  -- User who completed the course
            PRIMARY KEY (course_id, user_id),
            FOREIGN KEY (course_id) REFERENCES Course(course_id),
            FOREIGN KEY (user_id) REFERENCES User(user_id)
        );
    )";

    // Execute the SQL commands to initialize the schema. Report any errors encountered.
    char* errorMessage = nullptr;
    if (sqlite3_exec(db, createTablesSQL, nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::string error = "Failed to initialize database schema: ";
        error += errorMessage;
        sqlite3_free(errorMessage);
        throw std::runtime_error(error);
    }
}
