import React from 'react';
import { useNavigate, useLocation } from 'react-router-dom';

const Header = () => {
  const navigate = useNavigate();
  const location = useLocation(); // Get the current path

  const handleMainMenu = () => {
    if (location.pathname === '/student/enroll') {
      navigate('/student/schedule'); // Navigate to the schedule if on course enrollment
    } else if (location.pathname === '/student/schedule') {
      navigate('/student/enroll'); // Navigate to course enrollment if on the schedule
    } else {
      const userType = localStorage.getItem('userType');
      if (userType === 'Admin') {
        navigate('/admin');
      } else {
        navigate('/student/enroll');
      }
    }
  };

  const handleLogout = () => {
    localStorage.removeItem('userType');
    localStorage.removeItem('username');
    navigate('/login');
  };

  // Redirect to login if the user is not logged in
  React.useEffect(() => {
    const username = localStorage.getItem('username');
    if (!username) {
      navigate('/login');
    }
  }, [navigate]);

  // Determine the label for the Main Menu button
  const mainMenuLabel =
    location.pathname === '/student/enroll'
      ? 'View Schedule'
      : location.pathname === '/student/schedule'
      ? 'Course Enrollment'
      : 'Main Menu';

  return (
    <header className="navbar">
      {/* Placeholder for logo */}
      <div className="logo">
        <img
          src="/westernlogo.svg" // Path to logo in the public folder
          alt="Western Logo"
          style={{ height: '80px', cursor: 'pointer' }} // Adjust size and appearance
        />
      </div>
      {/* Buttons */}
      <div>
        <button onClick={handleMainMenu} className="header-button">
          {mainMenuLabel}
        </button>
        <button onClick={handleLogout} className="header-button">
          Log Out
        </button>
      </div>
    </header>
  );
};

export default Header;
