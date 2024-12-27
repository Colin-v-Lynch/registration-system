#ifndef REGISTRATIONFACADE_H
#define REGISTRATIONFACADE_H

#include "Student.h"
#include "Course.h"
#include <string>

class RegistrationFacade {
public:
    void createUser(const std::string&  userName);
    std::string registerCourse(Student& student, Course& course);
    std::string dropCourse(Student& student, Course& course); 

private:
    bool hasCompletedPrerequisites(const Student& student, const Course& course);
    bool hasViolatedAntirequisites(const Student& student, const Course& course);
    bool isAlreadyEnrolled(const Student& student, const Course& course);
};

#endif // REGISTRATIONFACADE_H
