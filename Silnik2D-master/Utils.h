#pragma once
#include <fstream>
#include <string>
#include <ctime>
#include <iostream>

using namespace std;

class Utils {
public:
    static void logError(const string& errorMessage) {
        ofstream errorFile("error_log.txt", ios::app);
        if (errorFile.is_open()) {
            time_t now = time(nullptr);
            char timeStr[100];
            ctime_s(timeStr, sizeof(timeStr), &now);
            timeStr[strlen(timeStr) - 1] = '\0';
            errorFile << "[" << timeStr << "] " << errorMessage << endl;
            errorFile.close();
        }
        cerr << errorMessage << endl;
    }

    static void logDebug(const string& debugMessage) {
        ofstream debugFile("debug_log.txt", ios::app);
        if (debugFile.is_open()) {
            time_t now = time(nullptr);
            char timeStr[100];
            ctime_s(timeStr, sizeof(timeStr), &now);
            timeStr[strlen(timeStr) - 1] = '\0';
            debugFile << "[" << timeStr << "] " << debugMessage << endl;
            debugFile.close();
        }
    }
};