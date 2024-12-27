// Admin.cpp
// This file implements the Admin class, providing functions for administrators
// to view course details, enrolled students, and their own profile information.

#include "Admin.h"
#include "Course.h"
#include "DatabaseConnection.h"
#include <iostream>
#include <vector>
#include <stdexcept>

// Displays detailed information about a specific course.
void Admin::viewCourseDetails(const Course& course) {
    // Print the name, ID, time, and seat availability for the provided course.
    std::cout << "Course Details for: " << course.getCourseName() << "\n";
    std::cout << "Course ID: " << course.getCourseID() << "\n";
    std::cout << "Time: " << course.getCourseTime() << "\n";
    std::cout << "Available Seats: " << course.getAvailableSeats() << "\n";
}

// Displays all courses along with the list of students enrolled in each.
void Admin::viewAllClassLists() {
    // Retrieve a singleton instance of the database connection.
    DatabaseConnection* dbConnection = DatabaseConnection::getInstance();
    sqlite3* db = dbConnection->getDB();
    sqlite3_stmt* stmt;

    // Query to retrieve course IDs and descriptions from the Course table.
    std::string courseQuery = "SELECT course_id, course_description FROM Course";
    if (sqlite3_prepare_v2(db, courseQuery.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        // Throw an exception if the query fails to prepare.
        throw std::runtime_error("Failed to prepare Course query");
    }

    // Vector to store the course details as pairs of course ID and course description.
    std::vector<std::pair<std::string, std::string>> courses;
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        // Extract the course ID and description from the query result.
        std::string courseID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
        std::string courseName = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
        courses.emplace_back(courseID, courseName); // Store the course details in the vector.
    }
    sqlite3_finalize(stmt); // Finalize the query to release resources.

    // Iterate over each course and print its details along with the enrolled students.
    for (const auto& course : courses) {
        std::cout << "Course: " << course.second << " (ID: " << course.first << ")\n";
        std::cout << "Enrolled Students:\n";

        // Query to fetch the IDs of students enrolled in the current course.
        std::string enrolledQuery = "SELECT user_id FROM Enrolled WHERE course_id = ?";
        if (sqlite3_prepare_v2(db, enrolledQuery.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            // Throw an exception if the enrollment query fails to prepare.
            throw std::runtime_error("Failed to prepare Enrolled query");
        }
        // Bind the current course ID to the query.
        sqlite3_bind_text(stmt, 1, course.first.c_str(), -1, SQLITE_STATIC);

        bool hasStudents = false; // Tracks whether the course has any enrolled students.
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            hasStudents = true; // Mark that the course has students.
            // Extract the student ID from the query result.
            std::string studentID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            std::cout << " - " << studentID << "\n"; // Print the student ID.
        }
        sqlite3_finalize(stmt); // Finalize the enrollment query.

        // Print a message if no students are enrolled in the course.
        if (!hasStudents) {
            std::cout << " (No students enrolled)\n";
        }

        std::cout << "\n"; // Add spacing between courses.
    }
}

// Displays the admin's profile information, specifically their user ID.
void Admin::viewProfile() {
    // Print the admin's user ID as part of their profile information.
    std::cout << "Admin Profile: " << userID << "\n";
}
