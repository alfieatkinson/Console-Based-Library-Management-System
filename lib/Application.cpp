#include <iostream>
#include <sstream>
#include <limits>
#include <sys/socket.h>
#include <unistd.h>

#include "Application.hpp"

// Constructor
Application::Application(int client_socket) : client_socket(client_socket) {
    initialiseMenus();
}

// Destructor
Application::~Application() {
    close(client_socket);
}

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
        sendData("Incorrect password. Please try again.\n");
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
    sendData(prompt);
    return receiveData();
}

bool Application::areYouSurePrompt(bool undoable) {
    std::string extra = undoable ? " This action cannot be undone." : "";
    std::string input = promptInput("Are you sure?" + extra + " [y/N] ");
    return input == "y" || input == "Y";
}

void Application::dummyPrompt() {
    sendData("Press Enter to continue...");
    receiveData();
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
            sendData("Invalid input. Please enter an integer.\n");
            dummyPrompt();
        }
    }, true);
    menu->addOption("Search Transactions by User", [this]() { // Admin-only option
        try {
            int query = std::stoi(promptInput("Enter user ID: "));
            std::vector<std::shared_ptr<Transaction>> results = library.queryTransactionsByUserID(query);
            menu_stack.push(makeTransactionsMenu(results));
        } catch (const std::invalid_argument&) {
            sendData("Invalid input. Please enter an integer.\n");
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
            sendData(e.what());
            sendData("\n");
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
            sendData("Passwords do not match. Please try again.\n");
            dummyPrompt();
        }
    });
    menu->addOption("Update Email", [this, user]() {
        try {
            library.updateUser(user->getID(), "email", promptInput("Enter the new email: "));
        } catch (const std::invalid_argument& e) {
            sendData(e.what());
            sendData("\n");
            dummyPrompt();
        }
    });
    menu->addOption("Update Phone Number", [this, user]() {
        try {
            library.updateUser(user->getID(), "phone", promptInput("Enter the new phone number: "));
        } catch (const std::invalid_argument& e) {
            sendData(e.what());
            sendData("\n");
            dummyPrompt();
        }
    });
    menu->addOption("[BACK]", [this]() {
        menu_stack.pop(); // Go back to the previous menu
    });
    return menu;
}

std::shared_ptr<Menu> Application::makeUpdateTransactionMenu(std::shared_ptr<Transaction> transaction) {
    auto menu = std::make_shared<Menu>("Update Transaction Menu");
    menu->addOption("Update Status", [this, transaction]() {
        library.updateTransaction(transaction->getID(), "status", promptInput("Enter the new status: "));
    });
    menu->addOption("[BACK]", [this]() {
        menu_stack.pop(); // Go back to the previous menu
    });
    return menu;
}

// Methods for user authentication
void Application::login() {
    std::string username = promptInput("Enter your username: ");
    std::string password = promptInput("Enter your password: ");
    try {
        current_user = library.authenticateUser(username, password);
        is_admin = current_user->getUsername() == "admin";
        menu_stack.push(makeMainMenu());
    } catch (const std::invalid_argument& e) {
        sendData(e.what());
        sendData("\n");
        dummyPrompt();
    }
}

void Application::logout() {
    current_user = nullptr;
    is_admin = false;
    menu_stack = std::stack<std::shared_ptr<Menu>>();
    menu_stack.push(makeLoginMenu());
}

// Book management methods
void Application::createBook() {
    std::vector<std::string> book_info;
    book_info.push_back(promptInput("Enter the book title: "));
    book_info.push_back(promptInput("Enter the book author: "));
    book_info.push_back(promptInput("Enter the book ISBN: "));
    book_info.push_back(promptInput("Enter the year published: "));
    try {
        library.createBook(book_info);
    } catch (const std::invalid_argument& e) {
        sendData(e.what());
        sendData("\n");
        dummyPrompt();
    }
}

void Application::deleteBook(std::shared_ptr<Book> book) {
    if (areYouSurePrompt()) {
        library.deleteBook(book->getID());
    }
}

