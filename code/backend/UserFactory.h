#ifndef USERFACTORY_H
#define USERFACTORY_H

#include "Person.h"
#include <string>

class UserFactory {
public:
    Person* createUser(const std::string& userID);
};

#endif // USERFACTORY_H
