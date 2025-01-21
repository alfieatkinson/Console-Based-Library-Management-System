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

// Methods for making menus
std::shared_ptr<Menu> Application::makeLoginMenu() {
    auto menu = std::make_shared<Menu>("Login Menu");

    menu->addOption("Login", [this]() {
        login();
    });
    menu->addOption("Register", [this]() {
        createUser();
    });
    menu->addOption("Login as Admin", [this]() {
        promptAdminPassword();
    });
    menu->addOption("[EXIT]", [this]() {
        menu_stack = std::stack<std::shared_ptr<Menu>>(); // Clear the menu stack
    });
    return menu;
}

std::shared_ptr<Menu> Application::makeMainMenu() {
    auto menu = std::make_shared<Menu>("Main Menu");

    menu->addOption("Borrow Book", [this]() {
        borrowBook();
    });
    menu->addOption("Return Book", [this]() {
        returnBook();
    });
    menu->addOption("Search Items", [this]() {
        menu_stack.push(makeSearchMenu());
    });
    menu->addOption("View My Profile", [this]() {
        if (is_admin) {
            std::cout << "You are logged in as an admin." << std::endl;
            dummyPrompt();
        }
        showUserInfo(current_user);
    });
    menu->addOption("Update My Profile", [this]() {
        menu_stack.push(makeUpdateUserMenu(current_user));
    });
    menu->addOption("Add New Book", [this]() { // Admin-only option
        createBook();
    }, true);
    menu->addOption("Add New User", [this]() { // Admin-only option
        createUser();
    }, true);
    menu->addOption("[LOGOUT]", [this]() {
        logout();
    });
    return menu;
}