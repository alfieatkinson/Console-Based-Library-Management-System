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

std::shared_ptr<Menu> Application::makeSearchMenu() {
    auto menu = std::make_shared<Menu>("Search Menu");

    menu->addOption("Search Books", [this]() {
        std::string query = promptInput("Enter search term: ");
        std::vector<std::shared_ptr<Book>> results = library.queryBooks(query);
        menu_stack.push(makeBooksMenu(results));
    });
    menu->addOption("Search Users", [this]() { // Admin-only option
        std::string query = promptInput("Enter search term: ");
        std::vector<std::shared_ptr<User>> results = library.queryUsers(query);
        menu_stack.push(makeUsersMenu(results));
    }, true);
    menu->addOption("Search Transactions by Book", [this]() { // Admin-only option
        try {
            int query = std::stoi(promptInput("Enter book ID: "));
            std::vector<std::shared_ptr<Transaction>> results = library.queryTransactionsByBookID(query);
            menu_stack.push(makeTransactionsMenu(results));
        } catch (const std::invalid_argument&) {
            std::cout << "Invalid input. Please enter an integer." << std::endl;
            dummyPrompt();
        }
    }, true);
    menu->addOption("Search Transactions by User", [this]() { // Admin-only option
        try {
            int query = std::stoi(promptInput("Enter user ID: "));
            std::vector<std::shared_ptr<Transaction>> results = library.queryTransactionsByUserID(query);
            menu_stack.push(makeTransactionsMenu(results));
        } catch (const std::invalid_argument&) {
            std::cout << "Invalid input. Please enter an integer." << std::endl;
            dummyPrompt();
        }
    }, true);
    menu->addOption("[BACK]", [this]() {
        menu_stack.pop(); // Go back to the previous menu
    });
    return menu;
}

std::shared_ptr<Menu> Application::makeBooksMenu(std::vector<std::shared_ptr<Book>> books) {
    auto menu = std::make_shared<Menu>("Books", true);
    for (const auto& book : books) {
        menu->addOption(makeBookSummary(book), [this, book]() {
            menu_stack.push(makeBookMenu(book));
        });
    }
    menu->addOption("[BACK]", [this]() {
        menu_stack.pop(); // Go back to the previous menu
    });
    return menu;
}

std::shared_ptr<Menu> Application::makeUsersMenu(std::vector<std::shared_ptr<User>> users) {
    auto menu = std::make_shared<Menu>("Users", true);
    for (const auto& user : users) {
        menu->addOption(makeUserSummary(user), [this, user]() {
            menu_stack.push(makeUserMenu(user));
        });
    }
    menu->addOption("[BACK]", [this]() {
        menu_stack.pop(); // Go back to the previous menu
    });
    return menu;
}

std::shared_ptr<Menu> Application::makeTransactionsMenu(std::vector<std::shared_ptr<Transaction>> transactions) {
    auto menu = std::make_shared<Menu>("Transactions", true);
    for (const auto& transaction : transactions) {
        menu->addOption(makeTransactionSummary(transaction), [this, transaction]() {
            menu_stack.push(makeTransactionMenu(transaction));
        });
    }
    menu->addOption("[BACK]", [this]() {
        menu_stack.pop(); // Go back to the previous menu
    });
    return menu;
}

std::shared_ptr<Menu> Application::makeBookMenu(std::shared_ptr<Book> book) {
    auto menu = std::make_shared<Menu>(makeBookSummary(book));

    menu->addOption("View Book Info", [this, book]() {
        showBookInfo(book);
    });
    menu->addOption("Borrow Book", [this, book]() {
        borrowBook(book);
    });
    menu->addOption("Return Book", [this, book]() {
        returnBook(book);
    });
    menu->addOption("Update Book Info", [this, book]() { // Admin-only option
        menu_stack.push(makeUpdateBookMenu(book));
    }, true);
    menu->addOption("Delete Book", [this, book]() { // Admin-only option
        deleteBook(book);
    }, true);
    menu->addOption("[BACK]", [this]() {
        menu_stack.pop(); // Go back to the previous menu
    });
    return menu;
}

