#ifndef ADMIN_H
#define ADMIN_H

#include "Person.h"
#include "Course.h"

class Admin : public Person {
public:
    Admin() = default; // Default constructor
    void viewCourseDetails(const Course& course);
    void viewAllClassLists();
    void viewProfile();  // Implements virtual method
};

#endif // ADMIN_H
