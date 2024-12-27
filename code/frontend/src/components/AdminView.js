import React, { useState, useEffect } from 'react';
import { useNavigate } from 'react-router-dom';
import Header from './Header'; // Import the Header component

const AdminView = () => {
  const [courses, setCourses] = useState([]);
  const [sortConfig, setSortConfig] = useState({ key: null, direction: 'ascending' });
  const [isAdmin, setIsAdmin] = useState(true); // State to check if the user is an admin
  const navigate = useNavigate();

  useEffect(() => {
    const userType = localStorage.getItem('userType');
    if (userType !== 'Admin') {
      setIsAdmin(false); // Set isAdmin to false if user is not an admin
      return;
    }

    const fetchCourses = async () => {
      try {
        const response = await fetch('http://127.0.0.1:8000/admin/view');
        if (!response.ok) throw new Error('Failed to fetch courses');

        const data = await response.json();
        setCourses(data);
      } catch (error) {
        alert('Error fetching courses: ' + error.message);
      }
    };

    fetchCourses();
  }, []);

  const sortedCourses = [...courses].sort((a, b) => {
    if (sortConfig.key) {
      const aValue = a[sortConfig.key];
      const bValue = b[sortConfig.key];
      if (aValue < bValue) return sortConfig.direction === 'ascending' ? -1 : 1;
      if (aValue > bValue) return sortConfig.direction === 'ascending' ? 1 : -1;
      return 0;
    }
    return 0;
  });

  const handleSort = (key) => {
    setSortConfig((prevConfig) => ({
      key,
      direction: prevConfig.key === key && prevConfig.direction === 'ascending' ? 'descending' : 'ascending',
    }));
  };

  const handleShowDetails = (course) => {
    navigate(`/admin/class-details`, { state: { course } });
  };

  if (!isAdmin) {
    return (
      <div>
        <Header />
        <div className="container">
          <h1>Error: Admin access only</h1>
          <p>You do not have the required permissions to view this page.</p>
        </div>
      </div>
    );
  }

  return (
    <div>
      <Header /> {/* Add the Header */}
      <div className="container">
        <h1>Admin View</h1>
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
                  <button onClick={() => handleShowDetails(course)}>Course Details</button>
                </td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>
    </div>
  );
};

export default AdminView;
