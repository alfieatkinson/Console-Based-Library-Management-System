# Library Management System

## Overview

This project is a console-based library management system written in C++. It supports adding, removing, and searching for books within a library. The system is designed to be extensible, allowing for the addition of more features such as user management, networking capabilities, and persistence with a database.

## Features

- Add new books to the library
- Remove books from the library
- Search for books by title or author
- Display all books in the library

## Directory Structure

```Library-Management-System/
├── CMakeLists.txt  
├── doc/  
│   └── Doxyfile  
├── include/  
│   ├── Application.hpp  
│   ├── Book.hpp  
│   ├── Database.hpp  
│   ├── Library.hpp  
│   ├── Multithreading.hpp  
│   ├── Networking.hpp  
│   ├── Persistence.hpp  
│   └── User.hpp  
├── lib/  
│   ├── Book.cpp  
│   ├── Database.cpp  
│   ├── Library.cpp  
│   └── User.cpp  
├── src/  
│   ├── Application.cpp  
│   ├── main.cpp  
│   ├── Multithreading.cpp  
│   ├── Networking.cpp  
│   └── Persistence.cpp  
└── tests/  
    ├── BookTest.cpp  
    ├── DatabaseTest.cpp  
    ├── LibraryTest.cpp  
    ├── MultithreadingTest.cpp  
    ├── NetworkingTest.cpp  
    ├── PersistenceTest.cpp  
    └── UserTest.cpp```

## CMake Setup

This project uses CMake for the build process. To build the project, follow these steps:

1. Create a `build/` directory and navigate into it:
    ```bash
    mkdir build
    cd build
    ```

2. Run CMake to configure the project:
    ```bash
    cmake ..
    ```

3. Build the project:
    ```bash
    make
    ```

4. Optionally, you can run tests using:
    ```bash
    make test
    ```

## Documentation

This project uses [Doxygen](https://www.doxygen.nl/) for generating documentation. If you have Doxygen installed, you can generate the documentation by running the following command from the project root directory:

```bash
make doc