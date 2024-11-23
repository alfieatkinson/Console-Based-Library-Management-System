# Library Management System

## Overview

This project is a console-based library management system written in C++. It supports adding, removing, and searching for books within a library.

## Features

- Add new books to the library
- Remove books from the library
- Search for books by title or author
- Display all books in the library

## Directory Structure

Library-Management-System/
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
    └── UserTest.cpp