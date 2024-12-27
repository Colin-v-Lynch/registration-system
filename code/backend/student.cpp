// Student.cpp
// This file implements the Student class, which represents a student user in the system.
// It interacts with the database to fetch and manage the student's data, such as enrolled
// and completed courses, department, and profile information.

#include "Student.h"
#include "DatabaseConnection.h"
#include "RegistrationFacade.h"
#include <sqlite3.h>
#include <stdexcept>
#include <iostream>

// Constructor: Initializes a Student object by retrieving the user's data from the database.
Student::Student(const std::string& userID) {
    // Get the singleton instance of the database connection.
    DatabaseConnection* dbConnection = DatabaseConnection::getInstance();
    sqlite3* db = dbConnection->getDB();
    sqlite3_stmt* stmt;

    // Query the User table to retrieve the student's department.
    std::string query = "SELECT department FROM User WHERE user_id = ?";
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare User query");
    }
    sqlite3_bind_text(stmt, 1, userID.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {
        // If the user exists, populate the userID and department attributes.
        this->userID = userID;
        department = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    } else {
        // If the user doesn't exist, finalize the statement and create the user using RegistrationFacade.
        sqlite3_finalize(stmt);
        RegistrationFacade facade;
        facade.createUser(userID);

        // Default values for the newly created user.
        this->userID = userID;
        department = "CS"; // Default department set to "CS".
    }
    sqlite3_finalize(stmt);

    // Query the Taken table to retrieve courses the student has completed.
    query = "SELECT course_id FROM Taken WHERE user_id = ?";
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare Taken query");
    }
    sqlite3_bind_text(stmt, 1, userID.c_str(), -1, SQLITE_STATIC);

    // Populate the `takenCourses` vector with completed course IDs.
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        takenCourses.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }
    sqlite3_finalize(stmt);

    // Query the Enrolled table to retrieve courses the student is currently enrolled in.
    query = "SELECT course_id FROM Enrolled WHERE user_id = ?";
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare Enrolled query");
    }
    sqlite3_bind_text(stmt, 1, userID.c_str(), -1, SQLITE_STATIC);

    // Populate the `enrolledCourses` vector with enrolled course IDs.
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        enrolledCourses.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }
    sqlite3_finalize(stmt);
}

// Displays the courses the student is currently enrolled in.
void Student::viewEnrolledCourses() {
    for (const auto& course : enrolledCourses) {
        std::cout << "Enrolled Course: " << course << "\n";
    }
}

// Returns a reference to the vector of enrolled courses.
const std::vector<std::string>& Student::getEnrolledCourses() const {
    return enrolledCourses;
}

// Returns a reference to the vector of completed courses.
const std::vector<std::string>& Student::getTakenCourses() const {
    return takenCourses;
}

// Displays the student's schedule by retrieving details of each enrolled course.
void Student::viewSchedule() const {
    std::cout << "Student Schedule: \n";
    for (const auto& courseID : enrolledCourses) {
        // Create a Course object for each enrolled course to fetch its details.
        Course detailedCourse(courseID);
        std::cout << "- " << detailedCourse.getCourseID() << " at " << detailedCourse.getCourseTime() << "\n";
    }
}

// Displays the student's profile information, including their user ID and department.
void Student::viewProfile() {
    std::cout << "Student Profile: " << userID << "\n";
    std::cout << "Department: " << department << "\n";
}
