#ifndef TRANSACTION_H
#define TRANSACTION_H

#include <string>
#include <iostream>
#include <ctime>
#include "Utils.h"

enum class TransactionType {
    INCOME,
    EXPENSE
};

class Transaction {
private:
    int id;
    std::string date;
    std::string category;
    double amount;
    TransactionType type;
    std::string note;

public:
    // Constructors
    Transaction();
    Transaction(int id, double amount, std::string category, TransactionType type, std::string note = "", std::string date = Utils::getCurrentDate());

    // Getters
    int getId() const;
    std::string getDate() const;
    std::string getCategory() const;
    double getAmount() const;
    TransactionType getType() const;
    std::string getNote() const;

    // Setters
    void setDate(const std::string& date);
    void setCategory(const std::string& category);
    void setAmount(double amount);
    void setType(TransactionType type);
    void setNote(const std::string& note);

    // Serialization
    std::string toString() const; // For file storage
    static Transaction fromString(const std::string& line); // Deserialization

    // Display
    void display() const;
};

#endif // TRANSACTION_H
