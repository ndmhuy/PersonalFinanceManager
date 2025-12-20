#include "Views/ConsoleView.h"
#include "Utils/PlatformUtils.h"
#include <sstream>

using namespace std;

//CONSOLE CONTROL IMPLEMENTATIONS

void ConsoleView::MoveToXY(int x, int y) {
    // Update internal cursor tracking when moving cursor
    cursorY = y;
    MoveCursor(x, y);
}


void ConsoleView::SetColor(int color) {
    SetConsoleColor(color);
}

void ConsoleView::ClearScreen() {
    ::ClearScreen();
    // Reset internal cursor tracking to top-left and reset measured width
    cursorY = 0;
    contentWidth = 80;
    // Reset footer baseline
    lastFooterY = 23;
} 

void ConsoleView::ResetColor() {
    SetConsoleColor(COLOR_NORMAL);
}

//DRAWING IMPLEMENTATIONS

void ConsoleView::PrintLine(int x, int y, int length, char ch) {
    MoveToXY(x, y);
    for (int i = 0; i < length; i++) {
        cout << ch;
    }
    cout << endl;
    // After printing a full line and newline, update cursor row to the next line
    cursorY = y + 1;
    // Consider this as content that may be wide
    if (length > contentWidth) contentWidth = length;
}

void ConsoleView::PrintBox(int x, int y, int width, int height) {
    // Top border: +-----+
    MoveToXY(x, y);
    cout << "+";
    for (int i = 0; i < width - 2; i++) cout << "-";
    cout << "+";

    // Middle: |     |
    for (int row = 1; row < height - 1; row++) {
        MoveToXY(x, y + row);
        cout << "|";
        for (int i = 0; i < width - 2; i++) cout << " ";
        cout << "|";
    }

    // Bottom border
    MoveToXY(x, y + height - 1);
    cout << "+";
    for (int i = 0; i < width - 2; i++) cout << "-";
    cout << "+";
}

void ConsoleView::PrintHeader(string title, int requestedWidth) {
    ClearScreen();

    // Determine final width: prefer requestedWidth > 0, else use measured contentWidth (minimum 80)
    int width = 0;
    if (requestedWidth > 0) width = requestedWidth;
    else width = (contentWidth < 80) ? 80 : contentWidth;

    if (width < 80) width = 80; // ensure minimum

    // Track width
    if (width > contentWidth) contentWidth = width;

    // Top line
    SetColor(COLOR_HEADER);
    PrintLine(0, 0, width, '=');

    // Title centered on line 1
    int startX = (width - (int)(title.length())) / 2;
    if (startX < 0) startX = 0;
    MoveToXY(startX, 1);
    cout << title;
    // Track potential content width if title is long
    if ((int)title.length() > contentWidth) contentWidth = (int)title.length();

    // Separator line
    PrintLine(0, 3, width, '-');
    ResetColor();
}

void ConsoleView::PrintFooter(string message) {
    // Place separator and footer message below the error line recorded from PrintShortcutFooter
    int sepY = (lastFooterY >= 23) ? lastFooterY + 3 : 23 + 3;

    // Fixed-width footer separator
    PrintLine(0, sepY, 80, '-');

    // Footer text (fixed width 80)
    MoveToXY(1, sepY + 1);
    SetColor(COLOR_INFO);
    cout << message;
    // Overwrite any trailing characters from previous messages to keep the line clean
    if ((int)message.length() < 78) {
        cout << string(78 - (int)message.length(), ' ');
    }
    ResetColor();

    // Move cursor out of footer below the message
    cursorY = sepY + 2;
    MoveToXY(0, cursorY); 
}

void ConsoleView::PrintShortcutFooter(string shortcuts, string status) {
    int sepY = (cursorY < 23) ? 23 : cursorY;

    // Record baseline so errors and footers can be placed predictably
    lastFooterY = sepY; 

    // Fixed-width footer: always 80 columns
    const int sepWidth = 80;
    PrintLine(0, sepY, sepWidth, '-');

    // Left part: shortcuts
    MoveToXY(1, sepY + 1);
    SetColor(COLOR_WARNING);
    cout << shortcuts;

    // Right part: status (right-aligned)
    int statusX = sepWidth - (int)(status.length()) - 1;
    if (statusX < (int)(shortcuts.length()) + 2) {
        statusX = (int)(shortcuts.length()) + 2;
    }
    MoveToXY(statusX, sepY + 1);
    SetColor(COLOR_INFO);
    cout << status; 

    ResetColor();
    cursorY = sepY + 2;
    MoveToXY(0, cursorY);
} 

