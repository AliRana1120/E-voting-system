#ifndef VOTER_H
#define VOTER_H

#include <iostream>
#include <string>
#include <mysql.h>
#include "DatabaseManager.h"
#include "VotingControl.h"
#include "Validation.h"
using namespace std;

class Voter {
private:
    string username, password;

    bool login() {
        cout << "Username: "; cin >> username;
        if (!Validation::isAlphanumeric(username)) {
            cout << "Invalid username. Use only letters and numbers.\n";
            return false;
        }
        cout << "Password: "; cin >> password;
        string query = "SELECT * FROM Users WHERE username='" + username + "' AND password='" + password + "' AND role='voter'";
        if (mysql_query(DatabaseManager::conn, query.c_str()) != 0) return false;
        MYSQL_RES* res = mysql_store_result(DatabaseManager::conn);
        bool success = mysql_num_rows(res) > 0;
        mysql_free_result(res);
        return success;
    }

    void vote(int type) {
        string query = "SELECT city, province FROM Users WHERE username='" + username + "'";
        mysql_query(DatabaseManager::conn, query.c_str());
        MYSQL_RES* res = mysql_store_result(DatabaseManager::conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        string city = row[0], province = row[1];
        mysql_free_result(res);

        string cQuery = (type == 1)
            ? "SELECT id, name, party FROM Candidates WHERE type='MNA' AND province='" + province + "'"
            : "SELECT id, name, party FROM Candidates WHERE type='MPA' AND city='" + city + "'";

        mysql_query(DatabaseManager::conn, cQuery.c_str());
        res = mysql_store_result(DatabaseManager::conn);
        int candidateIds[10], count = 0;

        if (mysql_num_rows(res) == 0) {
            cout << "No candidates found.\n";
            mysql_free_result(res);
            return;
        }

        while ((row = mysql_fetch_row(res))) {
            int id = atoi(row[0]);
            candidateIds[count++] = id;
            cout << "ID: " << row[0] << ", Name: " << row[1] << ", Party: " << row[2] << endl;
        }
        mysql_free_result(res);

        string idInput;
        cout << "Enter Candidate ID to vote for: ";
        cin >> idInput;
        if (!Validation::isNumeric(idInput)) {
            cout << "Invalid input. Candidate ID must be numeric.\n";
            return;
        }

        int candidateId = stoi(idInput);
        bool valid = false;
        for (int i = 0; i < count; i++) {
            if (candidateId == candidateIds[i]) {
                valid = true;
                break;
            }
        }
        if (!valid) {
            cout << "Invalid Candidate ID.\n";
            return;
        }

        string checkQuery = (type == 1)
            ? "SELECT * FROM Votes_MNA WHERE username='" + username + "'"
            : "SELECT * FROM Votes_MPA WHERE username='" + username + "'";

        mysql_query(DatabaseManager::conn, checkQuery.c_str());
        res = mysql_store_result(DatabaseManager::conn);
        if (mysql_num_rows(res) > 0) {
            cout << "You have already voted in this category.\n";
            mysql_free_result(res);
            return;
        }
        mysql_free_result(res);
    };