// User management methods
void Application::createUser() {
    std::vector<std::string> user_info;
    user_info.push_back(promptInput("Enter the username: "));
    user_info.push_back(promptInput("Enter the forename: "));
    user_info.push_back(promptInput("Enter the surname: "));
    user_info.push_back(promptInput("Enter the email: "));
    user_info.push_back(promptInput("Enter the phone number: "));
    while(true) {
        std::string password = promptInput("Enter the password: ");
        std::string confirm_password = promptInput("Confirm the password: ");
        if (password == confirm_password) {
            user_info.push_back(password);
            break;
        } else {
            sendData("Passwords do not match. Please try again.\n");
        }
    }
    try {
        library.createUser(user_info);
    } catch (const std::invalid_argument& e) {
        sendData(e.what());
        sendData("\n");
        dummyPrompt();
    }
}

void Application::deleteUser(std::shared_ptr<User> user) {
    if (areYouSurePrompt()) {
        library.deleteUser(user->getID());
    }
}

// Transaction management methods
void Application::deleteTransaction(std::shared_ptr<Transaction> transaction) {
    if (areYouSurePrompt()) {
        library.deleteTransaction(transaction->getID());
    }
}

// Borrowing and returning books
void Application::borrowBook(std::shared_ptr<Book> book) {
    if (book == nullptr) {
        int book_id;
        try {
            book_id = std::stoi(promptInput("Enter the book ID: "));
            book = library.readBook(book_id);
        } catch (const std::invalid_argument& e) {
            sendData(e.what());
            sendData("\n");
            dummyPrompt();
            return;
        }
    }
    if (is_admin) {
        int user_id;
        try {
            user_id = std::stoi(promptInput("Enter the user ID: "));
            library.borrowBook(book->getID(), user_id);
            sendData("Book borrowed successfully.\n");
            dummyPrompt();
            return;
        } catch (const std::invalid_argument& e) {
            sendData(e.what());
            sendData("\n");
            dummyPrompt();
            return;
        }
    }
    library.borrowBook(book->getID(), current_user->getID());
    sendData("Book borrowed successfully.\n");
    dummyPrompt();
}

void Application::returnBook(std::shared_ptr<Book> book) {
    std::shared_ptr<User> user = current_user;
    if (is_admin) {
        int user_id;
        try {
            user_id = std::stoi(promptInput("Enter the user ID: "));
            user = library.readUser(user_id);
        } catch (const std::invalid_argument& e) {
            sendData(e.what());
            sendData("\n");
            dummyPrompt();
            return;
        }
    }
    if (book == nullptr) {
        int book_id;
        try {
            sendData(getUserBooks(user) + "\n");
            book_id = std::stoi(promptInput("Enter the book ID: "));
            book = library.readBook(book_id);
        } catch (const std::invalid_argument& e) {
            sendData(e.what());
            sendData("\n");
            dummyPrompt();
            return;
        }
    }
    if (user->getBorrowedBooks().empty()) {
        sendData("User has no borrowed books.\n");
        dummyPrompt();
        return;
    }
    library.returnBook(book->getID(), user->getID());
    sendData("Book returned successfully.\n");
    dummyPrompt();
}

// Methods for displaying information
std::string Application::getUserBooks(std::shared_ptr<User> user) {
    std::ostringstream oss;
    oss << "Borrowed Books:\n";
    auto books = user->getBorrowedBooks();
    for (const auto& book : books) {
        oss << " - " << makeBookSummary(book) << "\n";
    }
    return oss.str();
}

void Application::showBookInfo(std::shared_ptr<Book> book) {
    clearConsole();
    sendData(book->getInfo() + "\n");
    dummyPrompt();
}

void Application::showUserInfo(std::shared_ptr<User> user) {
    clearConsole();
    sendData(user->getInfo() + "\n");
    sendData(getUserBooks(user) + "\n");
    dummyPrompt();
}

void Application::showTransactionInfo(std::shared_ptr<Transaction> transaction) {
    clearConsole();
    sendData(transaction->getInfo() + "\n");
    dummyPrompt();
}

// Methods for sending and receiving data through the socket
std::string Application::receiveData() {
    char buffer[1024] = {0};
    int valread = read(client_socket, buffer, 1024);
    return std::string(buffer, valread);
}

// Method to clear the console
void Application::clearConsole() {
    sendData("\n\n\n");
    sendData("\033[2J\033[1;1H");
    sendData("\n");
}

void Application::run() {
    while (!menu_stack.empty()) {
        auto current_menu = menu_stack.top();
        clearConsole();
        current_menu->display(is_admin);
    }
}