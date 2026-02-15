# Advanced CLI Expense Tracker (C++)

A professional, modular, and interactive command-line expense tracker built in C++ using OOP principles and STL.

## Features

-   **Authentication System**: Secure user registration and login.
-   **Transaction Management**: Add, view, edit, search, and delete transactions.
-   **Budgeting**: Set monthly budgets with visual warnings (80% usage) and alerts (100% exceeded).
-   **Analytics**: View monthly summaries, category-wise breakdown, and text-based spending graphs.
-   **Persistence**: Automatic file-based saving for users and transactions.
-   **CSV Export**: Export transaction history to CSV for Excel/Google Sheets.
-   **Undo**: Stack-based undo functionality for the last action.

## Project Structure

The project follows a modular architecture:

-   `AccessSystem`: Handles user authentication and session management.
-   `Transaction`: Represents individual financial records.
-   `ExpenseManager`: Manages the collection of transactions and business logic.
-   `BudgetManager`: Handles budget tracking and alerts.
-   `ReportGenerator`: Generates analysis and visualizations.

## Getting Started

### Prerequisites

-   A C++ Compiler (g++, clang++, or MSVC).

### Installation

1.  Clone the repository:
    ```bash
    git clone https://github.com/yourusername/expense-tracker-cpp.git
    ```
2.  Navigate to the directory:
    ```bash
    cd expense-tracker-cpp
    ```

### Compilation

Compile the source files using g++:

```bash
g++ main.cpp Transaction.cpp ExpenseManager.cpp BudgetManager.cpp AuthSystem.cpp ReportGenerator.cpp -o expense_tracker
```

### Usage

Run the executable:

```bash
./expense_tracker
```

## Future Improvements

-   Add more complex data visualization.
-   Implement a database (SQLite) instead of text files.
-   Add a GUI (Qt or wxWidgets).
