#include <iostream>
#include <string>
#include <fstream>
#include <sqlite3.h>
#include <filesystem>
#include <vector>
#include <json/json.h>
#include "UserFactory.h"
#include "RegistrationFacade.h"
#include "Admin.h"
#include "Student.h"
#include "DatabaseConnection.h"

// Function to execute a single SQL command
void executeSQL(sqlite3* db, const std::string& sql) {
    char* errorMessage = nullptr;
    if (sqlite3_exec(db, sql.c_str(), nullptr, nullptr, &errorMessage) != SQLITE_OK) {
        std::string error = "SQL execution failed: ";
        error += errorMessage;
        sqlite3_free(errorMessage);
        throw std::runtime_error(error);
    }
}

// Function to insert data from a file
void insertDataFromFile(sqlite3* db, const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open file: " + filePath);
    }

    std::string line;
    std::string currentSQLBlock;

    while (std::getline(file, line)) {
        if (line.empty()) continue;
        currentSQLBlock += line;
        if (line.find(';') != std::string::npos) {
            executeSQL(db, currentSQLBlock);
            currentSQLBlock.clear();
        }
    }

    file.close();
    std::cout << "All SQL commands from " << filePath << " executed successfully." << std::endl;
}

// Function to initialize the database
void initializeDatabase() {
    // Check if data is needed
    bool data_need = false;
    const std::string dbFilePath = "registration_system.db";
    if (!std::filesystem::exists(dbFilePath)) {
        data_need = true;
    }

    // Initialize database
    DatabaseConnection* dbConnection = DatabaseConnection::getInstance();
    sqlite3* db = dbConnection->getDB();

    if (data_need) {
        std::cout << "Database file was empty. Initializing...\n";
        const std::string filePath = "SQL_Creation_Data.txt";
        insertDataFromFile(db, filePath);
    } else {
        std::cout << "Database loaded successfully.\n";
    }
}

// Working, but must create new user if not found
void handleLogin(const std::string& username) {
    try {
        UserFactory factory;
        Person* user = factory.createUser(username);
        if (dynamic_cast<Admin*>(user)) {
            std::cout << "Admin" << std::endl;
        } else if (dynamic_cast<Student*>(user)) {
            std::cout << "Student" << std::endl;
        } else {
            std::cerr << "Unknown user type" << std::endl;
            exit(1);
        }
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

// Working
void handleAdminView() {
    try {
        DatabaseConnection* dbConnection = DatabaseConnection::getInstance();
        sqlite3* db = dbConnection->getDB();
        sqlite3_stmt* stmt;

        std::string query = "SELECT course_id, course_description, department, seats, course_time FROM Course;";
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error("Failed to prepare query");
        }

        Json::Value courses(Json::arrayValue);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string courseID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::string department = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));
            int classSize = sqlite3_column_int(stmt, 3);
            std::string schedule = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4));

            // Calculate available seats
            std::string seatQuery = "SELECT COUNT(*) FROM Enrolled WHERE course_id = ?;";
            sqlite3_stmt* seatStmt;
            if (sqlite3_prepare_v2(db, seatQuery.c_str(), -1, &seatStmt, nullptr) != SQLITE_OK) {
                throw std::runtime_error("Failed to prepare seat query");
            }
            sqlite3_bind_text(seatStmt, 1, courseID.c_str(), -1, SQLITE_STATIC);
            sqlite3_step(seatStmt);
            int enrolledCount = sqlite3_column_int(seatStmt, 0);
            sqlite3_finalize(seatStmt);

            int seatsAvailable = classSize - enrolledCount;

            // Add course to JSON
            Json::Value course;
            course["code"] = courseID;
            course["name"] = name;
            course["department"] = department;
            course["class_size"] = classSize;
            course["schedule"] = schedule;
            course["seatsAvailable"] = seatsAvailable;
            courses.append(course);
        }
        sqlite3_finalize(stmt);

        Json::StreamWriterBuilder writer;
        std::cout << Json::writeString(writer, courses) << std::endl;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

// Working
void handleCourseDetail(const std::string& courseCode) {
    try {
        DatabaseConnection* dbConnection = DatabaseConnection::getInstance();
        sqlite3* db = dbConnection->getDB();
        sqlite3_stmt* stmt;

        std::string query = "SELECT user_id FROM Enrolled WHERE course_id = ?;";
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error("Failed to prepare query");
        }
        sqlite3_bind_text(stmt, 1, courseCode.c_str(), -1, SQLITE_STATIC);

        Json::Value users(Json::arrayValue);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string userID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            Json::Value user;
            user["id"] = userID;
            users.append(user);
        }
        sqlite3_finalize(stmt);

        Json::StreamWriterBuilder writer;
        std::cout << Json::writeString(writer, users) << std::endl;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

