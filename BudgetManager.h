#ifndef BUDGETMANAGER_H
#define BUDGETMANAGER_H

#include <string>
#include <iostream>
#include <fstream>

class BudgetManager {
private:
    double monthlyLimit;
    std::string budgetFile;

    void loadBudget();
    void saveBudget();

public:
    BudgetManager(const std::string& filename = "budget.txt");

    void setBudget(double limit);
    double getBudget() const;
    
    // Returns status message: "OK", "WARNING: 80% used", "ALERT: Budget Exceeded!"
    std::string checkBudget(double currentSpending) const;
    double getUsagePercentage(double currentSpending) const;
};

#endif // BUDGETMANAGER_H
