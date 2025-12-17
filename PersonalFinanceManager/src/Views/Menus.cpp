#include "Views/Menus.h"
#include "Utils/PlatformUtils.h" // Replaces conio.h
#include <iostream>

//CONSTRUCTOR
Menus::Menus() {}

//MAIN MENU IMPLEMENTATION
char Menus::DisplayMainMenu() {
    view.ClearScreen();
    view.PrintHeader(MAIN_MENU_TITLE);
    view.PrintBox(8,5,31,7);

    view.MoveToXY(10, 6);
    std::cout << MAIN_MENU_1 << std::endl;
    view.MoveToXY(10, 7);
    std::cout << MAIN_MENU_2 << std::endl;
    view.MoveToXY(10, 8);
    std::cout << MAIN_MENU_3 << std::endl;
    view.MoveToXY(10, 9);
    std::cout << MAIN_MENU_4 << std::endl;
    view.MoveToXY(10, 10);
    std::cout << MAIN_MENU_5 << std::endl;

    view.PrintShortcutFooter("[1-5] Select | [ESC] Exit", "Main Menu");
    
    // CROSS-PLATFORM FIX
    return GetKeyPress();
}

//EXPENSE MENU IMPLEMENTATION
char Menus::DisplayExpenseMenu() {
    view.ClearScreen();
    view.PrintHeader(EXPENSE_MENU_TITLE);
    view.PrintBox(8,5,30,6);

    view.MoveToXY(10, 6);
    std::cout << EXPENSE_MENU_1 << std::endl;
    view.MoveToXY(10, 7);
    std::cout << EXPENSE_MENU_2 << std::endl;
    view.MoveToXY(10, 8);
    std::cout << EXPENSE_MENU_3 << std::endl;
    view.MoveToXY(10, 9);
    std::cout << EXPENSE_MENU_4 << std::endl;

    view.PrintShortcutFooter("[1-4] Select | [ESC] Back", "Expense Menu");
    
    return GetKeyPress();
}

//INCOME MENU IMPLEMENTATION
char Menus::DisplayIncomeMenu() {
    view.ClearScreen();
    view.PrintHeader(INCOME_MENU_TITLE);
    view.PrintBox(8,5,30,6);

    view.MoveToXY(10, 6);
    std::cout << INCOME_MENU_1 << std::endl;
    view.MoveToXY(10, 7);
    std::cout << INCOME_MENU_2 << std::endl;
    view.MoveToXY(10, 8);
    std::cout << INCOME_MENU_3 << std::endl;
    view.MoveToXY(10, 9);
    std::cout << INCOME_MENU_4 << std::endl;

    view.PrintShortcutFooter("[1-4] Select | [ESC] Back", "Income Menu");
    
    return GetKeyPress();
}

//WALLET MENU IMPLEMENTATION
char Menus::DisplayWalletMenu() {
    view.ClearScreen();
    view.PrintHeader(WALLET_MENU_TITLE);
    view.PrintBox(8,5,30,5);

    view.MoveToXY(10, 6);
    std::cout << WALLET_MENU_1 << std::endl;
    view.MoveToXY(10, 7);
    std::cout << WALLET_MENU_2 << std::endl;
    view.MoveToXY(10, 8);
    std::cout << WALLET_MENU_3 << std::endl;

    view.PrintShortcutFooter("[1-3] Select | [ESC] Back", "Wallet Menu");
    
    return GetKeyPress();
}

//REPORTS MENU IMPLEMENTATION
char Menus::DisplayReportsMenu() {
    view.ClearScreen();
    view.PrintHeader(REPORTS_MENU_TITLE);
    view.PrintBox(8,5,30,6);

    view.MoveToXY(10, 6);
    std::cout << REPORTS_MENU_1 << std::endl;
    view.MoveToXY(10, 7);
    std::cout << REPORTS_MENU_2 << std::endl;
    view.MoveToXY(10, 8);
    std::cout << REPORTS_MENU_3 << std::endl;
    view.MoveToXY(10, 9);
    std::cout << REPORTS_MENU_4 << std::endl;

    view.PrintShortcutFooter("[1-4] Select | [ESC] Back", "Reports Menu");
    
    return GetKeyPress();
}

//STRING CONSTANTS
// Main Menu
const std::string Menus::MAIN_MENU_TITLE = "PERSONAL FINANCE MANAGER - MAIN MENU";
const std::string Menus::MAIN_MENU_1 = "1. Income Management";
const std::string Menus::MAIN_MENU_2 = "2. Expense Management";
const std::string Menus::MAIN_MENU_3 = "3. View Reports & Statistics";
const std::string Menus::MAIN_MENU_4 = "4. Manage Wallets";
const std::string Menus::MAIN_MENU_5 = "5. Exit";

// Expense Menu
const std::string Menus::EXPENSE_MENU_TITLE = "EXPENSE MANAGEMENT";
const std::string Menus::EXPENSE_MENU_1 = "1. Add Expense";
const std::string Menus::EXPENSE_MENU_2 = "2. View Expenses";
const std::string Menus::EXPENSE_MENU_3 = "3. Edit Expense";
const std::string Menus::EXPENSE_MENU_4 = "4. Delete Expense";

// Income Menu
const std::string Menus::INCOME_MENU_TITLE = "INCOME MANAGEMENT";
const std::string Menus::INCOME_MENU_1 = "1. Add Income";
const std::string Menus::INCOME_MENU_2 = "2. View Income";
const std::string Menus::INCOME_MENU_3 = "3. Edit Income";
const std::string Menus::INCOME_MENU_4 = "4. Delete Income";

// Wallet Menu
const std::string Menus::WALLET_MENU_TITLE = "WALLET MANAGEMENT";
const std::string Menus::WALLET_MENU_1 = "1. Create Wallet";
const std::string Menus::WALLET_MENU_2 = "2. View Wallets";
const std::string Menus::WALLET_MENU_3 = "3. Delete Wallet";

// Reports Menu
const std::string Menus::REPORTS_MENU_TITLE = "REPORTS & STATISTICS";
const std::string Menus::REPORTS_MENU_1 = "1. Monthly Summary";
const std::string Menus::REPORTS_MENU_2 = "2. Spending by Category";
const std::string Menus::REPORTS_MENU_3 = "3. Income vs Expense";
const std::string Menus::REPORTS_MENU_4 = "4. Wallet Balance Overview";

// Add Income Form
const std::string Menus::ADD_INCOME_TITLE = "=== ADD INCOME ===";
const std::string Menus::SELECT_SOURCE_PROMPT = "Enter Income Source ID: ";

// Add Expense Form
const std::string Menus::ADD_EXPENSE_TITLE = "=== ADD EXPENSE ===";
const std::string Menus::SELECT_CATEGORY_PROMPT = "Enter Category ID: ";
