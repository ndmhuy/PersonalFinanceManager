#include "Controllers/NavigationController.h"
#include <conio.h>
#include <iostream>

NavigationController::NavigationController(AppController* app) : appController(app), running(false) {}

NavigationController::~NavigationController() {}

void NavigationController::Init() {
    if (appController) {
        appController->loadData();
        view.ShowInfo("Data loaded.");
    }
}

void NavigationController::Run() {
    running = true;

    while (running) {
        // Show dashboard (uses appController internally)
        dashboard.Display();

        // Show main menu and get choice
        char choice = menus.DisplayMainMenu();

        if (choice == 27) { // ESC
            running = false;
            break;
        }

        HandleMainMenuChoice(choice);
    }

    Shutdown();
}

void NavigationController::Shutdown() {
    if (appController) {
        appController->saveData();
        view.ShowInfo("Data saved. Exiting...");
    }
}

void NavigationController::HandleMainMenuChoice(char c) {
    switch (c) {
        case '1':
            ShowIncomeFlow();
            break;
        case '2':
            ShowExpenseFlow();
            break;
        case '3':
            ShowReportsFlow();
            break;
        case '4':
            ShowWalletFlow();
            break;
        case '5':
            running = false;
            break;
        default:
            view.ShowError("Invalid selection. Try again.");
            break;
    }
}

void NavigationController::ShowIncomeFlow() {
    char c = menus.DisplayIncomeMenu();
    // Placeholder: actual handlers will be implemented later
    if (c == 27) return;
    view.ShowInfo("Income flow selected (placeholder)");
    PauseWithMessage("Press any key to return...");
}

void NavigationController::ShowExpenseFlow() {
    bool inExpenseMenu = true;
    
    while (inExpenseMenu) {
        char c = menus.DisplayExpenseMenu();
        
        if (c == 27) { // ESC - back to main menu
            break;
        }
        
        switch (c) {
            case '1':
                HandleAddExpense();
                break;
            case '2':
                HandleViewExpenses();
                break;
            case '3':
                HandleEditExpense();
                break;
            case '4':
                HandleDeleteExpense();
                break;
            default:
                view.ShowError("Invalid selection. Try again.");
                PauseWithMessage("Press any key to continue...");
                break;
        }
    }
}

