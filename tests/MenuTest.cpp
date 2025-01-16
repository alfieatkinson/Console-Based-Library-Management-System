#define CATCH_CONFIG_MAIN
#include <catch2/catch.hpp>
#include <sstream>
#include <iostream>

// Helper for redirecting console output
std::string captureConsoleOutput(std::function<void()> func) {
    std::ostringstream output;
    std::streambuf* oldCoutBuffer = std::cout.rdbuf(output.rdbuf());
    func();
    std::cout.rdbuf(oldCoutBuffer); // Reset cout
    return output.str();
}