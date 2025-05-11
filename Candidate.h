#pragma once
#ifndef CANDIDATE_H
#define CANDIDATE_H

#include <iostream>
#include "Validator.h"
#include "User.h"
#include "DatabaseManager.h"
using namespace std;

class Candidate : public User {
protected:
    string username;
    string password;
    int id;

public:
    virtual void viewDetails() = 0;
    virtual void viewVotes() = 0;

    bool login() override {
        cout << "\nEnter Candidate ID: ";
        if (!Validator::getIntInput(id)) return false;

        cout << "Enter Password: ";
        cin >> password;

        MYSQL* conn = DatabaseManager::getConnection();
        string query = "SELECT name FROM candidates WHERE id=" + to_string(id) +
            " AND password='" + password + "'";

        if (mysql_query(conn, query.c_str()) == 0) {
            MYSQL_RES* res = mysql_store_result(conn);
            if (mysql_num_rows(res) == 1) {
                MYSQL_ROW row = mysql_fetch_row(res);
                username = row[0]; // Set username from database
                cout << "Login successful. Welcome, " << username << "!\n";
                mysql_free_result(res);
                return true;
            }
            mysql_free_result(res);
        }
        cout << "Invalid ID or password. Please try again.\n";
        return false;
    }

    void showMenu() override {
        int ch;
        while (true) {
            cout << "\n--- Candidate Menu ---\n";
            cout << "1. View My Details\n";
            cout << "2. View My Votes\n";
            cout << "3. Logout\n";
            cout << "Enter choice: ";
            if (!Validator::getIntInput(ch)) continue;

            if (ch == 1) viewDetails();
            else if (ch == 2) viewVotes();
            else if (ch == 3) return;
            else cout << "Invalid choice. Try again.\n";
        }
    }
};

#endif