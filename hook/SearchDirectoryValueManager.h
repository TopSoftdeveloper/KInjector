#pragma once
#include <string>
#include <mutex>

class SearchDirectoryValueManager {
private:
    std::wstring searchDirectory;
    std::mutex directoryMutex;

    SearchDirectoryValueManager() = default;

public:
    static SearchDirectoryValueManager& GetInstance() {
        static SearchDirectoryValueManager instance;
        return instance;
    }

    void SetDirectory(const std::wstring& newValue) {
        std::lock_guard<std::mutex> lock(directoryMutex);
        searchDirectory = newValue;
    }

    std::wstring GetDirectory() {
        std::lock_guard<std::mutex> lock(directoryMutex);
        return searchDirectory;
    }

    // Delete copy constructor and assignment operator
    SearchDirectoryValueManager(const SearchDirectoryValueManager&) = delete;
    SearchDirectoryValueManager& operator=(const SearchDirectoryValueManager&) = delete;
};


