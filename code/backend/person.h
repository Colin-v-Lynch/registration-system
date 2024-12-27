#ifndef PERSON_H
#define PERSON_H

#include <string>

class Person {
protected:
    std::string userID;
    std::string department;

public:
    virtual void viewProfile() = 0;  // Pure virtual function

    // Getter and setter for userID
    std::string getID() const {
        return userID;
    }
    // Optional: Setter for userID (if necessary)
    void setID(const std::string& id) {
        userID = id;

    }
    // Getter and setter for name
    std::string getDepartmetn() const {
        return department;
    }
};

#endif // PERSON_H
