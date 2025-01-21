# Library Management System

This project is submitted in partial fulfilment of the Degree of **Master of Science in Computer Science**.

## Project Overview

This project is a console-based library management system written in C++. It supports adding, removing, and searching for books within a library. The system is designed to be extensible, allowing for the addition of more features such as user management, networking capabilities, and persistence with a database.

### Abstract

*Insert abstract here with link to final report.*

## Features

- Add new books to the library
- Remove books from the library
- Search for books by title or author
- Display all books in the library

## Directory Structure

```
Library-Management-System/
├── CMakeLists.txt              # Configuration for building the project
├── doc/                        # Documentation (e.g., Doxygen configuration)
│   └── Doxyfile
├── include/                    # Header files for classes and utilities
│   ├── Application.hpp         # Manages application flow (start, login/logout)
│   ├── Book.hpp                # Represents book data and operations
│   ├── Database.hpp            # Handles data loading and saving
│   ├── Library.hpp             # Manages library operations (book/user management)
│   ├── Menu.hpp                # Provides menu display and actions
│   ├── Multithreading.hpp      # Contains utilities for multithreading
│   ├── Networking.hpp          # Supports networking functionalities
│   ├── Persistence.hpp         # Manages data persistence to/from files
│   ├── Transaction.hpp         # Represents borrow/return transactions
│   └── User.hpp                # Represents user data and operations
├── lib/                        # Source files for implementation
│   ├── Application.cpp         # Implements application logic
│   ├── Book.cpp                # Implements book class
│   ├── Database.cpp            # Implements database interactions
│   ├── Library.cpp             # Implements library management logic
│   ├── main.cpp                # Entry point of the application
│   ├── Menu.cpp                # Implements menu functionality
│   ├── Multithreading.cpp      # Implements multithreading support
│   ├── Networking.cpp          # Implements networking features
│   ├── Persistence.cpp         # Handles file I/O for persistence
│   ├── Transaction.cpp         # Implements transaction management
│   └── User.cpp                # Implements user class
└── tests/                      # Unit tests for each component
    ├── ApplicationTest.cpp     # Tests for the Application class
    ├── BookTest.cpp            # Tests for the Book class
    ├── DatabaseTest.cpp        # Tests for database operations
    ├── LibraryTest.cpp         # Tests for LibraryManager functionality
    ├── MenuTest.cpp            # Tests for Menu class
    ├── MultithreadingTest.cpp  # Tests for multithreading utilities
    ├── NetworkingTest.cpp      # Tests for networking functionalities
    ├── PersistenceTest.cpp     # Tests for data persistence
    ├── TransactionTest.cpp     # Tests for transaction handling
    └── UserTest.cpp            # Tests for the User class
```

## CMake Setup

This project uses CMake for the build process. The `make.sh` script simplifies the build process. To build the project, use one of the following commands:

1. **Build the project without tests**:
    ```bash
    ./make.sh
    ```

2. **Build the project and run tests**:
    ```bash
    ./make.sh -t
    ```

3. **Build and run a specific test**:
    ```bash
    ./make.sh -t TestName
    ```

Replace `TestName` with the actual name of the test you want to run.

**Note for Windows users**: If you are using Windows, you will need to manually run the build commands and tests using PowerShell or Command Prompt. By default, tests are built. To disable them, use the `-DBUILD_TESTS=OFF` option. Examples:

1. **Build the project without tests**:
    ```powershell
    cmake -DBUILD_TESTS=OFF . && cmake --build . --target all
    ```

2. **Build the project and run tests**:
    ```powershell
    cmake . && cmake --build . --target all && ctest
    ```

3. **Build and run a specific test**:
    ```powershell
    cmake . && cmake --build . --target TestName && .\TestName.exe
    ```

Replace `TestName` with the actual name of the test executable you want to run.

## Documentation

This project uses [Doxygen](https://www.doxygen.nl/) for generating documentation. If you have Doxygen installed, the LaTeX documentation will be generated in the directory `./docs/latex/`.

## Code of Conduct

We are committed to fostering a welcoming and inclusive environment for all contributors. Please review our [Code of Conduct](CODE_OF_CONDUCT.md) to understand the behaviours we expect in the community.

## License

This project is licensed under the terms of the [MIT License](LICENSE).
