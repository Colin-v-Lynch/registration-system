// Course.cpp
// This file implements the Course class, which encapsulates information about a course
// and its relationships (prerequisites, antirequisites, and enrolled students). It interacts
// with the database to fetch and calculate course-related details.

#include "Course.h"
#include "DatabaseConnection.h"
#include <stdexcept>
#include <sqlite3.h>
#include <vector>
#include <string>
#include <algorithm>
#include <iostream>

// Constructor: Initializes a Course object by retrieving its details from the database.
Course::Course(const std::string& courseID) {
    // Retrieve a singleton instance of the database connection.
    DatabaseConnection* dbConnection = DatabaseConnection::getInstance();
    sqlite3* db = dbConnection->getDB();
    sqlite3_stmt* stmt;

    // Query the Course table to retrieve course details: description, total seats, and time.
    std::string query = "SELECT course_description, seats, course_time FROM Course WHERE course_id = ?";
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare Course query");
    }
    sqlite3_bind_text(stmt, 1, courseID.c_str(), -1, SQLITE_STATIC);

    // If the course exists, populate its attributes. Otherwise, throw an error.
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        this->courseID = courseID;
        name = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
        totalseats = sqlite3_column_int(stmt, 1);
        courseTime = std::string(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2)));
    } else {
        sqlite3_finalize(stmt);
        throw std::runtime_error("Course not found");
    }
    sqlite3_finalize(stmt);

    // Query the Prerequisite table to fetch the prerequisites for this course.
    query = "SELECT prereq_course_id FROM Prerequisite WHERE course_id = ?";
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare Prerequisite query");
    }
    sqlite3_bind_text(stmt, 1, courseID.c_str(), -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        prerequisites.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }
    sqlite3_finalize(stmt);

    // Query the Antirequisite table to fetch the antirequisites for this course.
    query = "SELECT antireq_course_id FROM Antirequisite WHERE course_id = ?";
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare Antirequisite query");
    }
    sqlite3_bind_text(stmt, 1, courseID.c_str(), -1, SQLITE_STATIC);
    while (sqlite3_step(stmt) == SQLITE_ROW) {
        antirequisites.push_back(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0)));
    }
    sqlite3_finalize(stmt);

    // Calculate the number of available seats by subtracting enrolled students from total seats.
    query = "SELECT COUNT(*) FROM Enrolled WHERE course_id = ?";
    if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        throw std::runtime_error("Failed to prepare Enrolled query");
    }
    sqlite3_bind_text(stmt, 1, courseID.c_str(), -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        int enrolledCount = sqlite3_column_int(stmt, 0);
        availableSeats = totalseats - enrolledCount;
    }
    sqlite3_finalize(stmt);
}

// Returns the course ID.
std::string Course::getCourseID() const {
    return courseID;
}

// Returns the course name.
std::string Course::getCourseName() const {
    return name;
}

// Returns the scheduled time of the course.
std::string Course::getCourseTime() const {
    return courseTime;
}

// Returns the number of available seats in the course.
int Course::getAvailableSeats() const {
    return availableSeats;
}

// Updates the number of available seats for the course.
void Course::setAvailableSeats(int seats) {
    availableSeats = seats;
}

// Returns the list of prerequisite courses.
std::vector<std::string> Course::getPrerequisites() const {
    return prerequisites;
}

// Returns the list of antirequisite courses.
std::vector<std::string> Course::getAntirequisites() const {
    return antirequisites;
}

// Returns the list of students enrolled in the course.
std::vector<std::string> Course::getEnrolledStudents() const {
    return enrolledStudents;
}

// Prints all details of the course to the console.
void Course::printDetails() const {
    std::cout << "Course ID: " << courseID << std::endl;
    std::cout << "Name: " << name << std::endl;
    std::cout << "Course Time: " << courseTime << std::endl;
    std::cout << "Total Seats: " << totalseats << std::endl;
    std::cout << "Available Seats: " << availableSeats << std::endl;

    std::cout << "Prerequisites: ";
    for (const auto& prereq : prerequisites) {
        std::cout << prereq << " ";
    }
    std::cout << std::endl;

    std::cout << "Antirequisites: ";
    for (const auto& antireq : antirequisites) {
        std::cout << antireq << " ";
    }
    std::cout << std::endl;

    std::cout << "Enrolled Students: ";
    for (const auto& student : enrolledStudents) {
        std::cout << student << " ";
    }
    std::cout << std::endl;
}
