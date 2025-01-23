#include "Multithreading.hpp"

// Constructor
ThreadManager::ThreadManager() : running(false) {}

// Destructor
ThreadManager::~ThreadManager() {
    stopBackgroundSave();
    joinClientThreads();
}

// Method to periodically save data in the background
void ThreadManager::periodicSaveTask(std::function<void()> saveFunction, std::chrono::minutes interval) {
    while (running) {
        std::this_thread::sleep_for(interval);
        if (running) {
            saveFunction();
        }
    }
}

// Methods for background save
void ThreadManager::startBackgroundSave(std::function<void()> saveFunction, std::chrono::minutes interval) {
    if (running) {
        return;
    }
    running = true;
    background_save_thread = std::thread(&ThreadManager::periodicSaveTask, this, saveFunction, interval);
}

void ThreadManager::stopBackgroundSave() {
    if (!running) {
        return;
    }
    running = false;
    if (background_save_thread.joinable()) {
        background_save_thread.join();
    }
}

// Methods for client threads
void ThreadManager::addClientThread(std::thread client_thread) {
    std::lock_guard<std::mutex> lock(mtx);
    client_threads.push_back(std::move(client_thread));
}

void ThreadManager::joinClientThreads() {
    std::lock_guard<std::mutex> lock(mtx);
    for (auto& thread : client_threads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    client_threads.clear();
}