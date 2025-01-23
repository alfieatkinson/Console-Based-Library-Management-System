cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(LibraryManagementSystem VERSION 0.1)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directories
include_directories(include)

# Add the library sources
add_library(libLibraryManagementSystem
    lib/Application.cpp
    lib/Book.cpp
    lib/Database.cpp
    lib/Library.cpp
    lib/Menu.cpp
    lib/Multithreading.cpp
    lib/Networking.cpp
    lib/Persistence.cpp
    lib/Transaction.cpp
    lib/User.cpp
    lib/Utils.cpp
)

# Add the executable for the main program
add_executable(LibraryManagementSystem lib/main.cpp)

# Link the library to the executable
target_link_libraries(LibraryManagementSystem libLibraryManagementSystem)

# Option to enable/disable tests
option(BUILD_TESTS "Build tests" ON)

if (BUILD_TESTS)
    # Find Catch2
    find_package(Catch2 REQUIRED)

    # Enable testing
    enable_testing()

    # Add test executables and register them with `add_test()`
    file(GLOB TEST_SOURCES tests/*.cpp)
    foreach(test_source ${TEST_SOURCES})
        get_filename_component(test_name ${test_source} NAME_WE)
        message(STATUS "Adding test: ${test_name} from ${test_source}")
        add_executable(${test_name} ${test_source})
        target_link_libraries(${test_name} libLibraryManagementSystem Catch2::Catch2)
        add_test(NAME ${test_name} COMMAND ${test_name} --reporter console)
    endforeach()
endif()

# Option to build documentation
option(BUILD_DOC "Build documentation" ON)

# Add custom targets for documentation generation
find_package(Doxygen)
if (DOXYGEN_FOUND AND BUILD_DOC)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    # Request to configure the file
    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    message("Doxygen build started")

    add_custom_target(doc ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
else()
    message("Doxygen needs to be installed to generate the documentation")
endif()