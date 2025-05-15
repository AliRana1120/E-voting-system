#pragma once
#ifndef VOTER_H
#define VOTER_H

#include <iostream>
#include "Validator.h"
#include "DatabaseManager.h"
#include "User.h"
using namespace std;

class Voter : public User {
private:
    string cnic;
    string password;
    string name;
    string province;
    string district;
    bool isLoggedIn;
    bool hasVotedMNA;
    bool hasVotedMPA;

    bool isMNAVotingOpen() {
        MYSQL* conn = DatabaseManager::getConnection();
        string query = "SELECT status FROM election_status WHERE level='national' AND status='start'";

        if (mysql_query(conn, query.c_str())) {
            cout << "Error checking MNA election status: " << mysql_error(conn) << endl;
            return false;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (!res) return false;

        MYSQL_ROW row = mysql_fetch_row(res);
        bool isOpen = (row && string(row[0]) == "start");
        mysql_free_result(res);
        return isOpen;
    }

    bool isMPAVotingOpen() {
        MYSQL* conn = DatabaseManager::getConnection();
        string query = "SELECT status FROM election_status WHERE level='local' AND status='start' AND province='" + province + "'";

        if (mysql_query(conn, query.c_str())) {
            cout << "Error checking MPA election status: " << mysql_error(conn) << endl;
            return false;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (!res) return false;

        MYSQL_ROW row = mysql_fetch_row(res);
        bool isOpen = (row && string(row[0]) == "start");
        mysql_free_result(res);
        return isOpen;
    }

    bool registerVoter() {
        cout << "\n--- Voter Registration ---\n";
        cout << "Enter CNIC (13 digits without dashes): ";
        string newCnic;
        cin >> newCnic;

        if (newCnic.length() != 13 || !Validator::isNumeric(newCnic)) {
            cout << "Invalid CNIC format. Must be 13 digits.\n";
            return false;
        }

        // Check if CNIC already exists
        MYSQL* conn = DatabaseManager::getConnection();
        string checkQuery = "SELECT id FROM voters WHERE cnic='" + newCnic + "'";
        if (mysql_query(conn, checkQuery.c_str()) == 0) {
            MYSQL_RES* res = mysql_store_result(conn);
            if (mysql_num_rows(res) > 0) {
                cout << "This CNIC is already registered.\n";
                mysql_free_result(res);
                return false;
            }
            mysql_free_result(res);
        }

        cout << "Enter your full name: ";
        cin.ignore();
        getline(cin, name);

        cout << "Enter your province: ";
        getline(cin, province);

        cout << "Enter your district: ";
        getline(cin, district);

        cout << "Set your password: ";
        cin >> password;

        string insertQuery = "INSERT INTO voters (cnic, name, province, district, password, has_voted_mna, has_voted_mpa) VALUES ('" +
            newCnic + "', '" + name + "', '" + province + "', '" + district + "', '" + password + "', 0, 0)";

        if (mysql_query(conn, insertQuery.c_str()) == 0) {
            cout << "Registration successful! Please login with your CNIC and password.\n";
            return true;
        }
        else {
            cout << "Registration failed: " << mysql_error(conn) << "\n";
            return false;
        }
    }

public:
    Voter() : isLoggedIn(false), hasVotedMNA(false), hasVotedMPA(false) {}

    bool login() override {
        int choice;
        while (true) {
            cout << "\n--- Voter Portal ---\n";
            cout << "1. Sign In\n";
            cout << "2. Sign Up\n";
            cout << "3. Back to Main Menu\n";
            cout << "Enter choice: ";

            if (!Validator::getIntInput(choice)) {
                cout << "Invalid input. Please enter a number.\n";
                continue;
            }

            switch (choice) {
            case 1: {
                cout << "\n--- Voter Login ---\n";
                cout << "Enter CNIC: ";
                cin >> cnic;
                cout << "Enter Password: ";
                cin >> password;

                MYSQL* conn = DatabaseManager::getConnection();
                string query = "SELECT name, province, district, has_voted_mna, has_voted_mpa FROM voters WHERE cnic='" + cnic + "' AND password='" + password + "'";

                if (mysql_query(conn, query.c_str()) == 0) {
                    MYSQL_RES* res = mysql_store_result(conn);
                    MYSQL_ROW row = mysql_fetch_row(res);
                    if (row) {
                        name = row[0];
                        province = row[1];
                        district = row[2];
                        hasVotedMNA = (row[3] && string(row[3]) == "1");
                        hasVotedMPA = (row[4] && string(row[4]) == "1");
                        isLoggedIn = true;
                        cout << "\nLogin successful. Welcome, " << name << "!\n";
                        mysql_free_result(res);
                        return true;
                    }
                    mysql_free_result(res);
                }
                cout << "\nInvalid CNIC or password.\n";
                break;
            }
            case 2:
                if (registerVoter()) {
                    continue; // Go back to login menu after registration
                }
                break;
            case 3:
                return false; // Back to main menu
            default:
                cout << "Invalid choice. Try again.\n";
            }
        }
    }

    void showMenu() override {
        if (!isLoggedIn) {
            cout << "Please login first.\n";
            return;
        }

        int ch;
        while (true) {
            cout << "\n--- Voter Menu ---\n";
            cout << "1. View Results\n";

            // Only show MNA option if election is open and voter hasn't voted
            if (isMNAVotingOpen() && !hasVotedMNA) {
                cout << "2. Vote for MNA\n";
            }

            // Only show MPA option if election is open and voter hasn't voted
            if (isMPAVotingOpen() && !hasVotedMPA) {
                cout << "3. Vote for MPA\n";
            }

            cout << "4. Logout\n";
            cout << "Enter choice: ";

            if (!Validator::getIntInput(ch)) {
                cout << "Invalid input. Please enter a number.\n";
                continue;
            }

            switch (ch) {
            case 1:
                viewResults();
                break;
            case 2:
                if (isMNAVotingOpen() && !hasVotedMNA) {
                    voteFor("MNA");
                }
                else {
                    cout << "Cannot vote for MNA at this time.\n";
                }
                break;
            case 3:
                if (isMPAVotingOpen() && !hasVotedMPA) {
                    voteFor("MPA");
                }
                else {
                    cout << "Cannot vote for MPA at this time.\n";
                }
                break;
            case 4:
                isLoggedIn = false;
                cout << "Logged out successfully.\n";
                return;
            default:
                cout << "Invalid choice. Try again.\n";
            }
        }
    }

    void viewResults() {
        MYSQL* conn = DatabaseManager::getConnection();
        string query = "SELECT id, name, party, type, votes FROM candidates WHERE province='" +
            province + "' OR district='" + district + "' ORDER BY votes DESC";

        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Error fetching results: " << mysql_error(conn) << endl;
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (!res) {
            cout << "Error storing result\n";
            return;
        }

        cout << "\n--- Election Results for your Region ---\n";
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            cout << "ID: " << row[0]
                << " | Name: " << row[1]
                << " | Party: " << row[2]
                << " | Type: " << row[3]
                << " | Votes: " << row[4] << "\n";
        }
        mysql_free_result(res);
    }

    void voteFor(string type) {
        MYSQL* conn = DatabaseManager::getConnection();

        // Get candidates based on vote type
        string filter = (type == "MNA") ? "province='" + province + "'" : "district='" + district + "'";
        string query = "SELECT id, name, party FROM candidates WHERE type='" + type + "' AND " + filter;

        if (mysql_query(conn, query.c_str()) != 0) {
            cout << "Error fetching candidates: " << mysql_error(conn) << endl;
            return;
        }

        MYSQL_RES* res = mysql_store_result(conn);
        if (!res) {
            cout << "Error storing result\n";
            return;
        }

        // Display candidates
        int ids[100];
        int idx = 0;
        cout << "\n--- " << type << " Candidates ---\n";
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res))) {
            int cid = atoi(row[0]);
            if (idx < 100) ids[idx++] = cid;
            cout << "ID: " << cid << " | Name: " << row[1] << " | Party: " << row[2] << "\n";
        }

        if (idx == 0) {
            cout << "No candidates available for your region.\n";
            mysql_free_result(res);
            return;
        }

        mysql_free_result(res);

        // Get voter's choice
        int voteId;
        cout << "Enter Candidate ID to vote for: ";
        if (!Validator::getIntInput(voteId)) {
            cout << "Invalid candidate ID.\n";
            return;
        }

        // Validate choice
        bool valid = false;
        for (int i = 0; i < idx; ++i) {
            if (voteId == ids[i]) {
                valid = true;
                break;
            }
        }

        if (!valid) {
            cout << "Invalid Candidate ID for your region.\n";
            return;
        }

        // Update candidate votes
        string updateVotes = "UPDATE candidates SET votes = votes + 1 WHERE id=" + to_string(voteId);
        if (mysql_query(conn, updateVotes.c_str()) != 0) {
            cout << "Error updating votes: " << mysql_error(conn) << endl;
            return;
        }

        // Update voter's voting status for the specific type
        string voteColumn = (type == "MNA") ? "has_voted_mna" : "has_voted_mpa";
        string updateVoter = "UPDATE voters SET " + voteColumn + "=1 WHERE cnic='" + cnic + "'";
        if (mysql_query(conn, updateVoter.c_str()) != 0) {
            cout << "Error updating voter status: " << mysql_error(conn) << endl;
            return;
        }

        // Update local state
        if (type == "MNA") {
            hasVotedMNA = true;
        }
        else {
            hasVotedMPA = true;
        }

        cout << "Your vote for " << type << " has been cast. Thank you!\n";
    }
};
#endif