// Working
void handleStudentSchedule(const std::string& studentID) {
    try {
        DatabaseConnection* dbConnection = DatabaseConnection::getInstance();
        sqlite3* db = dbConnection->getDB();
        sqlite3_stmt* stmt;

        std::string query = R"(
            SELECT c.course_id, c.course_description, c.course_time
            FROM Course c
            JOIN Enrolled e ON c.course_id = e.course_id
            WHERE e.user_id = ?;
        )";
        if (sqlite3_prepare_v2(db, query.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
            throw std::runtime_error("Failed to prepare query");
        }
        sqlite3_bind_text(stmt, 1, studentID.c_str(), -1, SQLITE_STATIC);

        Json::Value courses(Json::arrayValue);
        while (sqlite3_step(stmt) == SQLITE_ROW) {
            std::string courseID = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 0));
            std::string name = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1));
            std::string schedule = reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2));

            Json::Value course;
            course["code"] = courseID;
            course["name"] = name;
            course["schedule"] = schedule;
            courses.append(course);
        }
        sqlite3_finalize(stmt);

        Json::StreamWriterBuilder writer;
        std::cout << Json::writeString(writer, courses) << std::endl;

    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        exit(1);
    }
}

void handleStudentEnroll(const std::string& studentID, const std::string& courseCode) {
    try {
        RegistrationFacade facade;
        UserFactory factory;
        Person* currentUser = factory.createUser(studentID);
        Student* student = dynamic_cast<Student*>(currentUser);
        Course course(courseCode);

        // Call registerCourse and capture the message
        std::string message = facade.registerCourse(*student, course);

        // Check if the message indicates success or failure
        bool isSuccess = message.find("Registration successful") != std::string::npos;

        // Output valid JSON with success status
        std::cout << "{\"success\": " << (isSuccess ? "true" : "false")
                  << ", \"message\": \"" << message << "\"}" << std::endl;

    } catch (const std::exception& e) {
        // Output error message as JSON
        std::cerr << "{\"success\": false, \"message\": \"" << e.what() << "\"}" << std::endl;
        exit(1);
    }
}

// Working but need error handling
void handleStudentDrop(const std::string& studentID, const std::string& courseCode) {
    try {
        RegistrationFacade facade;
        UserFactory factory;
        Person* currentUser = factory.createUser(studentID);
        Student* student = dynamic_cast<Student*>(currentUser);
        Course course(courseCode);

        // Call dropCourse and capture the message
        std::string message = facade.dropCourse(*student, course);

        // Check if the message indicates success or failure
        bool isSuccess = message.find("Drop successful") != std::string::npos;

        // Output valid JSON with success status
        std::cout << "{\"success\": " << (isSuccess ? "true" : "false")
                  << ", \"message\": \"" << message << "\"}" << std::endl;

    } catch (const std::exception& e) {
        // Output error message as JSON
        std::cerr << "{\"success\": false, \"message\": \"" << e.what() << "\"}" << std::endl;
        exit(1);
    }
}

int main(int argc, char* argv[]) {
    try {
        if (argc < 2) {
            std::cerr << "{\"success\": false, \"message\": \"Invalid arguments\"}" << std::endl;
            return 1;
        }

        std::string command = argv[1];
        if (command == "--initialize-db") {
            initializeDatabase();
            std::cout << "{\"success\": true, \"message\": \"Database initialized successfully.\"}" << std::endl;
        } else if (command == "--login" && argc == 3) {
            handleLogin(argv[2]);
        } else if (command == "--admin-view") {
            handleAdminView();
        } else if (command == "--course-detail" && argc == 3) {
            handleCourseDetail(argv[2]);
        } else if (command == "--student-schedule" && argc == 3) {
            handleStudentSchedule(argv[2]);
        } else if (command == "--enroll" && argc == 4) {
            handleStudentEnroll(argv[2], argv[3]);
        } else if (command == "--drop" && argc == 4) {
            handleStudentDrop(argv[2], argv[3]);
        } else {
            std::cerr << "{\"success\": false, \"message\": \"Unknown command\"}" << std::endl;
            return 1;
        }

        return 0;
    } catch (const std::exception& e) {
        std::cerr << "{\"success\": false, \"message\": \"" << e.what() << "\"}" << std::endl;
        return 1;
    }
}