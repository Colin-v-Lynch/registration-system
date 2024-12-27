// RegistrationFacade.cpp
// This file implements the RegistrationFacade class, which provides a simplified interface
// for managing user creation, course registration, and dropping courses. It ensures the logic
// adheres to constraints like prerequisites, antirequisites, and seat availability.

#include "RegistrationFacade.h"
#include "Student.h"
#include "Course.h"
#include "DatabaseConnection.h"
#include <iostream>
#include <algorithm>

// Creates a new user in the system and inserts their details into the database.
void RegistrationFacade::createUser(const std::string& userName) {
    DatabaseConnection* dbConn = DatabaseConnection::getInstance();
    sqlite3* db = dbConn->getDB();
    sqlite3_stmt* stmt;

    // SQL query to insert a new user into the User table.
    const char* insertUserSQL = R"(
        INSERT INTO User (user_id, is_admin, department) 
        VALUES (?, 0, 'CS');  // Default department is 'CS', and the user is not an admin.
    )";

    // Prepare the SQL statement for execution.
    if (sqlite3_prepare_v2(db, insertUserSQL, -1, &stmt, nullptr) != SQLITE_OK) {
        std::cerr << "Failed to prepare user insertion query: " << sqlite3_errmsg(db) << "\n";
        return;
    }

    // Bind the userName parameter to the SQL statement.
    sqlite3_bind_text(stmt, 1, userName.c_str(), -1, SQLITE_STATIC);

    // Execute the SQL statement to insert the user.
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::cerr << "Failed to insert user: " << sqlite3_errmsg(db) << "\n";
    } else {
        std::cout << "User " << userName << " created successfully.\n";
    }

    // Finalize the statement to release resources.
    sqlite3_finalize(stmt);
}

// Registers a student for a course if all constraints are met (prerequisites, no antirequisites, etc.).
std::string RegistrationFacade::registerCourse(Student& student, Course& course) {
    if (isAlreadyEnrolled(student, course)) {
        return "Registration failed: Student already enrolled in " + course.getCourseID() + ".";
    }
    if (!hasCompletedPrerequisites(student, course)) {
        return "Registration failed: Missing prerequisites.";
    }
    if (hasViolatedAntirequisites(student, course)) {
        return "Registration failed: Antirequisite violation.";
    }

    DatabaseConnection* dbConn = DatabaseConnection::getInstance();
    sqlite3* db = dbConn->getDB();
    sqlite3_stmt* stmt;

    // Check if there are available seats in the course.
    if (course.getAvailableSeats() <= 0) {
        return "No available seats in course " + course.getCourseID() + ".";
    }

    // SQL query to register the student in the course.
    std::string registerQuery = "INSERT INTO Enrolled (user_id, course_id) VALUES (?, ?);";
    if (sqlite3_prepare_v2(db, registerQuery.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return "Failed to prepare registration query: " + std::string(sqlite3_errmsg(db));
    }

    // Bind student and course IDs to the query.
    sqlite3_bind_text(stmt, 1, student.getID().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, course.getCourseID().c_str(), -1, SQLITE_STATIC);

    // Execute the registration query.
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::string errorMsg = "Failed to register student: " + std::string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return errorMsg;
    }
    sqlite3_finalize(stmt);
    return "Registration successful for student " + student.getID() + " in course " + course.getCourseID() + ".";
}

// Removes a student from a course and updates the seat count.
std::string RegistrationFacade::dropCourse(Student& student, Course& course) {
    DatabaseConnection* dbConn = DatabaseConnection::getInstance();
    sqlite3* db = dbConn->getDB();
    sqlite3_stmt* stmt;

    // Check if the student is enrolled in the course.
    std::string checkEnrollmentQuery = "SELECT * FROM Enrolled WHERE user_id = ? AND course_id = ?;";
    if (sqlite3_prepare_v2(db, checkEnrollmentQuery.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return "Failed to prepare check enrollment query: " + std::string(sqlite3_errmsg(db));
    }

    sqlite3_bind_text(stmt, 1, student.getID().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, course.getCourseID().c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_ROW) {
        sqlite3_finalize(stmt);
        return "Drop failed: Student is not enrolled in " + course.getCourseID() + ".";
    }

    sqlite3_finalize(stmt); // Finalize the check query.

    // Remove the student from the course.
    std::string dropQuery = "DELETE FROM Enrolled WHERE user_id = ? AND course_id = ?;";
    if (sqlite3_prepare_v2(db, dropQuery.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return "Failed to prepare drop query: " + std::string(sqlite3_errmsg(db));
    }

    sqlite3_bind_text(stmt, 1, student.getID().c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, course.getCourseID().c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) != SQLITE_DONE) {
        std::string errorMsg = "Failed to drop student: " + std::string(sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return errorMsg;
    }

    sqlite3_finalize(stmt); // Finalize the drop query.
    return "Drop successful for student " + student.getID() + " from course " + course.getCourseID() + ".";
}

// Checks if the student has completed all prerequisites for a course.
bool RegistrationFacade::hasCompletedPrerequisites(const Student& student, const Course& course) {
    for (const auto& prereq : course.getPrerequisites()) {
        if (std::find(student.getTakenCourses().begin(), student.getTakenCourses().end(), prereq) == student.getTakenCourses().end()) {
            //std::cout << "Missing prerequisite: " << prereq << "\n";
            return false;
        }
    }
    return true;
}

// Checks if the student has violated any antirequisite constraints for a course.
bool RegistrationFacade::hasViolatedAntirequisites(const Student& student, const Course& course) {
    for (const auto& antireq : course.getAntirequisites()) {
        if (std::find(student.getTakenCourses().begin(), student.getTakenCourses().end(), antireq) != student.getTakenCourses().end()) {
            //std::cout << "Violated antirequisite: " << antireq << "\n";
            return true;
        }
    }
    return false;
}

// Checks if the student is already enrolled in a course.
bool RegistrationFacade::isAlreadyEnrolled(const Student& student, const Course& course) {
    if (std::find(student.getEnrolledCourses().begin(), student.getEnrolledCourses().end(), course.getCourseID()) != student.getEnrolledCourses().end()) {
        //std::cout << "Student is already enrolled in course: " << course.getCourseID() << "\n";
        return true;
    }
    return false;
}
