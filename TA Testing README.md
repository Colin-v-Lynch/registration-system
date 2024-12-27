# Instalation Instructions
 NOTE: This is for deliverable 3, if you are looking for deliverable 2 please check the deliverable 2 brach or click here: https://github.com/UWO-CS-3307/project-deliverable-2-registration-system/tree/Deliverable-2 
## C++ (Optional as API_Endpoints.cpp is already compiled)
Requirments (You can skip to step 6 if you just want to run the compiled program)
- Download Msys2 to get the c++ compiler (https://www.msys2.org/)
- Download Sqlite3.
- Use VSCode CodeRunner addon (Not needed but was tested with)

1. Install Msys2
2. Install mingw from within Msys2

```
pacman -S mingw-w64-x86_64-gcc
```

3. Install mingw from within Msys2 (make sure to add to enviromental variables)
```
C:\msys64\mingw64\bin
```
4. Install sqlite3 and json extension from within Msys2

```
pacman -S mingw-w64-x86_64-sqlite3
```

```
pacman -S mingw-w64-x86_64-jsoncpp
```

5. Run API_Endpoints.cpp with code runner extension (This will build it), and enjoy the registration system.

6. If the database is empty or not there, delete it and run the command below to initiazlize it.

```
./API_Endpoints --initialize-db
```


## API Endpoints (Python Fast API) (Note: You may want to create a virtual enviroment incase of in compatabilitites)

1. Naviagate to code/backend

```
cd \project-deliverable-2-registration-system\code\backend
```

2. Install FastAPI

```
pip install fastapi pydantic uvicorn
```

3. Run the backend server

```
uvicorn API_Endpoints:app --reload
```

4. Ensure backend is running here: http://localhost:8000/ , to view endpoints go here: http://localhost:8000/docs

## React.js frontend
1. Make sure Node.js is installed, you can get it here: https://nodejs.org/en

2. Make sure node is a enviromental variable, to test this run

```
node --version
```

3. Navigate to the frontend

```
cd \project-deliverable-2-registration-system\code\frontend
```

4. Install dependencies

```
npm install
```

5. Run the development enviroment

```
npm start
```

6. Open on web brower here: http://localhost:3000/ (Was tested with google chrome)


# Testing instructions

API_Endpoints.exe is a precompiled cpp code which runs the API calls passed from API_endpoints.py

To test admin functionality please login with "admin" this is our demo user.

To test student functionality I have add 4 users to the DB "first", "second", "thrid", "fourth". 

They have taken the required prequesties so that you can test they can enroll into the respective courses.

(Use thrid to test enrollement in third year course, etc)

Courses Taken

first: has taken no course

second: CS1026, CS1027

thrid: CS2208, CS2209, CS2210, CS2211, CS2212, CS2214

fourth: CS3305, CS3307, CS3319, CS3350
