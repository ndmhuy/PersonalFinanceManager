#ifndef Dashboard_h
#define Dashboard_h

#include "Views/ConsoleView.h"
#include "Utils/ArrayList.h"

// FORWARD DECLARATION
class Wallet;

class Dashboard {
private:
    ConsoleView view;

public:
    /// @brief Displays main dashboard with total balance and wallet list
    void Display();

    /// @brief Show main menu options 
    void ShowMainMenu();
};

#endif // !Dashboard_h
