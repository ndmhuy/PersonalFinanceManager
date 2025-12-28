#include "Controllers/NavigationController.h"
#include "Controllers/AppController.h"
#include "Utils/PlatformUtils.h"

// Include Models
#include "Models/Transaction.h"
#include "Models/Income.h"
#include "Models/Expense.h"
#include "Models/Wallet.h"
#include "Models/Category.h"
#include "Models/IncomeSource.h"
#include "Models/RecurringTransaction.h"

#include "Views/InputValidator.h"

#include <iostream>
#include <sstream>
#include <iomanip>

struct ReportStat {
    std::string id;
    double amount;
    ReportStat(std::string i, double a) : id(i), amount(a) {}
};

static bool GetReportDateRange(Date& start, Date& end) {
    ConsoleView view;
    view.ClearScreen();
    view.PrintHeader("SELECT REPORT PERIOD");
    
    std::cout << "1. Current Month" << std::endl;
    std::cout << "2. Specific Year (Annual Report)" << std::endl;
    std::cout << "3. Custom Date Range" << std::endl;
    std::cout << "0. Back" << std::endl;
    
    int choice = InputValidator::GetValidIndex("Select option: ", 0, 3);
    
    Date today = Date::GetTodayDate();
    
    if (choice == 0) return false;
    
    if (choice == 1) {
        start = Date(1, today.GetMonth(), today.GetYear());
        end = Date::GetEndOfMonth(today.GetMonth(), today.GetYear());
    } 
    else if (choice == 2) {
        int year = InputValidator::GetValidIndex("Enter Year (e.g. 2024): ", 1900, 2100);
        start = Date(1, 1, year);
        end = Date(31, 12, year);
    } 
    else if (choice == 3) {
        start = InputValidator::GetValidDate("Start Date (YYYY-MM-DD): ");
        end = InputValidator::GetValidDate("End Date (YYYY-MM-DD): ");
        if (start > end) {
            view.ShowError("Start date cannot be after end date.");
            view.ShowInfo("Press any key to retry...");
            GetKeyPress(); 
            return false;
        }
    }
    return true;
}

void NavigationController::ShowReportsFlow() {
    bool inReportsMenu = true;
    while (inReportsMenu) {
        char c = menus.DisplayReportsMenu();
        if (c == 27) break; // ESC

        switch (c) {
            case '1': HandleMonthlySummary(); break;      
            case '2': HandleSpendingByCategory(); break;  
            case '3': HandleIncomeVsExpense(); break;     
            case '4': HandleWalletBalanceOverview(); break; 
            case '5': HandleIncomeBySource(); break;      
            default:
                view.ShowError("Invalid selection. Try again.");
                PauseWithMessage("Press any key to continue...");
                break;
        }
    }
}

void NavigationController::HandleMonthlySummary() {
    Date start, end;
    if (!GetReportDateRange(start, end)) return;

    view.ClearScreen();
    view.PrintHeader("FINANCIAL SUMMARY");
    view.PrintText("Period: " + start.ToString() + " to " + end.ToString());

    ArrayList<Transaction*>* list = appController->GetTransactionsByDateRange(start, end);
    
    double totalIncome = 0;
    double totalExpense = 0;

    for (size_t i = 0; i < list->Count(); ++i) {
        Transaction* t = list->Get(i);
        if (t->GetType() == TransactionType::Income) totalIncome += t->GetAmount();
        else totalExpense += t->GetAmount();
    }

    double net = totalIncome - totalExpense;

    view.MoveToXY(5, 5);
    view.PrintText("Total Income : " + view.FormatCurrency((long long)totalIncome));
    view.MoveToXY(5, 6);
    view.PrintText("Total Expense: " + view.FormatCurrency((long long)totalExpense));
    view.PrintLine(5, 7, 30);
    
    view.MoveToXY(5, 8);
    if (net >= 0) view.SetColor(ConsoleView::COLOR_SUCCESS);
    else view.SetColor(ConsoleView::COLOR_ERROR);
    view.PrintText("Net Balance  : " + view.FormatCurrency((long long)net));
    view.ResetColor();

    delete list;
    PauseWithMessage("Press any key to continue...");
}

