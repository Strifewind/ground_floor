#ifndef VALID_H
#define VALID_H


int readIntInRange(const char* prompt, int minVal, int maxVal);

double readDoubleInRange(const char* prompt, double minVal, double maxVal);

void readCharArray(const char* prompt, char* buffer, int bufferSize, bool requireNonEmpty = true);

void readDate(int& month, int& day, int& year);

void clearInput();

#endif
