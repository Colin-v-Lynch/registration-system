import React, { useState, useEffect } from 'react';
import Header from './Header'; // Import the Header component
import { useNavigate } from 'react-router-dom';

const StudentSchedule = () => {
  const [enrolledCourses, setEnrolledCourses] = useState([]);
  const [isStudent, setIsStudent] = useState(true); // State to check if the user is a student
  const navigate = useNavigate();

  useEffect(() => {
    const userType = localStorage.getItem('userType');
    if (userType !== 'Student') {
      setIsStudent(false); // Set isStudent to false if the user is not a student
      return;
    }

    const studentId = localStorage.getItem('username'); // Get the student ID from localStorage or a global state
    if (!studentId) {
      alert('No student ID found. Please log in.');
      navigate('/login');
      return;
    }

    const fetchSchedule = async () => {
      try {
        const response = await fetch(`http://127.0.0.1:8000/student/${studentId}/schedule`);
        if (!response.ok) throw new Error('Failed to fetch student schedule');

        const data = await response.json();
        setEnrolledCourses(data);
      } catch (error) {
        alert('Error fetching schedule: ' + error.message);
      }
    };

    fetchSchedule();
  }, [navigate]);

  const handleDropCourse = async (courseCode) => {
    const studentId = localStorage.getItem('username');
    if (!studentId) {
      alert('No student ID found. Please log in.');
      navigate('/login');
      return;
    }

    try {
      // Construct the URL with query parameters
      const url = `http://127.0.0.1:8000/student/drop?student_id=${studentId}&course_code=${courseCode}`;
      const response = await fetch(url, {
        method: 'POST', // Use POST as specified by the endpoint
      });

      if (!response.ok) throw new Error('Failed to drop course');

      const result = await response.json();
      alert(result.message || 'Course dropped successfully');

      // Refresh the enrolled courses list
      setEnrolledCourses((prevCourses) =>
        prevCourses.filter((course) => course.code !== courseCode)
      );
    } catch (error) {
      alert('Error dropping course: ' + error.message);
    }
  };

  const daysOfWeek = ['Monday', 'Tuesday', 'Wednesday', 'Thursday', 'Friday'];
  const scheduleByDay = daysOfWeek.reduce((acc, day) => {
    acc[day] = enrolledCourses.filter((course) => course.schedule.startsWith(day));
    return acc;
  }, {});

  if (!isStudent) {
    return (
      <div>
        <Header />
        <div className="container">
          <h1>Error: Student access only</h1>
          <p>You do not have the required permissions to view this page.</p>
        </div>
      </div>
    );
  }

  return (
    <div>
      <Header /> {/* Add the Header */}
      <div className="student-schedule-container">
        <h1>Student Schedule</h1>

        {/* List View */}
        <div className="schedule-list">
          <h2>List View</h2>
          <ul>
            {enrolledCourses.map((course) => (
              <li
                key={course.code}
                style={{ display: 'flex', justifyContent: 'space-between', alignItems: 'center' }}
              >
                <div>
                  <strong>{course.name}</strong> ({course.code}) - {course.schedule}
                </div>
                <button onClick={() => handleDropCourse(course.code)} style={{ marginLeft: '10px' }}>
                  Drop
                </button>
              </li>
            ))}
          </ul>
        </div>

        {/* Calendar View */}
        <div className="schedule-calendar">
          <h2>Calendar View</h2>
          <div className="calendar-grid">
            {daysOfWeek.map((day) => (
              <div key={day} className="calendar-day">
                <h3>{day}</h3>
                {scheduleByDay[day].length > 0 ? (
                  scheduleByDay[day].map((course) => (
                    <div key={course.code} className="calendar-course">
                      <strong>{course.name}</strong> ({course.schedule.split(' ')[1]})
                    </div>
                  ))
                ) : (
                  <p>No classes</p>
                )}
              </div>
            ))}
          </div>
        </div>
      </div>
    </div>
  );
};

export default StudentSchedule;
