import React, { useState, useEffect } from 'react';
import Header from './Header'; // Import the Header component
import { useNavigate } from 'react-router-dom';

const CourseEnrollment = () => {
  const [courses, setCourses] = useState([]); // Stores the list of courses
  const [loading, setLoading] = useState(true); // Loading state for API calls
  const [error, setError] = useState(null); // Error state
  const [sortConfig, setSortConfig] = useState({ key: null, direction: 'ascending' }); // Sorting configuration
  const [isStudent, setIsStudent] = useState(true); // State to check if the user is a student
  const navigate = useNavigate();

  // Fetch courses when the component loads
  useEffect(() => {
    const userType = localStorage.getItem('userType');
    if (userType !== 'Student') {
      setIsStudent(false); // Set isStudent to false if the user is not a student
      return;
    }

    const fetchCourses = async () => {
      try {
        const response = await fetch('http://127.0.0.1:8000/admin/view');
        if (!response.ok) throw new Error('Failed to fetch courses');

        const data = await response.json();
        setCourses(data); // Update the courses state with fetched data
        setLoading(false); // Set loading to false after data is fetched
      } catch (error) {
        setError(error.message); // Set error state
        setLoading(false); // Set loading to false even if there is an error
      }
    };

    fetchCourses();
  }, []);

  // Handles enrollment in a course
  const handleEnroll = async (courseCode) => {
    const studentId = localStorage.getItem("username"); // Retrieve the logged-in student's ID
    if (!studentId) {
      alert("No student ID found. Please log in.");
      return;
    }

    try {
      // Construct the URL with query parameters
      const url = `http://127.0.0.1:8000/student/enroll?student_id=${encodeURIComponent(
        studentId
      )}&course_code=${encodeURIComponent(courseCode)}`;

      // Call the backend API for enrollment
      const response = await fetch(url, {
        method: "POST",
        headers: { "Content-Type": "application/json" },
      });

      if (!response.ok) {
        const errorData = await response.json();
        throw new Error(errorData.detail || "Enrollment failed");
      }

      const data = await response.json(); // Parse the successful response JSON

      // Display the API message as an alert
      alert(data.message);

      // Update the course list only if enrollment was successful
      if (data.success) {
        setCourses((prevCourses) =>
          prevCourses.map((course) =>
            course.code === courseCode && course.seatsAvailable > 0
              ? { ...course, seatsAvailable: course.seatsAvailable - 1 }
              : course
          )
        );
      }
    } catch (error) {
      // Handle any errors (either from the API or network issues)
      alert(`Error enrolling in course: ${error.message}`);
    }
  };

  // Handle sorting
  const handleSort = (key) => {
    setSortConfig((prevConfig) => {
      const direction =
        prevConfig.key === key && prevConfig.direction === 'ascending'
          ? 'descending'
          : 'ascending';
      return { key, direction };
    });
  };

  const sortedCourses = [...courses].sort((a, b) => {
    if (sortConfig.key) {
      const aValue = a[sortConfig.key];
      const bValue = b[sortConfig.key];

      if (aValue < bValue) {
        return sortConfig.direction === 'ascending' ? -1 : 1;
      }
      if (aValue > bValue) {
        return sortConfig.direction === 'ascending' ? 1 : -1;
      }
      return 0;
    }
    return 0;
  });

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

  // Render loading state
  if (loading) {
    return (
      <div>
        <Header />
        <p>Loading courses...</p>
      </div>
    );
  }

  // Render error state
  if (error) {
    return (
      <div>
        <Header />
        <p>Error fetching courses: {error}</p>
      </div>
    );
  }

  // Render course table
  return (
    <div>
      <Header /> {/* Add the Header */}
      <div className="container">
        <h1>Course Enrollment</h1>
        <table className="table course-table">
          <thead>
            <tr>
              <th onClick={() => handleSort('code')}>Code</th>
              <th onClick={() => handleSort('name')}>Name</th>
              <th onClick={() => handleSort('department')}>Department</th>
              <th onClick={() => handleSort('class_size')}>Class Size</th>
              <th onClick={() => handleSort('seatsAvailable')}>Seats Available</th>
              <th onClick={() => handleSort('schedule')}>Schedule</th>
              <th>Action</th>
            </tr>
          </thead>
          <tbody>
            {sortedCourses.map((course) => (
              <tr key={course.code}>
                <td>{course.code}</td>
                <td>{course.name}</td>
                <td>{course.department}</td>
                <td>{course.class_size}</td>
                <td>{course.seatsAvailable}</td>
                <td>{course.schedule}</td>
                <td>
                  <button
                    onClick={() => handleEnroll(course.code)}
                    disabled={course.seatsAvailable <= 0} // Disable button if course is full
                  >
                    {course.seatsAvailable > 0 ? 'Enroll In Course' : 'Full'}
                  </button>
                </td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>
    </div>
  );
};

export default CourseEnrollment;
