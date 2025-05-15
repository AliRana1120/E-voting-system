#pragma once
#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include "Validator.h"
#include "User.h"
#include "DatabaseManager.h"
#include "LocalElection.h"
#include "NationalElection.h"
using namespace std;

class Admin : public User {
private:
    string username = "admin";
    string password = "admin123";
    void startVoting() {
        int level;
        cout << "\nSelect Election Type to Start:\n";
        cout << "1. Local Election (MPA)\n";
        cout << "2. National Election (MNA)\n";
        cout << "Enter choice: ";

        if (!Validator::getIntInput(level)) return;

        if (level == 1) {
            string province;
            cout << "Enter Province name: ";
            cin >> province;
            LocalElection le(province);
            le.start();
        }
        else if (level == 2) {
            NationalElection ne;
            ne.start();
        }
        else {
            cout << "Invalid choice.\n";
        }
    }

    void stopVoting() {
        int level;
        cout << "\nSelect Election to Stop:\n";
        cout << "1. District (Local) Election\n";
        cout << "2. Provincial (National) Election\nChoice: ";
        if (!Validator::getIntInput(level)) return;
        if (level == 1) {
            string province;
            cout << "Enter Province name: "; cin >> province;
            LocalElection le(province);
            le.end();
        }
        else if (level == 2) {
            NationalElection ne;
            ne.end();
        }
        else {
            cout << "Invalid election level.\n";
        }
    }

public:
    bool login() override {
        string usr, pwd;
        cout << "\nEnter Admin Username: "; cin >> usr;
        cout << "Enter Admin Password: "; cin >> pwd;
        if (usr == username && pwd == password) {
            cout << "\nLogin successful!\n";
            return true;
        }
        cout << "\nInvalid admin credentials.\n";
        return false;
    }

    void showMenu() override {
        int ch;
        while (true) {
            cout << "\n--- Admin Menu ---\n";
            cout << "1. Start Voting\n";
            cout << "2. Stop Voting\n";
            cout << "3. Add Candidate\n";
            cout << "4. Remove Candidate\n";
            cout << "5. View Results\n";
            cout << "6. Logout\n";
            cout << "Enter choice: ";
            if (!Validator::getIntInput(ch)) continue;

            switch (ch) {
            case 1: startVoting(); break;
            case 2: stopVoting(); break;
            case 3: addCandidate(); break;
            case 4: removeCandidate(); break;
            case 5: viewResults(); break;
            case 6: return;
            default: cout << "Invalid choice. Try again.\n";
            }
        }
    }

    void addCandidate() {
        string name, party, type, province, district;
        int age;
        cout << "\nEnter Candidate Name: "; cin >> name;
        cout << "Enter Party: "; cin >> party;
        cout << "Enter Candidate Type (MNA/MPA): "; cin >> type;
        cout << "Enter Province: "; cin >> province;
        cout << "Enter District: "; cin >> district;
        cout << "Enter Age: "; if (!Validator::getIntInput(age)) return;

        MYSQL* conn = DatabaseManager::getConnection();
        string query = "INSERT INTO candidates (name, party, type, province, district, age, votes) VALUES ('" +
            name + "', '" + party + "', '" + type + "', '" + province + "', '" + district + "', '" + to_string(age) + "', 0)";
        if (mysql_query(conn, query.c_str()) == 0)
            cout << "Candidate added successfully.\n";
        else
            cout << "Error adding candidate: " << mysql_error(conn) << "\n";
    }

    void removeCandidate() {
        int id;
        cout << "Enter Candidate ID to remove: "; if (!Validator::getIntInput(id)) return;
        MYSQL* conn = DatabaseManager::getConnection();
        string query = "DELETE FROM candidates WHERE id=" + to_string(id);
        if (mysql_query(conn, query.c_str()) == 0)
            cout << "Candidate removed successfully.\n";
        else
            cout << "Error removing candidate: " << mysql_error(conn) << "\n";
    }

    void viewResults() {
        int level;
        cout << "\nSelect Results to View:\n";
        cout << "1. District (Local) Results\n";
        cout << "2. Provincial (National) Results\nChoice: ";
        if (!Validator::getIntInput(level)) return;
        if (level == 1) {
            string province;
            cout << "Enter Province name: "; cin >> province;
            MYSQL* conn = DatabaseManager::getConnection();
            string q = "SELECT id, name, party, votes FROM candidates WHERE type='MPA' AND province='" + province + "' ORDER BY votes DESC";
            mysql_query(conn, q.c_str());
            MYSQL_RES* res = mysql_store_result(conn);
            MYSQL_ROW row;
            cout << "\n--- Local Results for " << province << " ---\n";
            while ((row = mysql_fetch_row(res))) {
                cout << "ID: " << row[0] << " | Name: " << row[1] << " | Party: " << row[2] << " | Votes: " << row[3] << "\n";
            }
            mysql_free_result(res);
        }
        else if (level == 2) {
            string province;
            cout << "Enter Province name: "; cin >> province;
            MYSQL* conn = DatabaseManager::getConnection();
            string q = "SELECT id, name, party, votes FROM candidates WHERE type='MNA' AND province='" + province + "' ORDER BY votes DESC";
            mysql_query(conn, q.c_str());
            MYSQL_RES* res = mysql_store_result(conn);
            MYSQL_ROW row;
            cout << "\n--- National Results for " << province << " ---\n";
            while ((row = mysql_fetch_row(res))) {
                cout << "ID: " << row[0] << " | Name: " << row[1] << " | Party: " << row[2] << " | Votes: " << row[3] << "\n";
            }
            mysql_free_result(res);

            NationalElection ne;
            ne.countWinner();
        }
        else {
            cout << "Invalid selection.\n";
        }
    }
};
#endif