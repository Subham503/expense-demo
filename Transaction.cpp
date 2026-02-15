#include "Transaction.h"
#include <sstream>
#include <iomanip>

Transaction::Transaction() : id(0), amount(0.0), type(TransactionType::EXPENSE) {}

Transaction::Transaction(int id, double amount, std::string category, TransactionType type, std::string note, std::string date)
    : id(id), amount(amount), category(category), type(type), note(note), date(date) {}

int Transaction::getId() const { return id; }
std::string Transaction::getDate() const { return date; }
std::string Transaction::getCategory() const { return category; }
double Transaction::getAmount() const { return amount; }
TransactionType Transaction::getType() const { return type; }
std::string Transaction::getNote() const { return note; }

void Transaction::setDate(const std::string& date) { this->date = date; }
void Transaction::setCategory(const std::string& category) { this->category = category; }
void Transaction::setAmount(double amount) { this->amount = amount; }
void Transaction::setType(TransactionType type) { this->type = type; }
void Transaction::setNote(const std::string& note) { this->note = note; }

std::string Transaction::toString() const {
    std::stringstream ss;
    // Format: ID|Date|Category|Amount|Type|Note
    ss << id << "|" << date << "|" << category << "|" << amount << "|" 
       << (type == TransactionType::INCOME ? "INCOME" : "EXPENSE") << "|" << note;
    return ss.str();
}

Transaction Transaction::fromString(const std::string& line) {
    std::stringstream ss(line);
    std::string segment;
    std::vector<std::string> parts;

    while (std::getline(ss, segment, '|')) {
        parts.push_back(segment);
    }

    if (parts.size() < 6) return Transaction();

    int id = std::stoi(parts[0]);
    std::string date = parts[1];
    std::string category = parts[2];
    double amount = std::stod(parts[3]);
    TransactionType type = (parts[4] == "INCOME") ? TransactionType::INCOME : TransactionType::EXPENSE;
    std::string note = parts[5];

    return Transaction(id, amount, category, type, note, date);
}

void Transaction::display() const {
    std::cout << std::left << std::setw(5) << id 
              << std::setw(12) << date 
              << std::setw(15) << category 
              << std::setw(10) << amount 
              << std::setw(10) << (type == TransactionType::INCOME ? "Income" : "Expense")
              << note << std::endl;
}
