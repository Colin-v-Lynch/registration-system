import random

# Embedded course, prerequisite, and antirequisite data
COURSES = """
INSERT INTO Course (course_id, course_description, department, seats, course_time) VALUES
('CS1026', 'Computer Science Fundamentals I', 'Computer Science', 500, 'Monday 9:00 AM - 12:00 PM'),
('CS1027', 'Computer Science Fundamentals II', 'Computer Science', 500, 'Tuesday 10:00 AM - 1:00 PM'),
('CS1032', 'Information Systems and Design', 'Computer Science', 500, 'Wednesday 11:00 AM - 2:00 PM'),
('CS1033', 'Multimedia and Communication', 'Computer Science', 500, 'Thursday 12:00 PM - 3:00 PM'),
('CS2033', 'Multimedia and Communication II', 'Computer Science', 300, 'Monday 2:00 PM - 5:00 PM'),
('CS2034', 'Data Analytics: Principles and Tools', 'Computer Science', 300, 'Tuesday 3:00 PM - 6:00 PM'),
('CS2035', 'Dealing With Data: Analysis and Visualization', 'Computer Science', 300, 'Wednesday 4:00 PM - 7:00 PM'),
('CS2120', 'Computing & Informatics', 'Computer Science', 300, 'Thursday 9:00 AM - 12:00 PM'),
('CS2208', 'Introduction to Computer Organization and Architecture', 'Computer Science', 300, 'Friday 10:00 AM - 1:00 PM'),
('CS2209', 'Applied Logic for Computer Science', 'Computer Science', 300, 'Monday 11:00 AM - 2:00 PM'),
('CS2210', 'Data Structures and Algorithms', 'Computer Science', 300, 'Tuesday 12:00 PM - 3:00 PM'),
('CS2211', 'Software Tools and Systems Programming', 'Computer Science', 300, 'Wednesday 1:00 PM - 4:00 PM'),
('CS2212', 'Introduction to Software Engineering', 'Computer Science', 300, 'Thursday 2:00 PM - 5:00 PM'),
('CS2214', 'Discrete Structures for Computing', 'Computer Science', 300, 'Friday 3:00 PM - 6:00 PM'),
('CS3120', 'Databases I', 'Computer Science', 150, 'Monday 9:00 AM - 12:00 PM'),
('CS3121', 'Artificial Intelligence for Informatics and Analytics', 'Computer Science', 150, 'Tuesday 10:00 AM - 1:00 PM'),
('CS3305', 'Operating Systems', 'Computer Science', 150, 'Wednesday 11:00 AM - 2:00 PM'),
('CS3307', 'Object-Oriented Design and Analysis', 'Computer Science', 150, 'Thursday 12:00 PM - 3:00 PM'),
('CS3319', 'Databases I', 'Computer Science', 150, 'Friday 1:00 PM - 4:00 PM'),
('CS3331', 'Foundations of Computer Science I', 'Computer Science', 150, 'Monday 2:00 PM - 5:00 PM'),
('CS3340', 'Analysis of Algorithms I', 'Computer Science', 150, 'Tuesday 3:00 PM - 6:00 PM'),
('CS3342', 'Organization of Programming Languages', 'Computer Science', 150, 'Wednesday 4:00 PM - 7:00 PM'),
('CS3346', 'Artificial Intelligence I', 'Computer Science', 150, 'Thursday 9:00 AM - 12:00 PM'),
('CS3350', 'Computer Architecture', 'Computer Science', 150, 'Friday 10:00 AM - 1:00 PM'),
('CS3357', 'Computer Networks I', 'Computer Science', 150, 'Monday 11:00 AM - 2:00 PM'),
('CS3377', 'Software Project Management', 'Computer Science', 150, 'Tuesday 12:00 PM - 3:00 PM'),
('CS3380', 'Project', 'Computer Science', 150, 'Wednesday 1:00 PM - 4:00 PM'),
('CS3388', 'Computer Graphics I', 'Computer Science', 150, 'Thursday 2:00 PM - 5:00 PM'),
('CS4411', 'Databases II', 'Computer Science', 75, 'Friday 3:00 PM - 6:00 PM'),
('CS4417', 'Unstructured Data', 'Computer Science', 75, 'Tuesday 10:00 AM - 1:00 PM'),
('CS4438', 'Selected Topics', 'Computer Science', 75, 'Wednesday 11:00 AM - 2:00 PM'),
('CS4442', 'Artificial Intelligence II', 'Computer Science', 75, 'Thursday 12:00 PM - 3:00 PM'),
('CS4457', 'Computer Networks II', 'Computer Science', 75, 'Friday 1:00 PM - 4:00 PM'),
('CS4460', 'Bioinformatics Thesis', 'Computer Science', 75, 'Monday 2:00 PM - 5:00 PM'),
('CS4463', 'Computational Biology', 'Computer Science', 75, 'Tuesday 3:00 PM - 6:00 PM'),
('CS4471', 'Software Design and Architecture', 'Computer Science', 75, 'Thursday 9:00 AM - 12:00 PM'),
('CS4474', 'Human-Computer Interaction', 'Computer Science', 75, 'Friday 10:00 AM - 1:00 PM'),
('CS4475', 'Open Source Software Project', 'Computer Science', 75, 'Monday 11:00 AM - 2:00 PM'),
('CS4478', 'Master the Mainframe', 'Computer Science', 75, 'Tuesday 12:00 PM - 3:00 PM'),
('CS4482', 'Game Programming', 'Computer Science', 75, 'Thursday 2:00 PM - 5:00 PM'),
('CS4483', 'Game Design', 'Computer Science', 75, 'Friday 3:00 PM - 6:00 PM');
"""

