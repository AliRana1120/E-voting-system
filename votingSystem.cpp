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
    bool hasVoted;

    bool isVotingOpen() {
        MYSQL* conn = DatabaseManager::getConnection();
        mysql_query(conn, "SELECT status FROM election_status WHERE id=1");
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row = mysql_fetch_row(res);
        bool open = (row && string(row[0]) == "start");
        mysql_free_result(res);
        return open;
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

        string insertQuery = "INSERT INTO voters (cnic, name, province, district, password) VALUES ('" +
            newCnic + "', '" + name + "', '" + province + "', '" + district + "', '" + password + "')";

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
    Voter() : isLoggedIn(false), hasVoted(false) {}

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
                string query = "SELECT name, province, district, is_voted FROM voters WHERE cnic='" + cnic + "' AND password='" + password + "'";

                if (mysql_query(conn, query.c_str()) == 0) {
                    MYSQL_RES* res = mysql_store_result(conn);
                    MYSQL_ROW row = mysql_fetch_row(res);
                    if (row) {
                        name = row[0];
                        province = row[1];
                        district = row[2];
                        hasVoted = (row[3] && string(row[3]) == "1");
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
            cout << "2. Vote for MNA\n";
            cout << "3. Vote for MPA\n";
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
                voteFor("MNA");
                break;
            case 3:
                voteFor("MPA");
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

