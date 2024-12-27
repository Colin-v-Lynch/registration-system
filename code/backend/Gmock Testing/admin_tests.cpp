
#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "Admin.h"
#include "Course.h"

// Mock for Course class
class MockCourse : public Course {
public:
    MOCK_METHOD(std::string, getCourseID, (), (const, override));
    MOCK_METHOD(std::string, getCourseName, (), (const, override));
    MOCK_METHOD(std::string, getCourseTime, (), (const, override));
    MOCK_METHOD(int, getAvailableSeats, (), (const, override));
};

// Test 2.1: View Course Details
TEST(AdminTest, ViewCourseDetailsDisplaysCorrectData) {
    MockCourse mockCourse;
    EXPECT_CALL(mockCourse, getCourseName()).WillOnce(testing::Return("Algorithms"));
    EXPECT_CALL(mockCourse, getCourseID()).WillOnce(testing::Return("CS301"));
    EXPECT_CALL(mockCourse, getCourseTime()).WillOnce(testing::Return("2:00 PM"));
    EXPECT_CALL(mockCourse, getAvailableSeats()).WillOnce(testing::Return(20));

    Admin admin("Admin");

    testing::internal::CaptureStdout();
    admin.viewCourseDetails(mockCourse);
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Course Details for: Algorithms") != std::string::npos);
    EXPECT_TRUE(output.find("Course ID: CS301") != std::string::npos);
    EXPECT_TRUE(output.find("Time: 2:00 PM") != std::string::npos);
    EXPECT_TRUE(output.find("Available Seats: 20") != std::string::npos);
}

// Test 2.2: View Profile
TEST(AdminTest, ViewProfileDisplaysUserID) {
    Admin admin("Admin");

    testing::internal::CaptureStdout();
    admin.viewProfile();
    std::string output = testing::internal::GetCapturedStdout();

    EXPECT_TRUE(output.find("Admin Profile: Admin") != std::string::npos);
}
