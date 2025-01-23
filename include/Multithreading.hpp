#ifndef MULTITHREADING_HPP
#define MULTITHREADING_HPP

#include <thread>
#include <atomic>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <chrono>
#include <vector>

class ThreadManager {
private:
    std::thread background_save_thread;
    std::vector<std::thread> client_threads;
    std::atomic<bool> running;
    std::mutex mtx;
    std::condition_variable cv;

    // Method to periodically save data in the background
    void periodicSaveTask(std::function<void()> saveFunction, std::chrono::minutes interval);

public:
    // Constructor
    ThreadManager();

    // Destructor
    ~ThreadManager();

    // Methods for background save
    void startBackgroundSave(std::function<void()> saveFunction, std::chrono::minutes interval);
    void stopBackgroundSave();

    // Methods for client threads
    void addClientThread(std::thread client_thread);
    void joinClientThreads();
};

#endif // MULTITHREADING_HPP