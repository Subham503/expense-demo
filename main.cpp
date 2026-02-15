#include <iostream>
#include <limits>
#include "AuthSystem.h"
#include "ExpenseManager.h"
#include "BudgetManager.h"
#include "ReportGenerator.h"
#include "Utils.h"

void waitForEnter() {
    std::cout << "\nPress Enter to continue...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void printHeader(const std::string& title) {
    Utils::clearScreen();
    std::cout << "========================================\n";
    std::cout << "   " << title << "\n";
    std::cout << "========================================\n";
}

int main() {
    AuthSystem auth;
    
    while (true) {
        if (!auth.isLoggedIn()) {
            printHeader("Welcome to Advanced Expense Tracker");
            std::cout << "1. Login\n";
            std::cout << "2. Register\n";
            std::cout << "3. Exit\n";
            std::cout << "Enter choice: ";
            
            int choice;
            if (!(std::cin >> choice)) {
                std::cin.clear();
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                continue;
            }

            if (choice == 3) break;

            std::string username, password;
            if (choice == 1) {
                std::cout << "Username: ";
                std::cin >> username;
                std::cout << "Password: ";
                std::cin >> password;
                if (auth.login(username, password)) {
                    std::cout << "Login successful!\n";
                } else {
                    std::cout << "Invalid credentials.\n";
                    waitForEnter();
                }
            } else if (choice == 2) {
                std::cout << "Enter new username: ";
                std::cin >> username;
                std::cout << "Enter new password: ";
                std::cin >> password;
                if (auth.registerUser(username, password)) {
                    std::cout << "Registration successful! You can now login.\n";
                } else {
                    std::cout << "Username already taken.\n";
                }
                waitForEnter();
            }
        } else {
            // User is logged in
            std::string user = auth.getCurrentUser();
            ExpenseManager expenseMgr;
            expenseMgr.loadTransactions(user);
            
            BudgetManager budgetMgr(user + "_budget.txt"); // User specific budget file

            while (auth.isLoggedIn()) {
                printHeader("Dashboard - " + user);
                
                // Show Quick Stats
                double balance = expenseMgr.calculateBalance();
                std::cout << "Current Balance: " << balance << "\n";
                
                // Show Budget Status
                double totalExpense = expenseMgr.calculateTotalExpense(); 
                // Note: Budget is usually strictly monthly, but here we compare total for simplicity
                // or we should filter expenses by current month for budget check.
                // Let's filter by current month for budget check.
                std::string currentMonth = Utils::getCurrentDate().substr(0, 7);
                double monthlyExpense = 0;
                for (const auto& t : expenseMgr.getAllTransactions()) {
                    if (t.getDate().find(currentMonth) == 0 && t.getType() == TransactionType::EXPENSE) {
                        monthlyExpense += t.getAmount();
                    }
                }
                
                std::cout << budgetMgr.checkBudget(monthlyExpense) << "\n";
                std::cout << "----------------------------------------\n";
                
                std::cout << "1. Add Transaction\n";
                std::cout << "2. View Transactions\n";
                std::cout << "3. Expense Analytics & Reports\n";
                std::cout << "4. Budget Settings\n";
                std::cout << "5. Undo Last Action\n";
                std::cout << "6. Logout\n";
                std::cout << "Enter choice: ";

                int choice;
                if (!(std::cin >> choice)) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    continue;
                }

                if (choice == 1) {
                    printHeader("Add Transaction");
                    double amount;
                    std::string category, note, typeStr, date;
                    int typeInt;

                    std::cout << "Type (1: Income, 2: Expense): ";
                    std::cin >> typeInt;
                    TransactionType type = (typeInt == 1) ? TransactionType::INCOME : TransactionType::EXPENSE;

                    std::cout << "Amount: ";
                    std::cin >> amount;
                    
                    std::cout << "Category (single word): ";
                    std::cin >> category; // Simple cin for single word
                    
                    std::cin.ignore(); // Clear buffer
                    std::cout << "Note (optional): ";
                    std::getline(std::cin, note);
                    
                    std::cout << "Date (YYYY-MM-DD) [Enter for today]: ";
                    std::string dateInput;
                    std::getline(std::cin, dateInput);
                    if (dateInput.empty()) date = Utils::getCurrentDate();
                    else date = dateInput;

                    // Generate a random ID (simple) or max ID + 1
                    int newId = 1;
                    if (!expenseMgr.getAllTransactions().empty()) {
                         // Find max id
                         for(const auto& t : expenseMgr.getAllTransactions()) {
                             if(t.getId() >= newId) newId = t.getId() + 1;
                         }
                    }

                    Transaction t(newId, amount, category, type, note, date);
                    expenseMgr.addTransaction(t);
                    std::cout << "Transaction added!\n";
                    waitForEnter();

                } else if (choice == 2) {
                    printHeader("View Transactions");
                    std::cout << "1. All Transactions\n";
                    std::cout << "2. Filter by Month\n";
                    std::cout << "3. Filter by Category\n";
                    std::cout << "Choice: ";
                    int viewChoice;
                    std::cin >> viewChoice;

                    std::vector<Transaction> toShow;
                    if (viewChoice == 1) {
                         toShow = expenseMgr.getAllTransactions();
                    } else if (viewChoice == 2) {
                        std::cout << "Enter Month (YYYY-MM): ";
                        std::string m;
                        std::cin >> m;
                        // Manual filter or use helper
                        for(const auto& t : expenseMgr.getAllTransactions()) {
                            if(t.getDate().find(m) == 0) toShow.push_back(t);
                        }
                    } else if (viewChoice == 3) {
                        std::cout << "Category: ";
                        std::string c;
                        std::cin >> c;
                        toShow = expenseMgr.getTransactionsByCategory(c);
                    }

                    // Sort option
                    std::cout << "Sort by (1: Date, 2: Amount): ";
                    int sortC;
                    std::cin >> sortC;
                    if (sortC == 1) {
                         std::sort(toShow.begin(), toShow.end(), [](const Transaction& a, const Transaction& b){
                             return a.getDate() < b.getDate();
                         });
                    } else {
                         std::sort(toShow.begin(), toShow.end(), [](const Transaction& a, const Transaction& b){
                             return a.getAmount() < b.getAmount();
                         });
                    }

                    std::cout << "\nID | Date | Category | Amount | Type | Note\n";
                    for(const auto& t : toShow) {
                        t.display();
                    }
                    
                    std::cout << "\nActions: (0) Back (1) Delete (2) CSV Export\nChoice: ";
                    int act;
                    std::cin >> act;
                    if (act == 1) {
                        std::cout << "Enter ID to delete: ";
                        int delId;
                        std::cin >> delId;
                        expenseMgr.deleteTransaction(delId);
                    } else if (act == 2) {
                        ReportGenerator::exportToCSV(toShow, "export.csv");
                    }
                    
                    waitForEnter();

                } else if (choice == 3) {
                    printHeader("Analytics");
                    std::cout << "1. Monthly Report\n";
                    std::cout << "2. Spending Graph\n";
                    std::cout << "Choice: ";
                    int anChoice;
                    std::cin >> anChoice;

                    if (anChoice == 1) {
                        std::cout << "Enter Month (YYYY-MM): ";
                        std::string m;
                        std::cin >> m;
                        ReportGenerator::generateMonthlyReport(expenseMgr, m);
                    } else if (anChoice == 2) {
                        ReportGenerator::plotSpendingGraph(expenseMgr.getAllTransactions());
                    }
                    waitForEnter();

                } else if (choice == 4) {
                    printHeader("Budget Settings");
                    std::cout << "Current Monthly Budget: " << budgetMgr.getBudget() << "\n";
                    std::cout << "Enter new budget limit: ";
                    double limit;
                    std::cin >> limit;
                    budgetMgr.setBudget(limit);
                    std::cout << "Budget updated.\n";
                    waitForEnter();
                } else if (choice == 5) {
                    expenseMgr.undo();
                    waitForEnter();
                } else if (choice == 6) {
                    auth.logout();
                }
            }
        }
    }
    return 0;
}
