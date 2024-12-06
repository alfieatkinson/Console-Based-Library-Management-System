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
├── CMakeLists.txt              # Build configuration
├── doc/                        # Documentation (e.g., Doxygen config)
│   └── Doxyfile
├── include/                    # Header files
│   ├── Application.hpp         # Application logic (managing flow)
│   ├── Book.hpp                # Book model (attributes & methods)
│   ├── Database.hpp            # Database interaction (loading/saving data)
│   ├── Library.hpp             # Manages library operations (book/user)
│   ├── Multithreading.hpp      # Multithreading utilities
│   ├── Networking.hpp          # Network-related functions
│   ├── Persistence.hpp         # Handles saving/loading data to/from files
│   ├── Transaction.hpp         # Transaction (borrow/return books)
│   └── User.hpp                # User model (attributes & methods)
├── lib/                        # Source files
│   ├── Application.cpp         # Application logic (start, login/logout, menu)
│   ├── Book.cpp                # Book class implementation
│   ├── Database.cpp            # Database class implementation (load/save data)
│   ├── Library.cpp             # LibraryManager logic (book/user management)
│   ├── main.cpp                # Main entry point of the application
│   ├── Multithreading.cpp      # Multithreading class implementation
│   ├── Networking.cpp          # Networking-related functions
│   ├── Persistence.cpp         # File I/O operations for persistence
│   ├── Transaction.cpp         # Transaction handling (borrow/return)
│   └── User.cpp                # User class implementation
└── tests/                      # Unit tests
    ├── ApplicationTest.cpp     # Tests for the Application class
    ├── BookTest.cpp            # Tests for the Book class
    ├── DatabaseTest.cpp        # Tests for Database interactions
    ├── LibraryTest.cpp         # Tests for LibraryManager logic
    ├── MultithreadingTest.cpp  # Tests for Multithreading utilities
    ├── NetworkingTest.cpp      # Tests for Networking functionality
    ├── PersistenceTest.cpp     # Tests for Persistence operations
    ├── TransactionTest.cpp     # Tests for Transaction handling
    └── UserTest.cpp            # Tests for User class
```

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
```

## Code of Conduct

We are committed to fostering a welcoming and inclusive environment for all contributors. Please review our [Code of Conduct](CODE_OF_CONDUCT.md) to understand the behaviours we expect in the community.

## License

This project is licensed under the terms of the [MIT License](LICENSE).