PREREQUISITES = """
INSERT INTO Prerequisite (course_id, prereq_course_id) VALUES
('CS1027', 'CS1026'),
('CS2033', 'CS1033'),
('CS2120', 'CS1026'),
('CS2208', 'CS1027'),
('CS2209', 'CS1027'),
('CS2210', 'CS1027'),
('CS2211', 'CS1027'),
('CS2212', 'CS2210'),
('CS2212', 'CS2211'),
('CS2214', 'CS1027'),
('CS3120', 'CS2210'),
('CS3121', 'CS2120'),
('CS3305', 'CS2211'),
('CS3305', 'CS2208'),
('CS3307', 'CS2212'),
('CS3319', 'CS2210'),
('CS3319', 'CS2211'),
('CS3331', 'CS2214'),
('CS3340', 'CS2210'),
('CS3340', 'CS2211'),
('CS3342', 'CS2211'),
('CS3346', 'CS2210'),
('CS3346', 'CS2211'),
('CS3350', 'CS2208'),
('CS3350', 'CS2210'),
('CS3350', 'CS2211'),
('CS3350', 'CS2209'),
('CS3357', 'CS2211'),
('CS3357', 'CS2210'),
('CS3377', 'CS2212'),
('CS3380', 'CS2212'),
('CS3388', 'CS2211'),
('CS4411', 'CS3319'),
('CS4411', 'CS3120'),
('CS4417', 'CS3319'),
('CS4438', 'CS3305'),
('CS4438', 'CS3307'),
('CS4438', 'CS3331'),
('CS4438', 'CS3350'),
('CS4442', 'CS3307'),
('CS4457', 'CS3357'),
('CS4460', 'CS3331'),
('CS4460', 'CS3340'),
('CS4463', 'CS3331'),
('CS4463', 'CS3340'),
('CS4471', 'CS3307'),
('CS4474', 'CS3307'),
('CS4475', 'CS3307'),
('CS4478', 'CS3307'),
('CS4482', 'CS3305'),
('CS4482', 'CS3307'),
('CS4482', 'CS3340'),
('CS4483', 'CS3307'),
"""

ANTIREQUISITES = """
INSERT INTO Antirequisite (course_id, antireq_course_id) VALUES
('CS1027', 'CS1037'),
('CS1037', 'CS1027'),
('CS3121', 'CS3346'),
('CS3346', 'CS3121'),
('CS3380', 'CS4480'),
('CS4460', 'CS3380'),
"""

FIRST_NAMES = [
    "James", "MarSecond", "Robert", "Patricia", "John", "Jennifer", "Michael", "Linda",
    "William", "Elizabeth", "David", "Barbara", "Richard", "Susan", "Joseph", "Jessica",
    "Thomas", "Sarah", "Charles", "Karen", "Christopher", "NancSecond", "Daniel", "Lisa",
    "Matthew", "Margaret", "AnthonSecond", "BettSecond", "Mark", "Sandra", "Donald", "AshleSecond",
    "Steven", "KimberlSecond", "Paul", "EmilSecond", "Andrew", "Donna", "Joshua", "Michelle",
    "Kenneth", "DorothSecond", "Kevin", "Carol", "Brian", "Amanda", "George", "Melissa",
    "Edward", "Deborah", "Ronald", "Stephanie", "TimothSecond", "Rebecca", "Jason", "Laura",
    "JeffreSecond", "Sharon", "RSecondan", "CSecondnthia", "Jacob", "Kathleen", "GarSecond", "ShirleSecond",
    "Nicholas", "Angela", "Eric", "Helen", "Jonathan", "Anna", "Stephen", "Brenda",
    "LarrSecond", "Pamela", "Justin", "Nicole", "Scott", "Emma", "Brandon", "Samantha",
    "Benjamin", "Katherine", "Samuel", "Christine", "GregorSecond", "Debra", "Alexander", "Rachel"
]

