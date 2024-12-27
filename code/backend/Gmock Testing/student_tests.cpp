
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Student.h"
#include "DatabaseConnection.h"
#include "Course.h"

// Mock for DatabaseConnection
class MockDatabaseConnection : public DatabaseConnection {
public:
    MOCK_METHOD(sqlite3*, getDB, (), (override));
};

// Mock for Course class
class MockCourse : public Course {
public:
    MOCK_METHOD(std::string, getCourseID, (), (const, override));
    MOCK_METHOD(std::string, getCourseName, (), (const, override));
    MOCK_METHOD(std::string, getCourseTime, (), (const, override));
    MOCK_METHOD(int, getAvailableSeats, (), (const, override));
};

// Test 1.1: Constructor Initialization
TEST(StudentTest, ConstructorFetchesUserData) {
    MockDatabaseConnection mockDB;
    sqlite3* mockDBPointer = nullptr;
    EXPECT_CALL(mockDB, getDB()).WillRepeatedly(testing::Return(mockDBPointer));

    Student student("S12345");
    EXPECT_EQ(student.getEnrolledCourses().size(), 0);
    EXPECT_EQ(student.getTakenCourses().size(), 0);
}

// Test 1.2: View Enrolled Courses
TEST(StudentTest, ViewEnrolledCourses) {
    Student student("S12345");
    std::vector<std::string> enrolled = {"CS101", "CS102"};
    student.getEnrolledCourses() = enrolled;

    testing::internal::CaptureStdout();
    student.viewEnrolledCourses();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Enrolled Course: CS101") != std::string::npos);
    EXPECT_TRUE(output.find("Enrolled Course: CS102") != std::string::npos);
}

// Test 1.3: View Schedule
TEST(StudentTest, ViewScheduleDisplaysCorrectCourses) {
    MockCourse mockCourse;
    EXPECT_CALL(mockCourse, getCourseID()).WillRepeatedly(testing::Return("CS101"));
    EXPECT_CALL(mockCourse, getCourseTime()).WillRepeatedly(testing::Return("10:00 AM"));

    Student student("S12345");
    std::vector<std::string> enrolled = {"CS101"};
    student.getEnrolledCourses() = enrolled;

    testing::internal::CaptureStdout();
    student.viewSchedule();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("CS101 at 10:00 AM") != std::string::npos);
}
