#include "../../include/Views/ConsoleView.h"
#include <iomanip>
#include <sstream>

// ===== CONSOLE CONTROL IMPLEMENTATIONS =====

void ConsoleView::MoveToXY(int x, int y) {
    COORD coord = {(short)x, (short)y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void ConsoleView::SetColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (WORD)color);
}

void ConsoleView::ClearScreen() {
    system("cls");
}

void ConsoleView::ResetColor() {
    SetColor(COLOR_NORMAL);
}

// ===== DRAWING PRIMITIVES =====

void ConsoleView::PrintLine(int x, int y, int length, char ch) {
    MoveToXY(x,y);
    for (int i = 0; i < length; ++i) cout << ch;
    cout << endl;
}

void ConsoleView::PrintBox(int x, int y, int width, int height) {
    MoveToXY(x,y);
    cout << string(width,'=') << endl;
    for (int i = 1; i< height - 1; ++i) {
        MoveToXY(x,y+i);
        cout << "║" << string(width - 2, ' ') << '║' << endl;
    }
    MoveToXY(x, y + height - 1);
    cout << string(width, '=') << endl;
}


void ConsoleView::PrintHeader(string title) {
    ClearScreen();
    MoveToXY(0, 0);
    SetColor(COLOR_HEADER);
    PrintLine(0, 0, 80, '=');   
    MoveToXY((80 - title.length()) / 2, 1);
    cout << ' ' << title << ' ';
    PrintLine(0, 3, 80, '-');
    ResetColor();
}

void ConsoleView::PrintFooter(string message) {
    MoveToXY(0, 23);
    PrintLine(0, 23, 80, '-');
    MoveToXY(2, 24);
    SetColor(COLOR_INFO);
    cout << message;
    MoveToXY(0, 25);
    ResetColor();
}

void ConsoleView::PrintShortcutFooter(string shortcuts, string status) {
    MoveToXY(0, 23);
    PrintLine(0, 23, 80, '-');
    MoveToXY(2, 24);
    SetColor(COLOR_WARNING);
    cout << shortcuts;
    MoveToXY(50, 24);
    SetColor(COLOR_INFO);
    cout << status;
    ResetColor();
    MoveToXY(0, 25);
}

// ===== MESSAGE IMPLEMENTATIONS =====

void ConsoleView::ShowSuccess(string message) {
    SetColor(COLOR_SUCCESS);
    cout << "[✓] " << message << endl;
    ResetColor();
}

void ConsoleView::ShowError(string message) {
    SetColor(COLOR_ERROR);
    cout << "[✗] " << message << endl;
    ResetColor();
}

void ConsoleView::ShowWarning(string message) {
    SetColor(COLOR_WARNING);
    cout << "[!] " << message << endl;
    ResetColor();
}

void ConsoleView::ShowInfo(string message) {
    SetColor(COLOR_INFO);
    cout << message << endl;
    ResetColor();
}

