
#ifndef STUDENT_H
#define STUDENT_H

#include "Person.h"
#include "Course.h"
#include <vector>
#include <string>

class Student : public Person {
private:
    std::vector<std::string> enrolledCourses;
    std::vector<std::string> takenCourses;

public:
    Student(const std::string& userID);  // Declare constructor with userID
    void viewEnrolledCourses();
    const std::vector<std::string>& getEnrolledCourses() const;
    const std::vector<std::string>& getTakenCourses() const;
    void viewSchedule() const;
    void viewProfile();
};

#endif // STUDENT_H
