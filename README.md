# Library Management System

[![Build Status](https://img.shields.io/badge/build-passing-brightgreen)](https://shields.io/)
[![License](https://img.shields.io/badge/license-MIT-blue)](LICENSE)

This project is submitted in partial fulfilment of the Degree of **Master of Science in Computer Science**.

## Table of Contents

- [Project Overview](#project-overview)
- [Features](#features)
- [Directory Structure](#directory-structure)
- [Installation Instructions](#installation-instructions)
- [Testing](#testing)
- [Usage](#usage)
- [Contributing](#contributing)
- [Documentation](#documentation)
- [Acknowledgements](#acknowledgements)
- [License](#license)

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
├── CMakeLists.txt          # CMake build configuration file
├── doc/                    # Documentation directory
│   └── Doxyfile            # Doxygen configuration file
├── include/                # Header files
│   ├── Application.hpp     # Application class header
│   ├── Book.hpp            # Book class header
│   ├── Database.hpp        # Database class header
│   ├── Library.hpp         # LibraryManager class header
│   ├── Menu.hpp            # Menu class header
│   ├── Multithreading.hpp  # Multithreading utilities header
│   ├── Networking.hpp      # Networking utilities header
│   ├── Persistence.hpp     # Persistence utilities header
│   ├── Transaction.hpp     # Transaction class header
│   └── User.hpp            # User class header
├── lib/                    # Source files
│   ├── Application.cpp     # Application class implementation
│   ├── Book.cpp            # Book class implementation
│   ├── Database.cpp        # Database class implementation
│   ├── Library.cpp         # LibraryManager class implementation
│   ├── main.cpp            # Main program entry point
│   ├── Menu.cpp            # Menu class implementation
│   ├── Multithreading.cpp  # Multithreading utilities implementation
│   ├── Networking.cpp      # Networking utilities implementation
│   ├── Persistence.cpp     # Persistence utilities implementation
│   ├── Transaction.cpp     # Transaction class implementation
│   └── User.cpp            # User class implementation
└── tests/                  # Test files
    ├── ApplicationTest.cpp # Application class tests
    ├── BookTest.cpp        # Book class tests
    ├── DatabaseTest.cpp    # Database class tests
    ├── LibraryTest.cpp     # LibraryManager class tests
    ├── MenuTest.cpp        # Menu class tests
    ├── MultithreadingTest.cpp # Multithreading utilities tests
    ├── NetworkingTest.cpp  # Networking utilities tests
    ├── PersistenceTest.cpp # Persistence utilities tests
    ├── TransactionTest.cpp # Transaction class tests
    └── UserTest.cpp        # User class tests
```

## Installation Instructions

To install and set up the project locally, follow these steps:

1. **Clone the repository**:
    ```bash
    git clone https://github.com/alfieatkinson/Library-Management-System.git
    cd Library-Management-System
    ```

2. **Run the build script**:
    ```bash
    ./make.sh
    ```

3. **Run the server**:
    ```bash
    ./build/LibraryManagementSystem
    ```

## Testing

This project uses CMake for the build and test process. The `make.sh` script simplifies the build process. To test the project, use one of the following commands:

1. **Build the project and run tests**:
    ```bash
    ./make.sh -t
    ```

2. **Build and run a specific test**:
    ```bash
    ./make.sh -t TestName
    ```

Replace `TestName` with the actual name of the test you want to run.

## Usage

To use the library management system, follow these steps:

1. **Connect a client to the server**:
    ```bash
    telnet localhost 8080
    ```
    **OR**
    ```bash
    nc localhost 8080
    ```

2. **Interact with the system**:
    - **Login**: Enter your username and password.
    - **Register**: Create a new user account.
    - **Borrow Book**: Borrow a book from the library.
    - **Return Book**: Return a borrowed book to the library.
    - **Search Items**: Search for books, users, or transactions.
    - **View My Profile**: View your user profile.
    - **Update My Profile**: Update your user profile.
    - **Add New Book**: Add a new book to the library (admin only).
    - **Add New User**: Add a new user to the system (admin only).

## Contributing

We welcome contributions to the Library Management System project. To contribute, follow these steps:

1. **Fork the repository** on GitHub.
2. **Clone your forked repository**:
    ```bash
    git clone https://github.com/your-username/Library-Management-System.git
    cd Library-Management-System
    ```
3. **Create a new branch** for your feature or bugfix:
    ```bash
    git checkout -b feature-or-bugfix-name
    ```
4. **Make your changes** and commit them with a descriptive message:
    ```bash
    git commit -am 'Add new feature or fix bug'
    ```
5. **Push your changes** to your forked repository:
    ```bash
    git push origin feature-or-bugfix-name
    ```
6. **Create a pull request** on the original repository.

Please review our [Code of Conduct](CODE_OF_CONDUCT.md) before contributing.

## Documentation

This project uses [Doxygen](https://www.doxygen.nl/) for generating documentation. If you have Doxygen installed, the LaTeX documentation will be generated in the directory `./docs/latex/`.

## Acknowledgements

We would like to thank the following contributors and third-party libraries/tools used in this project:

- [Doxygen](https://www.doxygen.nl/) for generating documentation.
- [Catch2](https://github.com/catchorg/Catch2) for unit testing framework.
- [Shields.io](https://shields.io/) for providing badges.

## License

This project is licensed under the terms of the [MIT License](LICENSE).