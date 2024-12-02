cmake_minimum_required(VERSION 3.10)

# Set the project name and version
project(LibraryManagementSystem VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Add include directories
include_directories(include)

# Add the library sources
add_library(libLibraryManagementSystem
    lib/Library.cpp
    lib/User.cpp
    lib/Book.cpp
    lib/Database.cpp
    lib/Application.cpp
    lib/Networking.cpp
    lib/Multithreading.cpp
    lib/Persistence.cpp
    lib/Transaction.cpp
)

# Add the executable for the main program
add_executable(LibraryManagementSystem src/main.cpp)

# Link the library to the executable
target_link_libraries(LibraryManagementSystem libLibraryManagementSystem)

# Find Catch2
find_package(Catch2 REQUIRED)

# Add test executables and link them to Catch2 and the main library
file(GLOB TEST_SOURCES tests/*.cpp)
foreach(test_source ${TEST_SOURCES})
    get_filename_component(test_name ${test_source} NAME_WE)
    add_executable(${test_name} ${test_source})
    target_link_libraries(${test_name} libLibraryManagementSystem Catch2::Catch2)
endforeach()

# Add custom targets for documentation generation
find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(DOXYGEN_IN ${CMAKE_CURRENT_SOURCE_DIR}/doc/Doxyfile)
    set(DOXYGEN_OUT ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile)

    add_custom_command(
        OUTPUT ${DOXYGEN_OUT}
        COMMAND ${CMAKE_COMMAND} -E copy ${DOXYGEN_IN} ${DOXYGEN_OUT}
        COMMENT "Copying Doxyfile to build directory"
    )

    add_custom_target(doc ALL
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
    )
endif (DOXYGEN_FOUND)