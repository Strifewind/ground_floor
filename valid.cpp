#include "Valid.h"

#include <iostream>
#include <limits>
#include <cctype>
#include <cstring>

using namespace std;


/**
 *
 * Clears the input stream after reading numeric input.
 */
void clearInput() {

    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}


/**
 * Determines whether a year is a leap year.
 *
 * @param y [int] The year to check.
 *
 * @return [bool] True if leap year
 */
static bool isLeap(int y) {

    return (y % 4 == 0 && y % 100 != 0) || (y % 400 == 0);
}

/**
 * Determines how many days in a month.
 *
 * @param y [int] The year to check.
 *
 * @return [bool] True if leap year
 */
static int daysInMonth(int m, int y) {

    switch (m) {

        case 1: return 31;
        case 2: return isLeap(y) ? 29 : 28;
        case 3: return 31;
        case 4: return 30;
        case 5: return 31;
        case 6: return 30;
        case 7: return 31;
        case 8: return 31;
        case 9: return 30;
        case 10: return 31;
        case 11: return 30;
        case 12: return 31;
        default: return 0;

    }
}


/**
 * Reads an interger within a given inclusive range.
 *
 * @param prompt [const char*] Message to display.
 * @param minVal [int] Minimum allowed value.
 * @param maxVAl [int] Maximum allowed value.
 *
 * @return [int] A validated integer.
 */
int readIntInRange(const char* prompt, int minVal, int maxVal) {

    int value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value >= minVal && value <= maxVal) {
                clearInput();
                return value;
            } else {
                cout << "Please enter a value between " << minVal << " and " << maxVal << ".\n";
            }
        } else {
            cout << "Invalid input. Please enter an Integer. \n";
            clearInput();
        }
    }
}


/**
 * Reads a double within a given inclusive range.
 *
 * @param prompt [const char*] Message to display.
 * @param minVal [double] Minimum allowed value.
 * @param maxVal [double] Maximum allowed value.
 *
 * @return [double] A valid double.
 */
double readDoubleInRange(const char* prompt, double minVal, double maxVal) {

    double value;
    while (true) {
        cout << prompt;
        if (cin >> value) {
            if (value >= minVal && value <= maxVal) {
                clearInput();
                return value;
            } else {
                cout << "Please enter a value between " << minVal << " and " << maxVal << ".\n";
            }
        } else {
            cout << "Invalid input. Please enter a number.\n";
            clearInput();
        }
    }
}


/**
 * Reads a line of text from standard input into buffer.
 * Optionally requires non-empty input.
 *
 *
 * @param prompt [const char*] Message to show the user.
 * @param buffer [char*] Output buffer.
 * @param bufferSize [int] Max size of buffer.
 * @param requireNonEmpty [bool] If true, reprompt until non-empty.
 */
void readCharArray(const char* prompt, char* buffer, int bufferSize, bool requireNonEmpty) {

    while (true) {
        cout << prompt;
        cin.getline(buffer, bufferSize);

        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
        }

        int len = strlen(buffer);
        while (len > 0 && isspace(static_cast<unsigned char>(buffer[len - 1]))) {
            buffer[--len] = '\0';
        }
        if (!requireNonEmpty || (len > 0)) {
            return;
        }
        cout << "Input cannot be empty. Please try again.\n";
    }
}


/**
 * Reads a valid date (month/day/year) from the user.
 *
 * @param month [int&] Output month (1 - 12).
 * @param day [int&] Output day (1 - 31 depending on month/year).
 * @param year [int&] Output year.
 */
void readDate(int& month, int& day, int& year) {

    while (true) {
        month = readIntInRange("Enter month (1-12): ", 1, 12);
        year = readIntInRange("Enter year (1900-2100): ", 1900, 2100);
        int maxDay = daysInMonth(month, year);
        day = readIntInRange("Enter day: ", 1, maxDay);
        return;
    }
}
