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
├── CMakeLists.txt
├── doc/
│   └── Doxyfile
├── include/
│   ├── Application.hpp
│   ├── Book.hpp
│   ├── Database.hpp
│   ├── Library.hpp
│   ├── Menu.hpp
│   ├── Multithreading.hpp
│   ├── Networking.hpp
│   ├── Persistence.hpp
│   ├── Transaction.hpp
│   └── User.hpp
├── lib/
│   ├── Application.cpp
│   ├── Book.cpp
│   ├── Database.cpp
│   ├── Library.cpp
│   ├── main.cpp
│   ├── Menu.cpp
│   ├── Multithreading.cpp
│   ├── Networking.cpp
│   ├── Persistence.cpp
│   ├── Transaction.cpp
│   └── User.cpp
└── tests/
    ├── ApplicationTest.cpp
    ├── BookTest.cpp
    ├── DatabaseTest.cpp
    ├── LibraryTest.cpp
    ├── MenuTest.cpp
    ├── MultithreadingTest.cpp
    ├── NetworkingTest.cpp
    ├── PersistenceTest.cpp
    ├── TransactionTest.cpp
    └── UserTest.cpp
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

## Running the Server

To run the server, follow these steps:

1. **Build the project**:
    ```bash
    ./make.sh
    ```

2. **Run the server**:
    ```bash
    ./LibraryManagementSystem
    ```

The server will start and listen for client connections on port 8080.

## Connecting Clients

To connect a client to the server, use a TCP client tool such as `telnet` or `netcat`. For example, using `telnet`:

```bash
telnet localhost 8080
```

Or using `netcat`:

```bash
nc localhost 8080
```

Once connected, you can interact with the library management system through the client.

## Documentation

This project uses [Doxygen](https://www.doxygen.nl/) for generating documentation. If you have Doxygen installed, the LaTeX documentation will be generated in the directory `./docs/latex/`.

## Code of Conduct

We are committed to fostering a welcoming and inclusive environment for all contributors. Please review our [Code of Conduct](CODE_OF_CONDUCT.md) to understand the behaviours we expect in the community.

## License

This project is licensed under the terms of the [MIT License](LICENSE).
