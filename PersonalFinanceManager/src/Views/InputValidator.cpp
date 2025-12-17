#include "Views/InputValidator.h"
#include "Views/ConsoleView.h"
#include <iostream>
#include <cctype>

double InputValidator::GetValidMoney(const std::string& prompt) {
    double amount;
    while (true) {
        std::cout << prompt;
        if (std::cin >> amount && ValidateMoney(amount)) {
            std::cin.ignore(); // Clear input buffer
            return amount;
        }
        // Invalid input
        std::cin.clear();
        std::cin.ignore(10000, '\n');
        ConsoleView view;
        view.ShowError("Invalid input! Please enter an amount greater than 0.");
    }
}

Date InputValidator::GetValidDate(const std::string& prompt) {
    std::string dateStr;
    while (true) {
        std::cout << prompt;
        getline(std::cin, dateStr);
        
        try {
            Date date = Date::FromString(dateStr);
            if (ValidateDate(date)) {
                return date;
            }
        } catch (...) {
            // Invalid date format
        }
        
        ConsoleView view;
        view.ShowError("Invalid date! Please enter date in format YYYY-MM-DD.");
    }
}

std::string InputValidator::GetValidString(const std::string& prompt) {
    std::string input;
    while (true) {
        std::cout << prompt;
        getline(std::cin, input);
        
        if (ValidateString(input)) {
            return input;
        }
        
        ConsoleView view;
        view.ShowError("Invalid input! Text cannot be empty.");
    }
}

bool InputValidator::ValidateMoney(double amount) {
    return amount > 0;
}

bool InputValidator::ValidateDate(const Date& date) {
    // Check if date is not default (0/0/0)
    return !(date.GetDay() == 0 && date.GetMonth() == 0 && date.GetYear() == 0);
}

bool InputValidator::ValidateString(const std::string& str) {
    // Remove leading/trailing whitespace check
    if (str.empty()) return false;
    
    // Check if string contains only whitespace
    for (char c : str) {
        if (!isspace(c)) return true;
    }
    return false;
}
