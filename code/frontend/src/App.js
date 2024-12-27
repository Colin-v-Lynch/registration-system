import React from 'react';
import { BrowserRouter as Router, Routes, Route, Navigate } from 'react-router-dom';
import Login from './components/Login';
import AdminView from './components/AdminView';
import AdminClassDetails from './components/AdminClassDetails';
import StudentSchedule from './components/StudentSchedule';
import CourseEnrollment from './components/CourseEnrollment';

const ProtectedRoute = ({ children, redirectPath = '/login' }) => {
  const username = localStorage.getItem('username'); // Check if the user is logged in
  if (!username) {
    return <Navigate to={redirectPath} replace />; // Redirect to login if not logged in
  }
  return children; // Render the protected component if logged in
};

const RoleBasedRedirect = ({ adminComponent, studentComponent }) => {
  const userType = localStorage.getItem('userType');
  if (userType === 'Admin') {
    return adminComponent;
  } else if (userType === 'Student') {
    return studentComponent;
  }
  return <Navigate to="/login" replace />; // Redirect to login if userType is invalid or missing
};

const App = () => {
  return (
    <Router>
      <Routes>
        {/* Login Route */}
        <Route path="/login" element={<Login />} />

        {/* Admin Routes */}
        <Route
          path="/admin"
          element={
            <ProtectedRoute>
              <RoleBasedRedirect adminComponent={<AdminView />} />
            </ProtectedRoute>
          }
        />
        <Route
          path="/admin/class-details"
          element={
            <ProtectedRoute>
              <RoleBasedRedirect adminComponent={<AdminClassDetails />} />
            </ProtectedRoute>
          }
        />

        {/* Student Routes */}
        <Route
          path="/student/schedule"
          element={
            <ProtectedRoute>
              <RoleBasedRedirect studentComponent={<StudentSchedule />} />
            </ProtectedRoute>
          }
        />
        <Route
          path="/student/enroll"
          element={
            <ProtectedRoute>
              <RoleBasedRedirect studentComponent={<CourseEnrollment />} />
            </ProtectedRoute>
          }
        />

        {/* Default Route */}
        <Route
          path="*"
          element={
            <ProtectedRoute>
              <RoleBasedRedirect
                adminComponent={<Navigate to="/admin" replace />}
                studentComponent={<Navigate to="/student/enroll" replace />}
              />
            </ProtectedRoute>
          }
        />
      </Routes>
    </Router>
  );
};

export default App;
