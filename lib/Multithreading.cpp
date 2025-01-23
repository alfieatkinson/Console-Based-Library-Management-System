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