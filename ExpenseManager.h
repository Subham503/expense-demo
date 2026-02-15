#ifndef EXPENSEMANAGER_H
#define EXPENSEMANAGER_H

#include <vector>
#include <string>
#include <map>
#include <stack>
#include <algorithm>
#include "Transaction.h"

class ExpenseManager {
private:
    std::vector<Transaction> transactions;
    std::string currentUser;
    std::stack<std::vector<Transaction>> undoStack; // Simple undo: store state before change

    void saveStateForUndo();
    std::string getFilename() const;
    void saveTransactions();

public:
    ExpenseManager();
    void loadTransactions(const std::string& username);
    
    // CRUD
    void addTransaction(const Transaction& t);
    void deleteTransaction(int id);
    void editTransaction(int id, const Transaction& updatedT);
    
    // Getters / Search
    const std::vector<Transaction>& getAllTransactions() const;
    std::vector<Transaction> getTransactionsByDate(const std::string& date) const;
    std::vector<Transaction> getTransactionsByCategory(const std::string& category) const;
    
    // Analytics
    double calculateTotalIncome() const;
    double calculateTotalExpense() const;
    double calculateBalance() const;
    std::map<std::string, double> getCategorySummary() const;
    
    // Advanced Features
    void undo();
    void sortTransactionsByDate(bool ascending = true);
    void sortTransactionsByAmount(bool ascending = true);
};

#endif // EXPENSEMANAGER_H
