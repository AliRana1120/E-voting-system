#pragma once
#ifndef VALIDATOR_H
#define VALIDATOR_H

#include <iostream>
#include <string>
using namespace std;

class Validator {
public:
    static bool getIntInput(int& val) {
        string input;
        cin >> input;
        int result = 0;
        bool valid = true;

        for (int i = 0; input[i] != '\0'; i++) {
            if (input[i] < '0' || input[i] > '9') {
                valid = false;
                break;
            }
            result = result * 10 + (input[i] - '0');
        }

        if (!valid) {
            cout << "Invalid input. Only numbers are allowed.\n";
            return false;
        }

        val = result;
        return true;
    }

    static bool isAlpha(const string& s) {
        for (int i = 0; s[i] != '\0'; i++) {
            if (!((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z')))
                return false;
        }
        return true;
    }

    static bool isAlphanumeric(const string& s) {
        for (int i = 0; s[i] != '\0'; i++) {
            if (!((s[i] >= 'A' && s[i] <= 'Z') || (s[i] >= 'a' && s[i] <= 'z') || (s[i] >= '0' && s[i] <= '9')))
                return false;
        }
        return true;
    }

    static bool isNumeric(const string& s) {
        for (int i = 0; s[i] != '\0'; i++) {
            if (!(s[i] >= '0' && s[i] <= '9'))
                return false;
        }
        return true;
    }
};
#endif