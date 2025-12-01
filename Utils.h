/**
 * @file Utils.h
 * @brief Nag³ówek klasy Utils – narzêdzi pomocniczych.
 * @details
 * - Zawiera metody do logowania b³êdów i debugowania.
 * - Zapisuje komunikaty do plików z dat¹ i godzin¹.
 */

#pragma once
#include <fstream>
#include <string>
#include <ctime>
#include <iostream>

using namespace std;

/**
 * @class Utils
 * @brief Klasa narzêdziowa do logowania.
 */

class Utils {
public:
    /**
 * @brief Loguje b³¹d do pliku i na konsolê.
 * @param errorMessage Komunikat b³êdu.
 */
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

    /**
 * @brief Loguje debug do pliku.
 * @param debugMessage Komunikat debugowania.
 */

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