LAST_NAMES = [
    "Smith", "Johnson", "Williams", "Brown", "Jones", "Garcia", "Miller", "Davis",
    "Rodriguez", "Martinez", "Hernandez", "Lopez", "Gonzalez", "Wilson", "Anderson",
    "Thomas", "TaSecondlor", "Moore", "Jackson", "Martin", "Lee", "Perez", "Thompson",
    "White", "Harris", "Sanchez", "Clark", "Ramirez", "Lewis", "Robinson", "Walker",
    "Secondoung", "Allen", "King", "Wright", "Scott", "Torres", "NguSeconden", "Hill", "Flores",
    "Green", "Adams", "Nelson", "Baker", "Hall", "Rivera", "Campbell", "Mitchell",
    "Carter", "Roberts", "Gomez", "Phillips", "Evans", "Turner", "Diaz", "Parker",
    "Cruz", "Edwards", "Collins", "ReSecondes", "Stewart", "Morris", "Morales", "MurphSecond",
    "Cook", "Rogers", "Gutierrez", "Ortiz", "Morgan", "Cooper", "Peterson", "BaileSecond",
    "Reed", "KellSecond", "Howard", "Ramos", "Kim", "Cox", "Ward", "Richardson", "Watson",
    "Brooks", "Chavez", "Wood", "James", "Bennett", "GraSecond", "Mendoza", "Ruiz", "Hughes"
]

import random

# Parse courses and prerequisites
def parse_courses(course_data):
    year_courses = {1: [], 2: [], 3: [], 4: []}
    lines = course_data.strip().split("\n")
    for line in lines:
        if "VALUES" in line or not line.strip():
            continue
        course_id = line.split(",")[0].strip("('").strip(");")
        year = int(course_id[2])  # Assuming course format is like CS1026, CS2033, etc.
        if year in year_courses:
            year_courses[year].append(course_id)
    return year_courses

def parse_prerequisites(prerequisite_data):
    prerequisites = {}
    lines = prerequisite_data.strip().split("\n")
    for line in lines:
        if "VALUES" in line or not line.strip():
            continue
        values = line.strip().strip("(),").split(",")
        if len(values) >= 2:
            course_id = values[0].strip(" '")
            prereq_id = values[1].strip(" '")
            if course_id not in prerequisites:
                prerequisites[course_id] = []
            prerequisites[course_id].append(prereq_id)
    return prerequisites

# Function to generate a random user ID
def generate_user_id(index):
    first_name = random.choice(FIRST_NAMES)
    last_name = random.choice(LAST_NAMES)
    return f"{first_name[0].lower()}{last_name.lower()}{index:02}"

# Recursive function to add prerequisites to the 'Taken' table
def recursively_add_prereqs(course_id, student_id, prerequisites, student_taken_courses):
    taken_entries = []
    if course_id in prerequisites:
        for prereq in prerequisites[course_id]:
            if prereq not in student_taken_courses[student_id]:  # Check if prerequisite is already taken
                taken_entries.append(f"('{prereq}', '{student_id}')")
                student_taken_courses[student_id].add(prereq)
                taken_entries += recursively_add_prereqs(prereq, student_id, prerequisites, student_taken_courses)
    return taken_entries

