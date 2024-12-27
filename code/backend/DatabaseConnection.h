// This is working
#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <sqlite3.h>
#include <string>

class DatabaseConnection {
private:
    static DatabaseConnection* instance;
    sqlite3* db;

    DatabaseConnection();  // Private constructor
    ~DatabaseConnection();  // Destructor

    void initializeSchema();  // New method to initialize the schema

public:
    static DatabaseConnection* getInstance();
    sqlite3* getDB();
};

#endif // DATABASECONNECTION_H
