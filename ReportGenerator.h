#ifndef REPORTGENERATOR_H
#define REPORTGENERATOR_H

#include "ExpenseManager.h"
#include <string>
#include <vector>

class ReportGenerator {
public:
    static void exportToCSV(const std::vector<Transaction>& transactions, const std::string& filename);
    
    // Generates a simple text-based bar chart of expenses by category
    static void plotSpendingGraph(const std::vector<Transaction>& transactions);
    
    // Generates a detailed monthly report to console
    static void generateMonthlyReport(const ExpenseManager& manager, const std::string& monthYYYY_MM);
};

#endif // REPORTGENERATOR_H
