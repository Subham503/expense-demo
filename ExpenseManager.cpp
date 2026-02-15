#include "ExpenseManager.h"
#include <fstream>
#include <iostream>
#include <algorithm>

ExpenseManager::ExpenseManager() : currentUser("") {}

std::string ExpenseManager::getFilename() const {
    if (currentUser.empty()) return "transactions.txt";
    return currentUser + "_transactions.txt";
}

void ExpenseManager::loadTransactions(const std::string& username) {
    currentUser = username;
    transactions.clear();
    while (!undoStack.empty()) undoStack.pop();

    std::ifstream file(getFilename());
    if (!file.is_open()) return;

    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            transactions.push_back(Transaction::fromString(line));
        }
    }
    file.close();
}

void ExpenseManager::saveTransactions() {
    if (currentUser.empty()) return;

    std::ofstream file(getFilename());
    for (const auto& t : transactions) {
        file << t.toString() << "\n";
    }
    file.close();
}

void ExpenseManager::saveStateForUndo() {
    undoStack.push(transactions);
    // Limit stack size if necessary, e.g., to 10
    if (undoStack.size() > 10) {
        std::vector<Transaction> bottom = undoStack.top(); // This is wrong logic for removing bottom, but std::stack doesn't support it easily.
        // For simplicity, we just keep pushing. In a real app we'd use deque.
    }
}

void ExpenseManager::addTransaction(const Transaction& t) {
    saveStateForUndo();
    transactions.push_back(t);
    saveTransactions();
}

void ExpenseManager::deleteTransaction(int id) {
    saveStateForUndo();
    auto it = std::remove_if(transactions.begin(), transactions.end(), 
                             [id](const Transaction& t) { return t.getId() == id; });
    
    if (it != transactions.end()) {
        transactions.erase(it, transactions.end());
        saveTransactions();
        std::cout << "Transaction deleted.\n";
    } else {
        std::cout << "Transaction ID not found.\n";
        // Pop the undo state since nothing changed? 
        // Actually simpler to just leave it, or pop it.
        undoStack.pop();
    }
}

void ExpenseManager::editTransaction(int id, const Transaction& updatedT) {
    saveStateForUndo();
    bool found = false;
    for (auto& t : transactions) {
        if (t.getId() == id) {
            t = updatedT; // Overwrite
            found = true;
            break;
        }
    }
    if (found) {
        saveTransactions();
        std::cout << "Transaction updated.\n";
    } else {
        std::cout << "Transaction ID not found.\n";
        undoStack.pop();
    }
}

const std::vector<Transaction>& ExpenseManager::getAllTransactions() const {
    return transactions;
}

std::vector<Transaction> ExpenseManager::getTransactionsByDate(const std::string& date) const {
    std::vector<Transaction> result;
    for (const auto& t : transactions) {
        if (t.getDate() == date) {
            result.push_back(t);
        }
    }
    return result;
}

std::vector<Transaction> ExpenseManager::getTransactionsByCategory(const std::string& category) const {
    std::vector<Transaction> result;
    for (const auto& t : transactions) {
        if (t.getCategory().find(category) != std::string::npos) { // Partial match
            result.push_back(t);
        }
    }
    return result;
}

double ExpenseManager::calculateTotalIncome() const {
    double total = 0;
    for (const auto& t : transactions) {
        if (t.getType() == TransactionType::INCOME) {
            total += t.getAmount();
        }
    }
    return total;
}

double ExpenseManager::calculateTotalExpense() const {
    double total = 0;
    for (const auto& t : transactions) {
        if (t.getType() == TransactionType::EXPENSE) {
            total += t.getAmount();
        }
    }
    return total;
}

double ExpenseManager::calculateBalance() const {
    return calculateTotalIncome() - calculateTotalExpense();
}

std::map<std::string, double> ExpenseManager::getCategorySummary() const {
    std::map<std::string, double> summary;
    for (const auto& t : transactions) {
        if (t.getType() == TransactionType::EXPENSE) {
            summary[t.getCategory()] += t.getAmount();
        }
    }
    return summary;
}

void ExpenseManager::undo() {
    if (undoStack.empty()) {
        std::cout << "Nothing to undo.\n";
        return;
    }
    transactions = undoStack.top();
    undoStack.pop();
    saveTransactions();
    std::cout << "Undo successful.\n";
}

void ExpenseManager::sortTransactionsByDate(bool ascending) {
    std::sort(transactions.begin(), transactions.end(), 
              [ascending](const Transaction& a, const Transaction& b) {
                  return ascending ? a.getDate() < b.getDate() : a.getDate() > b.getDate();
              });
}

void ExpenseManager::sortTransactionsByAmount(bool ascending) {
    std::sort(transactions.begin(), transactions.end(), 
              [ascending](const Transaction& a, const Transaction& b) {
                  return ascending ? a.getAmount() < b.getAmount() : a.getAmount() > b.getAmount();
              });
}