std::shared_ptr<Menu> Application::makeUserMenu(std::shared_ptr<User> user) {
    auto menu = std::make_shared<Menu>(makeUserSummary(user));

    menu->addOption("View User Info", [this, user]() {
        showUserInfo(user);
    });
    menu->addOption("View Borrowed Books", [this, user]() {
        menu_stack.push(makeBooksMenu(user->getBorrowedBooks()));
    });
    menu->addOption("View User Transactions", [this, user]() {
        auto transactions = library.queryTransactionsByUserID(user->getID());
        menu_stack.push(makeTransactionsMenu(transactions));
    });
    menu->addOption("Update User Info", [this, user]() { // Admin-only option
        menu_stack.push(makeUpdateUserMenu(user));
    }, true);
    menu->addOption("Delete User", [this, user]() { // Admin-only option
        deleteUser(user);
    }, true);
    menu->addOption("[BACK]", [this]() {
        menu_stack.pop(); // Go back to the previous menu
    });
    return menu;
}

std::shared_ptr<Menu> Application::makeTransactionMenu(std::shared_ptr<Transaction> transaction) {
    auto menu = std::make_shared<Menu>(makeTransactionSummary(transaction));

    menu->addOption("View Transaction Info", [this, transaction]() {
        showTransactionInfo(transaction);
    });
    menu->addOption("Update Transaction Info", [this, transaction]() { // Admin-only option
        menu_stack.push(makeUpdateTransactionMenu(transaction));
    }, true);
    menu->addOption("Delete Transaction", [this, transaction]() { // Admin-only option
        deleteTransaction(transaction);
    }, true);
    menu->addOption("[BACK]", [this]() {
        menu_stack.pop(); // Go back to the previous menu
    });
    return menu;
}

std::shared_ptr<Menu> Application::makeUpdateBookMenu(std::shared_ptr<Book> book) {
    auto menu = std::make_shared<Menu>("Update Book Menu");

    menu->addOption("Update Title", [this, book]() { // Admin-only option
        library.updateBook(book->getID(), "title", promptInput("Enter the new title: "));
    }, true);
    menu->addOption("Update Author", [this, book]() { // Admin-only option
        library.updateBook(book->getID(), "author", promptInput("Enter the new author: "));
    }, true);
    menu->addOption("Update ISBN", [this, book]() { // Admin-only option
        try {
            library.updateBook(book->getID(), "isbn", promptInput("Enter the new ISBN: "));
        } catch (const std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
            dummyPrompt();
        }
    }, true);
    menu->addOption("Update Year Published", [this, book]() { // Admin-only option
        library.updateBook(book->getID(), "year_published", promptInput("Enter the new year published: "));
    }, true);
    menu->addOption("[BACK]", [this]() {
        menu_stack.pop(); // Go back to the previous menu
    });
    return menu;
}

std::shared_ptr<Menu> Application::makeUpdateUserMenu(std::shared_ptr<User> user) {
    auto menu = std::make_shared<Menu>("Update User Menu");

    menu->addOption("Update Forename", [this, user]() {
        library.updateUser(user->getID(), "forename", promptInput("Enter the new forename: "));
    });
    menu->addOption("Update Surname", [this, user]() {
        library.updateUser(user->getID(), "surname", promptInput("Enter the new surname: "));
    });
    menu->addOption("Update Username", [this, user]() {
        library.updateUser(user->getID(), "username", promptInput("Enter the new username: "));
    });
    menu->addOption("Update Password", [this, user]() {
        std::string new_password = promptInput("Enter the new password: ");
        std::string confirm_password = promptInput("Confirm the new password: ");

        if (new_password == confirm_password) {
            library.updateUser(user->getID(), "password", new_password);
        } else {
            std::cout << "Passwords do not match. Please try again." << std::endl;
            dummyPrompt();
        }
    });
    menu->addOption("Update Email", [this, user]() {
        try {
            library.updateUser(user->getID(), "email", promptInput("Enter the new email: "));
        } catch (const std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
            dummyPrompt();
        }
    });
    menu->addOption("Update Phone Number", [this, user]() {
        try {
            library.updateUser(user->getID(), "phone", promptInput("Enter the new phone number: "));
        } catch (const std::invalid_argument& e) {
            std::cout << e.what() << std::endl;
            dummyPrompt();
        }
    });
    menu->addOption("[BACK]", [this]() {
        menu_stack.pop(); // Go back to the previous menu
    });
    return menu;
}