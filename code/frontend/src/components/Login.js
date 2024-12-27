import React, { useState } from 'react';
import { useNavigate } from 'react-router-dom';

const Login = () => {
  const [username, setUsername] = useState('');
  const navigate = useNavigate();

  const handleLogin = async () => {
    if (username.trim()) {
      try {
        const response = await fetch('http://127.0.0.1:8000/login', {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify({ username }), // Send username as JSON
        });

        if (!response.ok) {
          throw new Error('Failed to log in. Please try again.');
        }

        const data = await response.json();
        alert(`Welcome, ${username}! You are logged in as ${data.userType}.`);
        localStorage.setItem('userType', data.userType);
        localStorage.setItem('username', username);

        // Navigate to the appropriate page based on user type
        if (data.userType === 'Admin') {
          navigate('/admin');
        } else {
          navigate('/student/schedule');
        }
      } catch (error) {
        alert('Error during login: ' + error.message);
      }
    } else {
      alert('Please enter your username.');
    }
  };

  return (
    <div className="login-container">
      <h1>Login</h1>
      <input
        type="text"
        placeholder="Enter your username"
        value={username}
        onChange={(e) => setUsername(e.target.value)}
        className="login-input"
      />
      <button onClick={handleLogin} className="login-button">
        Login
      </button>
    </div>
  );
};

export default Login;