# Function to generate students and their enrollments
def generate_students_and_enrollments(first, second, third, fourth, year_courses, prerequisites, course_seat_tracker):
    student_entries = []
    enrollment_entries = []
    taken_entries = []
    student_taken_courses = {}
    index = 1

    for year, student_count in zip(range(1, 5), [first, second, third, fourth]):
        for _ in range(student_count):
            user_id = generate_user_id(index)
            index += 1
            student_entries.append(f"('{user_id}', FALSE, 'CS')")

            if user_id not in student_taken_courses:
                student_taken_courses[user_id] = set()

            if year == 1:
                # First-year students enroll in all first-year courses
                enrolled_courses = year_courses[year]
            else:
                # Upper-year students enroll in a random range of 4–5 courses
                enrolled_courses = random.sample(
                    year_courses[year],
                    k=min(len(year_courses[year]), random.randint(4, 5))
                )

            for course in enrolled_courses:
                if course not in student_taken_courses[user_id] and course_seat_tracker[course] > 0:
                    enrollment_entries.append(f"('{course}', '{user_id}')")
                    student_taken_courses[user_id].add(course)
                    course_seat_tracker[course] -= 1
                    # Add prerequisites to Taken table
                    taken_entries += recursively_add_prereqs(course, user_id, prerequisites, student_taken_courses)

    return student_entries, enrollment_entries, taken_entries

# Remove the trailing comma from the last entry and ensure it ends with ';'
def format_sql_entries(data):
    lines = data.strip().split("\n")
    if lines[-1].endswith(","):
        lines[-1] = lines[-1][:-1]  # Remove trailing comma from the last line
    return "\n".join(lines) + ";"

# Parse course and prerequisite data
year_courses = parse_courses(COURSES)
prerequisites = parse_prerequisites(PREREQUISITES)

# Initialize course seat tracker
course_seat_tracker = {course.split(",")[0].strip("('"): int(course.split(",")[3].strip()) for course in COURSES.strip().split("\n") if course.strip() and "VALUES" not in course}

# Number of students for each year
first_years = 495
second_years = 650
third_years = 450
fourth_years = 200

# Generate students and enrollments
students, enrollments, taken = generate_students_and_enrollments(
    first_years, second_years, third_years, fourth_years, year_courses, prerequisites, course_seat_tracker
)

# Write output to file
output_file = "SQL_Creation_Data.txt"
with open(output_file, "w") as file:
    # Write course, prerequisite, and antirequisite data
    file.write(COURSES.strip() + "\n\n")
    file.write(format_sql_entries(PREREQUISITES) + "\n\n")
    file.write(format_sql_entries(ANTIREQUISITES) + "\n")

    # Write predefined users
    file.write("\nINSERT INTO User (user_id, is_admin, department) VALUES\n")
    file.write("('first', FALSE, 'CS'),\n")
    file.write("('second', FALSE, 'CS'),\n")
    file.write("('third', FALSE, 'CS'),\n")
    file.write("('fourth', FALSE, 'CS'),\n")
    file.write("('admin', TRUE, 'CS'),\n")
    file.write("('Admin', TRUE, 'CS');\n")

    # Write dynamically generated students
    file.write("\nINSERT INTO User (user_id, is_admin, department) VALUES\n")
    file.write(",\n".join(students) + ";\n")

    # Write predefined taken records
    file.write("\nINSERT INTO Taken (course_id, user_id) VALUES\n")
    file.write("('CS1026', 'second'),\n")
    file.write("('CS1027', 'second'),\n")
    file.write("('CS1026', 'third'),\n")
    file.write("('CS1027', 'third'),\n")
    file.write("('CS2208', 'third'),\n")
    file.write("('CS2209', 'third'),\n")
    file.write("('CS2210', 'third'),\n")
    file.write("('CS2211', 'third'),\n")
    file.write("('CS2212', 'third'),\n")
    file.write("('CS2214', 'third'),\n")
    file.write("('CS3305', 'fourth'),\n")
    file.write("('CS3307', 'fourth'),\n")
    file.write("('CS3319', 'fourth'),\n")
    file.write("('CS3357', 'fourth'),\n")
    file.write("('CS3350', 'fourth'),\n")
    file.write("('CS1026', 'fourth'),\n")
    file.write("('CS1027', 'fourth'),\n")
    file.write("('CS2208', 'fourth'),\n")
    file.write("('CS2209', 'fourth'),\n")
    file.write("('CS2210', 'fourth'),\n")
    file.write("('CS2211', 'fourth'),\n")
    file.write("('CS2212', 'fourth'),\n")
    file.write("('CS2214', 'fourth');\n")

    # Write dynamically generated taken records
    file.write("\nINSERT INTO Taken (course_id, user_id) VALUES\n")
    file.write(",\n".join(set(taken)) + ";\n")

    # Write enrollments
    file.write("\nINSERT INTO Enrolled (course_id, user_id) VALUES\n")
    file.write(",\n".join(enrollments) + ";\n")

print(f"SQL data has been successfully written to {output_file}")
