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
class MNA : public Candidate {
public:
    void viewDetails() override {
        MYSQL* conn = DatabaseManager::getConnection();
        string query = "SELECT name, party, province, district, age FROM candidates WHERE id=" + to_string(id);

        if (mysql_query(conn, query.c_str()) == 0) {
            MYSQL_RES* res = mysql_store_result(conn);
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row) {
                cout << "\n--- MNA Candidate Details ---\n";
                cout << "Name: " << row[0] << "\n";
                cout << "Party: " << row[1] << "\n";
                cout << "Province: " << row[2] << "\n";
                cout << "District: " << row[3] << "\n";
                cout << "Age: " << row[4] << "\n";
            }
            else {
                cout << "No details found for this candidate.\n";
            }
            mysql_free_result(res);
        }
        else {
            cout << "Error fetching details: " << mysql_error(conn) << "\n";
        }
    }

    void viewVotes() override {
        MYSQL* conn = DatabaseManager::getConnection();
        string query = "SELECT votes FROM candidates WHERE id=" + to_string(id);

        if (mysql_query(conn, query.c_str()) == 0) {
            MYSQL_RES* res = mysql_store_result(conn);
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row) {
                cout << "\nYour current vote count: " << row[0] << "\n";
            }
            else {
                cout << "No vote data found.\n";
            }
            mysql_free_result(res);
        }
        else {
            cout << "Error fetching votes: " << mysql_error(conn) << "\n";
        }
    }
};

class MPA : public Candidate {
public:
    void viewDetails() override {
        MYSQL* conn = DatabaseManager::getConnection();
        string query = "SELECT name, party, province, district, age FROM candidates WHERE id=" + to_string(id);

        if (mysql_query(conn, query.c_str()) == 0) {
            MYSQL_RES* res = mysql_store_result(conn);
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row) {
                cout << "\n--- MPA Candidate Details ---\n";
                cout << "Name: " << row[0] << "\n";
                cout << "Party: " << row[1] << "\n";
                cout << "Province: " << row[2] << "\n";
                cout << "District: " << row[3] << "\n";
                cout << "Age: " << row[4] << "\n";
            }
            else {
                cout << "No details found for this candidate.\n";
            }
            mysql_free_result(res);
        }
        else {
            cout << "Error fetching details: " << mysql_error(conn) << "\n";
        }
    }

    void viewVotes() override {
        MYSQL* conn = DatabaseManager::getConnection();
        string query = "SELECT votes FROM candidates WHERE id=" + to_string(id);

        if (mysql_query(conn, query.c_str()) == 0) {
            MYSQL_RES* res = mysql_store_result(conn);
            MYSQL_ROW row = mysql_fetch_row(res);
            if (row) {
                cout << "\nYour current vote count: " << row[0] << "\n";
            }
            else {
                cout << "No vote data found.\n";
            }
            mysql_free_result(res);
        }
        else {
            cout << "Error fetching votes: " << mysql_error(conn) << "\n";
        }
    }
};
#endif
