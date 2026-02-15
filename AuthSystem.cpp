#include "AuthSystem.h"
#include <sstream>
#include <algorithm>

AuthSystem::AuthSystem(const std::string& filename) : userFile(filename), currentUser("") {
    loadUsers();
}

void AuthSystem::loadUsers() {
    std::ifstream file(userFile);
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string username, hash;
        if (std::getline(ss, username, '|') && std::getline(ss, hash)) {
            users.push_back({username, hash});
        }
    }
    file.close();
}

void AuthSystem::saveUsers() {
    std::ofstream file(userFile);
    if (!file.is_open()) {
        std::cerr << "Error opening user file for writing!" << std::endl;
        return;
    }

    for (const auto& user : users) {
        file << user.username << "|" << user.passwordHash << "\n";
    }
    file.close();
}

std::string AuthSystem::hashPassword(const std::string& password) {
    // Using std::hash for a simple hash simulation
    // In a real application, use a proper cryptographic hash like SHA-256
    std::hash<std::string> hasher;
    size_t hash = hasher(password);
    return std::to_string(hash);
}

bool AuthSystem::registerUser(const std::string& username, const std::string& password) {
    // Check if user already exists
    for (const auto& user : users) {
        if (user.username == username) {
            return false; // User already exists
        }
    }

    User newUser;
    newUser.username = username;
    newUser.passwordHash = hashPassword(password);
    users.push_back(newUser);
    saveUsers();
    return true;
}

bool AuthSystem::login(const std::string& username, const std::string& password) {
    std::string inputHash = hashPassword(password);

    for (const auto& user : users) {
        if (user.username == username && user.passwordHash == inputHash) {
            currentUser = username;
            return true;
        }
    }
    return false;
}

void AuthSystem::logout() {
    currentUser = "";
}

bool AuthSystem::isLoggedIn() const {
    return !currentUser.empty();
}

std::string AuthSystem::getCurrentUser() const {
    return currentUser;
}