void NavigationController::HandleAddExpense() {
    if (!appController) {
        view.ShowError("Controller not available.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    view.ClearScreen();
    view.PrintHeader("ADD EXPENSE");

    // Step 1: Select Wallet
    ArrayList<Wallet*>* wallets = appController->getWalletsList();
    if (!wallets || wallets->Count() == 0) {
        view.ShowError("No wallets available. Please create a wallet first.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    view.MoveToXY(5, 4);
    std::cout << "Available Wallets:" << std::endl;
    
    std::string headers[] = {"Index", "Wallet Name", "Balance"};
    int widths[] = {10, 30, 20};
    view.PrintTableHeader(headers, widths, 3);

    for (size_t i = 0; i < wallets->Count(); ++i) {
        Wallet* w = wallets->Get(i);
        view.PrintTableRow(std::to_string(i + 1), w->getName(), 
                          view.FormatCurrency(static_cast<long>(w->getBalance())));
    }
    view.PrintTableSeparator();

    view.MoveToXY(5, 5 + wallets->Count() + 3);
    std::cout << "Enter wallet index (1-" << wallets->Count() << "): ";
    int walletIdx;
    std::cin >> walletIdx;
    std::cin.ignore();

    if (walletIdx < 1 || walletIdx > static_cast<int>(wallets->Count())) {
        view.ShowError("Invalid wallet selection.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    Wallet* selectedWallet = wallets->Get(walletIdx - 1);
    std::string walletId = selectedWallet->getId();

    // Step 2: Get Amount
    view.ClearScreen();
    view.PrintHeader("ADD EXPENSE - AMOUNT");
    double amount = InputValidator::GetValidMoney("Enter amount: ");

    // Step 3: Get Date
    view.ClearScreen();
    view.PrintHeader("ADD EXPENSE - DATE");
    Date date = InputValidator::GetValidDate("Enter date (DD/MM/YYYY): ");

    // Step 4: Get Description
    view.ClearScreen();
    view.PrintHeader("ADD EXPENSE - DESCRIPTION");
    std::string description = InputValidator::GetValidString("Enter description: ");

    // Step 5: Select Category
    ArrayList<Category*>* categories = appController->getCategoriesList();
    if (!categories || categories->Count() == 0) {
        view.ShowError("No categories available. Please create a category first.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    view.ClearScreen();
    view.PrintHeader("ADD EXPENSE - CATEGORY");
    view.MoveToXY(5, 4);
    std::cout << "Available Categories:" << std::endl;

    std::string catHeaders[] = {"Index", "Category Name", ""};
    int catWidths[] = {10, 40, 20};
    view.PrintTableHeader(catHeaders, catWidths, 2);

    for (size_t i = 0; i < categories->Count(); ++i) {
        Category* cat = categories->Get(i);
        view.PrintTableRow(std::to_string(i + 1), cat->getName(), "");
    }
    view.PrintTableSeparator();

    view.MoveToXY(5, 5 + categories->Count() + 3);
    std::cout << "Enter category index (1-" << categories->Count() << "): ";
    int catIdx;
    std::cin >> catIdx;
    std::cin.ignore();

    if (catIdx < 1 || catIdx > static_cast<int>(categories->Count())) {
        view.ShowError("Invalid category selection.");
        PauseWithMessage("Press any key to continue...");
        return;
    }

    Category* selectedCategory = categories->Get(catIdx - 1);
    std::string categoryId = selectedCategory->getId();

    // Step 6: Add transaction to AppController
    try {
        appController->addTransaction(amount, walletId, categoryId, 
                                     TransactionType::Expense, date, description);
        
        view.ClearScreen();
        view.PrintHeader("EXPENSE ADDED SUCCESSFULLY");
        view.MoveToXY(5, 5);
        view.SetColor(10); // Green
        std::cout << "✓ Expense recorded!" << std::endl;
        view.ResetColor();
        
        view.MoveToXY(5, 7);
        std::cout << "Wallet: " << selectedWallet->getName() << std::endl;
        view.MoveToXY(5, 8);
        std::cout << "Amount: " << view.FormatCurrency(static_cast<long>(amount)) << std::endl;
        view.MoveToXY(5, 9);
        std::cout << "Category: " << selectedCategory->getName() << std::endl;
        view.MoveToXY(5, 10);
        std::cout << "Date: " << date.ToString() << std::endl;
        view.MoveToXY(5, 11);
        std::cout << "Description: " << description << std::endl;
        
        PauseWithMessage("Press any key to return to menu...");
    } catch (const std::exception& e) {
        view.ShowError(std::string("Error adding expense: ") + e.what());
        PauseWithMessage("Press any key to continue...");
    }
}

void NavigationController::HandleViewExpenses() {
    view.ClearScreen();
    view.PrintHeader("VIEW EXPENSES");
    view.ShowInfo("Feature not yet implemented (placeholder)");
    PauseWithMessage("Press any key to return...");
}

void NavigationController::HandleEditExpense() {
    view.ClearScreen();
    view.PrintHeader("EDIT EXPENSE");
    view.ShowInfo("Feature not yet implemented (placeholder)");
    PauseWithMessage("Press any key to return...");
}

void NavigationController::HandleDeleteExpense() {
    view.ClearScreen();
    view.PrintHeader("DELETE EXPENSE");
    view.ShowInfo("Feature not yet implemented (placeholder)");
    PauseWithMessage("Press any key to return...");
}

void NavigationController::ShowWalletFlow() {
    char c = menus.DisplayWalletMenu();
    if (c == 27) return;
    view.ShowInfo("Wallet flow selected (placeholder)");
    PauseWithMessage("Press any key to return...");
}

void NavigationController::ShowReportsFlow() {
    char c = menus.DisplayReportsMenu();
    if (c == 27) return;
    view.ShowInfo("Reports flow selected (placeholder)");
    PauseWithMessage("Press any key to return...");
}

void NavigationController::PauseWithMessage(const std::string& msg) {
    view.PrintFooter(msg);
    _getch();
}
