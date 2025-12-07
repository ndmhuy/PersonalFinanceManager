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

