#ifndef AUTHSYSTEM_H
#define AUTHSYSTEM_H

#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <functional> // For std::hash
#include "Utils.h"

struct User {
    std::string username;
    std::string passwordHash;
};

class AuthSystem {
private:
    std::string userFile;
    std::vector<User> users;
    std::string currentUser;

    void loadUsers();
    void saveUsers();
    std::string hashPassword(const std::string& password);

public:
    AuthSystem(const std::string& filename = "users.txt");
    
    bool registerUser(const std::string& username, const std::string& password);
    bool login(const std::string& username, const std::string& password);
    void logout();
    bool isLoggedIn() const;
    std::string getCurrentUser() const;
};

#endif // AUTHSYSTEM_H
