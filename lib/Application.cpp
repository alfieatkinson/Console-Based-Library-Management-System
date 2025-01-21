#include <iostream>
#include <sstream>
#include <limits>
#include "Application.hpp"

// Constructor
Application::Application() {
    initialiseMenus();
}

// Destructor
Application::~Application() {}

// Initialisation methods
void Application::initialiseMenus() {
    menu_stack.push(makeLoginMenu());
}

void Application::promptAdminPassword() {
    std::string input = promptInput("Enter the admin password: ");
    if (library.authenticateAdmin(input)) {
        is_admin = true;
        menu_stack.push(makeMainMenu());
    }
    else {
        std::cout << "Incorrect password. Please try again." << std::endl;
    }
}

// Methods for making item summaries
std::string Application::makeBookSummary(const std::shared_ptr<Book>& book) {
    std::ostringstream oss;
    oss << book->getAuthor() << ": "
        << book->getTitle() << " ["
        << book->getISBN() << ":"
        << book->getID() << "]";
    return oss.str();
}

std::string Application::makeUserSummary(const std::shared_ptr<User>& user) {
    std::ostringstream oss;
    oss << user->getForename() << " "
        << user->getSurname() << " ("
        << user->getUsername() << ")";
    return oss.str();
}

std::string Application::makeTransactionSummary(const std::shared_ptr<Transaction>& transaction) {
    std::ostringstream oss;
    oss << transaction->getDatetime() << "    - "
        << transaction->getBook()->getAuthor() << ": "
        << transaction->getBook()->getTitle() << " "
        << transaction->getType() << "ed by "
        << transaction->getUser()->getForename() << " "
        << transaction->getUser()->getSurname() << " ("
        << transaction->getUser()->getUsername() << ")";
    return oss.str();
}

// Microservices for menu actions
std::string Application::promptInput(const std::string& prompt) {
    std::string input;
    std::cout << prompt;
    std::cin >> input;
    return input;
}

bool Application::areYouSurePrompt(bool undoable) {
    std::string extra = undoable ? " This action cannot be undone." : "";
    std::string input = promptInput("Are you sure?" + extra + " [y/N] ");
    return input == "y" || input == "Y";
}

void Application::dummyPrompt() {
    std::cout << "Press Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n'); // Clear any lingering input
    std::cin.get(); // Wait for Enter
}