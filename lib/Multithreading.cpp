#include "Multithreading.hpp"

// Constructor
ThreadManager::ThreadManager() : running(false) {}

// Destructor
ThreadManager::~ThreadManager() {
    stopBackgroundSave();
    joinClientThreads();
}