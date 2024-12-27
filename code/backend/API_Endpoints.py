from fastapi import FastAPI, HTTPException
from pydantic import BaseModel
import subprocess
import json
import os
from typing import List
from fastapi import FastAPI
from fastapi.middleware.cors import CORSMiddleware

app = FastAPI()


# Allow all origins (disable CORS restrictions)
app.add_middleware(
    CORSMiddleware,
    allow_origins=["*"],  # Allows all origins
    allow_credentials=True,
    allow_methods=["*"],  # Allows all HTTP methods
    allow_headers=["*"],  # Allows all HTTP headers
)

# Models for request/response data

class LoginRequest(BaseModel):
    username: str


class LoginResponse(BaseModel):
    userType: str  # "Admin" or "Student"


class Course(BaseModel):
    code: str
    name: str
    department: str
    class_size: int
    schedule: str
    seatsAvailable: int


class EnrolledUser(BaseModel):
    id: str

# Define the schema for the request body
class EnrollRequest(BaseModel):
    student_id: str
    course_code: str

class EnrolledCourse(BaseModel):
    code: str
    name: str
    schedule: str
    
# Endpoints
@app.get("/")
def root():
    return {"Backend is running"}

#Working But needs a way to add a new user
@app.post("/login", response_model=LoginResponse)
def login(request: LoginRequest):
    username = request.username

    # Call the C++ executable to determine the user's type
    try:
        result = subprocess.run(["./API_Endpoints", "--login", username], text=True, capture_output=True)
        if result.returncode != 0:
            raise HTTPException(status_code=500, detail=result.stderr.strip())

        user_type = result.stdout.strip()  # "Admin" or "Student"
        return {"userType": user_type}
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

#Working
@app.get("/admin/view", response_model=List[Course])
def admin_view():
    try:
        # Call the renamed C++ executable to fetch all courses
        result = subprocess.run(["./API_Endpoints", "--admin-view"], text=True, capture_output=True)
        if result.returncode != 0:
            raise HTTPException(status_code=500, detail=result.stderr.strip())
        
        # Parse result into JSON format
        courses = eval(result.stdout.strip())  # Assuming the C++ outputs JSON-like string
        return courses
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

#Working
@app.get("/course/{course_code}/detail", response_model=List[EnrolledUser])
def course_detail(course_code: str):
    try:
        # Call the renamed C++ executable to fetch all users enrolled in the course
        result = subprocess.run(["./API_Endpoints", "--course-detail", course_code], text=True, capture_output=True)
        if result.returncode != 0:
            raise HTTPException(status_code=500, detail=result.stderr.strip())
        
        # Parse result into JSON format
        users = eval(result.stdout.strip())  # Assuming the C++ outputs JSON-like string
        return users
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

#Working
@app.get("/student/{student_id}/schedule", response_model=List[EnrolledCourse])
def student_schedule(student_id: str):
    try:
        # Call the renamed C++ executable to fetch the student's schedule
        result = subprocess.run(["./API_Endpoints", "--student-schedule", student_id], text=True, capture_output=True)
        if result.returncode != 0:
            raise HTTPException(status_code=500, detail=result.stderr.strip())
        
        # Parse result into JSON format
        schedule = eval(result.stdout.strip())  # Assuming the C++ outputs JSON-like string
        return schedule
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

#working
@app.post("/student/enroll")
def student_enroll(student_id: str, course_code: str):
    try:
        # Call the C++ executable to enroll the student
        result = subprocess.run(
            ["./API_Endpoints", "--enroll", student_id, course_code],
            text=True,
            capture_output=True
        )

        if result.returncode != 0:
            raise HTTPException(status_code=500, detail=result.stderr.strip())

        # Parse and return the JSON output from the C++ program
        message = json.loads(result.stdout.strip())
        return message

    except json.JSONDecodeError:
        raise HTTPException(status_code=500, detail="Invalid JSON response from the backend.")
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))

#Working
@app.post("/student/drop")
def student_drop(student_id: str, course_code: str):
    try:
        # Call the C++ executable to drop the student from the course
        result = subprocess.run(
            ["./API_Endpoints", "--drop", student_id, course_code],
            text=True,
            capture_output=True
        )

        if result.returncode != 0:
            raise HTTPException(status_code=500, detail=result.stderr.strip())

        # Parse and return the JSON output from the C++ program
        message = json.loads(result.stdout.strip())  # Parse JSON string
        return message  # Return as-is without wrapping
    
    except Exception as e:
        raise HTTPException(status_code=500, detail=str(e))
