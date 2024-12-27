import React, { useState, useEffect } from 'react';
import { useLocation, useNavigate } from 'react-router-dom';
import Header from './Header'; // Import the Header component

const AdminClassDetails = () => {
  const location = useLocation();
  const navigate = useNavigate();
  const { course } = location.state || {}; // Handle missing state gracefully
  const [registeredUsers, setRegisteredUsers] = useState([]);
  const [isAdmin, setIsAdmin] = useState(true); // State to check if the user is an admin

  useEffect(() => {
    const userType = localStorage.getItem('userType');
    if (userType !== 'Admin') {
      setIsAdmin(false); // Set isAdmin to false if user is not an admin
      return;
    }

    if (!course) {
      alert('Course details not found. Redirecting to Admin View.');
      navigate('/admin'); // Redirect to Admin View if course details are missing
      return;
    }

    const fetchRegisteredUsers = async () => {
      try {
        const response = await fetch(`http://127.0.0.1:8000/course/${course.code}/detail`);
        if (!response.ok) throw new Error('Failed to fetch registered users');

        const data = await response.json();
        setRegisteredUsers(data);
      } catch (error) {
        alert('Error fetching registered users: ' + error.message);
      }
    };

    fetchRegisteredUsers();
  }, [course, navigate]);

  const [sortConfig, setSortConfig] = useState({ key: null, direction: 'ascending' });

  const sortedUsers = [...registeredUsers].sort((a, b) => {
    const aValue = a[sortConfig.key];
    const bValue = b[sortConfig.key];

    if (sortConfig.key) {
      if (aValue < bValue) return sortConfig.direction === 'ascending' ? -1 : 1;
      if (aValue > bValue) return sortConfig.direction === 'ascending' ? 1 : -1;
    }
    return 0;
  });

  const handleSort = (key) => {
    setSortConfig((prevConfig) => ({
      key,
      direction: prevConfig.key === key && prevConfig.direction === 'ascending' ? 'descending' : 'ascending',
    }));
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

  if (!course) {
    return (
      <div>
        <Header />
        <div className="container">
          <h1>Error: Course not found</h1>
          <p>Redirecting to the Admin View...</p>
        </div>
      </div>
    );
  }

  return (
    <div>
      <Header /> {/* Add the Header */}
      <div className="container">
        <h1>Course Details</h1>
        <div className="card">
          <p><strong>Code:</strong> {course.code}</p>
          <p><strong>Name:</strong> {course.name}</p>
          <p><strong>Department:</strong> {course.department}</p>
          <p><strong>Class Size:</strong> {course.class_size}</p>
          <p><strong>Seats Available:</strong> {course.seatsAvailable}</p>
          <p><strong>Schedule:</strong> {course.schedule}</p>
        </div>

        <h2>Registered Users</h2>
        <table className="table user-table">
          <thead>
            <tr>
              <th onClick={() => handleSort('id')}>ID</th>
            </tr>
          </thead>
          <tbody>
            {sortedUsers.map((user) => (
              <tr key={user.id}>
                <td>{user.id}</td>
              </tr>
            ))}
          </tbody>
        </table>
      </div>
    </div>
  );
};

export default AdminClassDetails;