//MESSAGE IMPLEMENTATIONS

void ConsoleView::ShowSuccess(string message) {
    SetColor(COLOR_SUCCESS);
    std::string out = std::string(" [✓] ") + message;
    cout << out << endl;
    ResetColor();
    // Update cursor tracking since we emitted a newline
    cursorY++;
    if ((int)out.length() > contentWidth) contentWidth = (int)out.length();
}

void ConsoleView::ShowError(string message) {
    SetColor(COLOR_ERROR);
    std::string out = std::string("[✗] ") + message;

    // Place error immediately below the shortcut footer so placement is stable
    const int sepWidth = 80;
    int errY = (lastFooterY >= 23) ? (lastFooterY + 2) : (23 + 2);

    // Print at column 1 to keep a small left margin
    MoveToXY(1, errY);
    cout << out;
    // Clear any leftover characters on the line
    if ((int)out.length() < sepWidth - 2) {
        cout << string((sepWidth - 2) - (int)out.length(), ' ');
    }
    ResetColor();
}

void ConsoleView::ShowWarning(string message) {
    SetColor(COLOR_WARNING);
    std::string out = std::string(" [!] ") + message;
    cout << out << endl;
    ResetColor();
    cursorY++;
    if ((int)out.length() > contentWidth) contentWidth = (int)out.length();
}

void ConsoleView::ShowInfo(string message) {
    SetColor(COLOR_INFO);
    std::string out = std::string(" [i] ") + message;
    cout << out << endl;
    ResetColor();
    cursorY++;
    if ((int)out.length() > contentWidth) contentWidth = (int)out.length();
}

//TABLE IMPLEMENTATIONS

void ConsoleView::PrintTableHeader(string columns[], int colWidths[], int numCols) {
    // Compute table width: 1 (initial '+') + sum(colWidths) + numCols (for separators)
    int tableWidth = 1;
    for (int i = 0; i < numCols; ++i) tableWidth += colWidths[i] + 1;
    if (tableWidth > contentWidth) contentWidth = tableWidth;

    // Print header line at current cursor row
    int y = cursorY;
    MoveToXY(0, y);
    cout << "+";
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < colWidths[i]; j++) cout << "-";
        cout << "+";
    }
    cout << endl;
    cursorY = y + 1; y++;

    MoveToXY(0, y);
    cout << "|";
    for (int i = 0; i < numCols; i++) {
        cout << " " << left << setw(colWidths[i] - 1) << columns[i] << "|";
    }
    cout << endl;
    cursorY = y + 1; y++;

    // Separator after header
    PrintTableSeparator(colWidths,numCols);
}

void ConsoleView::PrintTableRow(const string data[], const int colWidths[], int numCols) {
    // Compute table width same as header
    int tableWidth = 1;
    for (int i = 0; i < numCols; ++i) tableWidth += colWidths[i] + 1;
    if (tableWidth > contentWidth) contentWidth = tableWidth;

    // Print row at current cursor row
    int y = cursorY;
    MoveToXY(0, y);
    cout << "|";
    for (int i = 0; i < numCols; i++) {
        // Left-align all columns and pad to the specified width
        cout << left << setw(colWidths[i]) << data[i] << "|";
    }
    cout << endl;
    cursorY = y + 1;
}

void ConsoleView::PrintTableSeparator(const int colWidths[], int numCols) {
    // Compute table width and track
    int tableWidth = 1;
    for (int i = 0; i < numCols; ++i) tableWidth += colWidths[i] + 1;
    if (tableWidth > contentWidth) contentWidth = tableWidth;

    int y = cursorY;
    MoveToXY(0, y);
    cout << "+";
    for (int i = 0; i < numCols; i++) {
        for (int j = 0; j < colWidths[i]; j++) cout << "-";
        cout << "+";
    }
    cout << endl;
    cursorY = y + 1;
}

void ConsoleView::PrintText(const string &text) {
    cout << text << endl;
    // Track printed width
    if ((int)text.length() > contentWidth) contentWidth = (int)text.length();
    // Advance internal cursor tracking because we emitted a newline
    cursorY++;
}

string ConsoleView::FormatCurrency(long long amount) {
    stringstream ss;
    ss << fixed << setprecision(0);
    string numStr = to_string(amount);
    string result;
    int count = 0;

    for (int i = (int)(numStr.length()) - 1; i >= 0; --i) {
        result = numStr[i] + result;
        count++;
        if (count % 3 == 0 && i > 0) {
            result = "," + result;
        }
    }
    return result + " VND";
} 

