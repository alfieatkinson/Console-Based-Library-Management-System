#ifndef APPLICATION_HPP
#define APPLICATION_HPP

#include <stack>
#include <memory>
#include <string>
#include "Menu.hpp"
#include "Library.hpp"
#include "User.hpp"
#include "Book.hpp"
#include "Transaction.hpp"

class Application {
private:
    // Stack to manage the menu navigation
    std::stack<std::shared_ptr<Menu>> menu_stack;

    // Current user and admin status
    std::shared_ptr<User> current_user;
    bool is_admin = false;

    // The library manager
    std::shared_ptr<LibraryManager> library_manager;

    // Socket file descriptor for client communication
    int client_socket;

    // Initialisation methods
    void initialiseMenus();
    void promptAdminPassword();

    // Methods for making item summaries
    std::string makeBookSummary(const std::shared_ptr<Book>& book);
    std::string makeUserSummary(const std::shared_ptr<User>& user);
    std::string makeTransactionSummary(const std::shared_ptr<Transaction>& transaction);

    // Microservices for menu actions
    std::string promptInput(const std::string& prompt);
    bool areYouSurePrompt(bool undoable = true);
    void dummyPrompt();

    // Methods for making menus
    std::shared_ptr<Menu> makeLoginMenu();
    std::shared_ptr<Menu> makeMainMenu();
    std::shared_ptr<Menu> makeSearchMenu();

    std::shared_ptr<Menu> makeBooksMenu(std::vector<std::shared_ptr<Book>> books);
    std::shared_ptr<Menu> makeUsersMenu(std::vector<std::shared_ptr<User>> users);
    std::shared_ptr<Menu> makeTransactionsMenu(std::vector<std::shared_ptr<Transaction>> transactions);

    std::shared_ptr<Menu> makeBookMenu(std::shared_ptr<Book> book);
    std::shared_ptr<Menu> makeUserMenu(std::shared_ptr<User> user);
    std::shared_ptr<Menu> makeTransactionMenu(std::shared_ptr<Transaction> transaction);

    std::shared_ptr<Menu> makeUpdateBookMenu(std::shared_ptr<Book> book);
    std::shared_ptr<Menu> makeUpdateUserMenu(std::shared_ptr<User> user);
    std::shared_ptr<Menu> makeUpdateTransactionMenu(std::shared_ptr<Transaction> transaction);

    // Methods for user authentication
    void login();
    void logout();

    // Book management methods
    void createBook();
    void deleteBook(std::shared_ptr<Book> book);

    // User management methods
    void createUser();
    void deleteUser(std::shared_ptr<User> user);

    // Transaction management methods
    void deleteTransaction(std::shared_ptr<Transaction> transaction);

    // Borrowing and returning books
    void borrowBook(std::shared_ptr<Book> book = nullptr);
    void returnBook(std::shared_ptr<Book> book = nullptr);

    // Methods for displaying information
    std::string getUserBooks(std::shared_ptr<User> user);
    void showBookInfo(std::shared_ptr<Book> book);
    void showUserInfo(std::shared_ptr<User> user);
    void showTransactionInfo(std::shared_ptr<Transaction> transaction);

    // Methods for sending and receiving data through the socket
    std::string receiveData();
    void sendData(const std::string& data);

public:
    // Constructor
    Application(int client_socket, std::shared_ptr<LibraryManager> library_manager);

    // Destructor
    ~Application();

    // Method to clear the console
    void clearConsole();

    // Method to run the application
    void run();
};

#endif // APPLICATION_HPP