void NavigationController::HandleSpendingByCategory() {
    Date start, end;
    if (!GetReportDateRange(start, end)) return;

    view.ClearScreen();
    view.PrintHeader("EXPENSE BY CATEGORY");
    view.PrintText("Period: " + start.ToString() + " to " + end.ToString());

    ArrayList<Transaction*>* list = appController->GetTransactionsByDateRange(start, end);
    
    ArrayList<ReportStat*> stats;
    double totalExp = 0;

    for (size_t i = 0; i < list->Count(); ++i) {
        Transaction* t = list->Get(i);
        if (t->GetType() == TransactionType::Expense) {
            double amount = t->GetAmount();
            std::string catId = t->GetCategoryId();
            totalExp += amount;

            bool found = false;
            for (size_t j = 0; j < stats.Count(); ++j) {
                if (stats.Get(j)->id == catId) {
                    stats.Get(j)->amount += amount;
                    found = true;
                    break;
                }
            }
            if (!found) {
                stats.Add(new ReportStat(catId, amount));
            }
        }
    }

    if (totalExp == 0) {
        view.ShowInfo("No expenses in this period.");
    } else {
        std::string headers[] = {"Category", "Amount", "%"};
        int widths[] = {30, 20, 10};
        view.PrintTableHeader(headers, widths, 3);

        for (size_t i = 0; i < stats.Count(); ++i) {
            ReportStat* item = stats.Get(i);
            Category* c = appController->GetCategoryById(item->id);
            std::string name = c ? c->GetName() : "Unknown";
            
            double pct = (item->amount / totalExp) * 100.0;
            std::ostringstream ss; ss << std::fixed << std::setprecision(1) << pct << "%";
            
            std::string row[] = {name, view.FormatCurrency((long long)item->amount), ss.str()};
            view.PrintTableRow(row, widths, 3);
            
            delete item;
        }
        view.PrintTableSeparator(widths, 3);
        view.PrintText("Total Expense: " + view.FormatCurrency((long long)totalExp));
    }

    delete list;
    PauseWithMessage("Press any key to continue...");
}

void NavigationController::HandleIncomeBySource() {
    Date start, end;
    if (!GetReportDateRange(start, end)) return;

    view.ClearScreen();
    view.PrintHeader("INCOME BY SOURCE");
    view.PrintText("Period: " + start.ToString() + " to " + end.ToString());

    ArrayList<Transaction*>* list = appController->GetTransactionsByDateRange(start, end);
    
    ArrayList<ReportStat*> stats;
    double totalInc = 0;

    for (size_t i = 0; i < list->Count(); ++i) {
        Transaction* t = list->Get(i);
        if (t->GetType() == TransactionType::Income) {
            double amount = t->GetAmount();
            std::string srcId = t->GetCategoryId();
            totalInc += amount;

            bool found = false;
            for (size_t j = 0; j < stats.Count(); ++j) {
                if (stats.Get(j)->id == srcId) {
                    stats.Get(j)->amount += amount;
                    found = true;
                    break;
                }
            }
            if (!found) {
                stats.Add(new ReportStat(srcId, amount));
            }
        }
    }

    if (totalInc == 0) {
        view.ShowInfo("No income in this period.");
    } else {
        std::string headers[] = {"Source", "Amount", "%"};
        int widths[] = {30, 20, 10};
        view.PrintTableHeader(headers, widths, 3);

        for (size_t i = 0; i < stats.Count(); ++i) {
            ReportStat* item = stats.Get(i);
            IncomeSource* s = appController->GetIncomeSourceById(item->id);
            std::string name = s ? s->GetName() : "Unknown";
            
            double pct = (item->amount / totalInc) * 100.0;
            std::ostringstream ss; ss << std::fixed << std::setprecision(1) << pct << "%";
            
            std::string row[] = {name, view.FormatCurrency((long long)item->amount), ss.str()};
            view.PrintTableRow(row, widths, 3);
            
            delete item;
        }
        view.PrintTableSeparator(widths, 3);
        view.PrintText("Total Income: " + view.FormatCurrency((long long)totalInc));
    }

    delete list;
    PauseWithMessage("Press any key to continue...");
}

void NavigationController::HandleIncomeVsExpense() {
    HandleMonthlySummary(); 
}

void NavigationController::HandleWalletBalanceOverview() {
    view.ClearScreen();
    view.PrintHeader("WALLET BALANCES");

    ArrayList<Wallet*>* wallets = appController->GetWalletsList();
    double total = appController->GetTotalBalance();

    std::string headers[] = {"Wallet", "Balance", "%"};
    int widths[] = {30, 20, 10};
    view.PrintTableHeader(headers, widths, 3);

    for (size_t i = 0; i < wallets->Count(); ++i) {
        Wallet* w = wallets->Get(i);
        
        double pct = (total > 0) ? (w->GetBalance() / total * 100.0) : 0;
        std::ostringstream ss; ss << std::fixed << std::setprecision(1) << pct << "%";
        
        std::string row[] = {w->GetName(), view.FormatCurrency((long long)w->GetBalance()), ss.str()};
        view.PrintTableRow(row, widths, 3);
    }
    view.PrintTableSeparator(widths, 3);
    view.PrintText("TOTAL ASSETS: " + view.FormatCurrency((long long)total));

    PauseWithMessage("Press any key to continue...");
}   