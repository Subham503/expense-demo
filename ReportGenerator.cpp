#include "ReportGenerator.h"
#include <fstream>
#include <iostream>
#include <iomanip>
#include <map>
#include <cmath>

void ReportGenerator::exportToCSV(const std::vector<Transaction>& transactions, const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Could not open file for export: " << filename << std::endl;
        return;
    }

    file << "ID,Date,Category,Amount,Type,Note\n";
    for (const auto& t : transactions) {
        file << t.getId() << "," 
             << t.getDate() << "," 
             << t.getCategory() << "," 
             << t.getAmount() << "," 
             << (t.getType() == TransactionType::INCOME ? "Income" : "Expense") << "," 
             << t.getNote() << "\n";
    }
    file.close();
    std::cout << "Data exported successfully to " << filename << std::endl;
}

void ReportGenerator::plotSpendingGraph(const std::vector<Transaction>& transactions) {
    std::map<std::string, double> categoryTotals;
    double maxTotal = 0;

    for (const auto& t : transactions) {
        if (t.getType() == TransactionType::EXPENSE) {
            categoryTotals[t.getCategory()] += t.getAmount();
            if (categoryTotals[t.getCategory()] > maxTotal) {
                maxTotal = categoryTotals[t.getCategory()];
            }
        }
    }

    if (categoryTotals.empty()) {
        std::cout << "No expenses to plot.\n";
        return;
    }

    std::cout << "\n--- Spending Graph (Category vs Amount) ---\n";
    for (const auto& pair : categoryTotals) {
        std::cout << std::left << std::setw(15) << pair.first << " | ";
        
        // Scale bar; max length 50 chars
        int barLength = (int)((pair.second / maxTotal) * 50);
        for (int i = 0; i < barLength; ++i) std::cout << "#";
        
        std::cout << " " << pair.second << "\n";
    }
    std::cout << "-------------------------------------------\n";
}

void ReportGenerator::generateMonthlyReport(const ExpenseManager& manager, const std::string& monthYYYY_MM) {
    auto allTransactions = manager.getAllTransactions();
    std::vector<Transaction> filtered;
    double totalIncome = 0;
    double totalExpense = 0;

    for (const auto& t : allTransactions) {
        // Simple check: if date starts with YYYY-MM
        if (t.getDate().substr(0, 7) == monthYYYY_MM) {
            filtered.push_back(t);
            if (t.getType() == TransactionType::INCOME) totalIncome += t.getAmount();
            else totalExpense += t.getAmount();
        }
    }

    std::cout << "\n=== Monthly Report for " << monthYYYY_MM << " ===\n";
    std::cout << "Total Income:  " << totalIncome << "\n";
    std::cout << "Total Expense: " << totalExpense << "\n";
    std::cout << "Net Savings:   " << (totalIncome - totalExpense) << "\n";
    std::cout << "Transaction Count: " << filtered.size() << "\n";
    
    // Find highest spending category for this month
    std::map<std::string, double> cats;
    std::string highCat = "None";
    double highVal = 0;
    
    for (const auto& t : filtered) {
        if (t.getType() == TransactionType::EXPENSE) {
            cats[t.getCategory()] += t.getAmount();
            if (cats[t.getCategory()] > highVal) {
                highVal = cats[t.getCategory()];
                highCat = t.getCategory();
            }
        }
    }
    
    std::cout << "Highest Spending Category: " << highCat << " (" << highVal << ")\n";
    std::cout << "======================================\n";
}
