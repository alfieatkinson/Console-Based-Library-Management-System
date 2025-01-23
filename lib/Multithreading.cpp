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