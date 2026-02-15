#include "BudgetManager.h"

BudgetManager::BudgetManager(const std::string& filename) : budgetFile(filename), monthlyLimit(0.0) {
    loadBudget();
}

void BudgetManager::loadBudget() {
    std::ifstream file(budgetFile);
    if (file.is_open()) {
        file >> monthlyLimit;
        file.close();
    }
}

void BudgetManager::saveBudget() {
    std::ofstream file(budgetFile);
    if (file.is_open()) {
        file << monthlyLimit;
        file.close();
    }
}

void BudgetManager::setBudget(double limit) {
    monthlyLimit = limit;
    saveBudget();
}

double BudgetManager::getBudget() const {
    return monthlyLimit;
}

std::string BudgetManager::checkBudget(double currentSpending) const {
    if (monthlyLimit == 0) return "No Budget Set";

    double percentage = (currentSpending / monthlyLimit) * 100.0;

    if (percentage >= 100.0) {
        return "ALERT: Budget Exceeded! (" + std::to_string((int)percentage) + "%)";
    } else if (percentage >= 80.0) {
        return "WARNING: " + std::to_string((int)percentage) + "% of budget used.";
    }
    return "Budget Status: OK (" + std::to_string((int)percentage) + "%)";
}

double BudgetManager::getUsagePercentage(double currentSpending) const {
    if (monthlyLimit == 0) return 0.0;
    return (currentSpending / monthlyLimit) * 100.0;
}
