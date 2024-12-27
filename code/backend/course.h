#ifndef COURSE_H
#define COURSE_H

#include <string>
#include <vector>

class Course {
private:
    std::string courseID;
    std::string name;
    std::vector<std::string> prerequisites;
    std::vector<std::string> antirequisites;
    std::vector<std::string> enrolledStudents;
    std::string courseTime;
    int totalseats;
    int availableSeats;

public:

    Course(const std::string& courseID);

    // Getters and setters for all attributes
    std::string getCourseID() const;
    void setCourseID(const std::string& id);
    
    std::string getCourseName() const;
    
    std::string getCourseTime() const;
    
    int getAvailableSeats() const;
    void setAvailableSeats(int seats);

    std::vector<std::string> getPrerequisites() const;
    std::vector<std::string> getAntirequisites() const;
    std::vector<std::string> getEnrolledStudents() const;

    void printDetails() const;

};

#endif // COURSE_H
