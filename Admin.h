#ifndef ADMIN_H
#define ADMIN_H
//including header files
#include <iostream>
#include <string>
#include <mysql.h>
#include "DatabaseManager.h"
#include "VotingControl.h"
#include "Validation.h"
using namespace std;
//Adding obvious class comments that I know I wont need
class Admin {
	//Thoughts:
    //What attributes should I use
    //Who will win in War
	string username, password;//Most relevant attributes that I could find
public:    
	//Time to decide Menu for Admin
    void menu() {
        int op;
        do {
            cout << "\nAdmin Menu\n1. Start Voting\n2. Stop Voting\n3. Add Candidate\n4. Remove Candidate\n5. View Results\n6. Logout\nChoice: ";
            cin >> op;
            if (op == 1) 
				VotingControl::setVotingStatus(true);//Calling a funtion from VotingControl class Created by my Group Member
            else if (op == 2) 
                VotingControl::setVotingStatus(false);//Calling a funtion from VotingControl class Created by my Group Member
			else if (op == 3) addCandidate();//DONE!!
			else if (op == 4) removeCandidate();//DONE!!
			else if (op == 5) viewResults();//Need to define it also
			else if (op == 4) removeCandidate(); //Need to define it also
			else if (op == 5) viewResults();//Need to define it also
			else if (op == 6) cout << "Logging out...\n";
			else cout << "Invalid choice. Try again.\n";
        } while (op != 6);
	}//INNER THOUGHTS WHY EVEN DEFAULT VALIDATION LIBRARIES EXIST IF WE HAVE TO DEFINE OUR OWN VALIDATION CLASS
    void addCandidate() {
        string name, party, type, city, province, password;
        cout << "Enter Candidate Name: "; cin >> name;
        cout << "Enter Party: "; cin >> party;
        cout << "Enter Type (MNA/MPA): "; cin >> type;
        cout << "Enter City: "; cin >> city;
        cout << "Enter Province: "; cin >> province;
        cout << "Set Candidate Password: "; cin >> password;
		//Validating Inputs
        if (!Validation::isAlphabetic(name) || !Validation::isAlphabetic(party) ||
            !(type == "MNA" || type == "MPA") ||
            !Validation::isAlphabetic(city) || !Validation::isAlphabetic(province) ||
            !Validation::isAlphanumeric(password)) {
            cout << "Invalid input. Please ensure all fields follow format.\n";
            return;
        }
        //Performing DataBase Operations
        string query = "INSERT INTO Candidates (name, party, type, city, province, password) VALUES ('" +
            name + "', '" + party + "', '" + type + "', '" + city + "', '" + province + "', '" + password + "')";
        if (mysql_query(DatabaseManager::conn, query.c_str()) == 0)
            cout << "Candidate added successfully!\n";
        else
            cout << "Error adding candidate: " << mysql_error(DatabaseManager::conn) << endl;
    }
    void removeCandidate() {
        string idInput;
        cout << "Enter Candidate ID to remove: "; cin >> idInput;
		//Validating Input
        if (!Validation::isNumeric(idInput)) {
            cout << "Invalid Candidate ID. Must be numeric.\n";
            return;
		}//Query to remove candidate
        string query = "DELETE FROM Candidates WHERE id=" + idInput;//Reminds me of Phython 
        if (mysql_query(DatabaseManager::conn, query.c_str()) == 0)
            cout << "Candidate removed successfully!\n";
        else
            cout << "Error removing candidate: " << mysql_error(DatabaseManager::conn) << endl;
    }
    void viewResults() {
        cout << "\n--- Chief Ministers ---\n";
        const char* provinces[] = { "Punjab", "Sindh", "KPK", "Balochistan", "Gilgit" };
		// Step 1: Get winning party in each province for MPA
        for (int i = 0; i < 5; ++i) {
            string query = "SELECT party, COUNT(*) as seats "
                "FROM Candidates "
                "WHERE type='MPA' AND province='" + string(provinces[i]) + "' "
                "GROUP BY party "
                "ORDER BY seats DESC, SUM(votes) DESC "
                "LIMIT 1";
            
            if (mysql_query(DatabaseManager::conn, query.c_str()) == 0) {
                MYSQL_RES* res = mysql_store_result(DatabaseManager::conn);
                if (res) {
                    MYSQL_ROW row = mysql_fetch_row(res);
                    if (row)
                        cout << provinces[i] << ": " << row[0] << " (" << row[1] << " seats)\n";
                    else
                        cout << provinces[i] << ": No data available.\n";
                    mysql_free_result(res);
                }
            }
            else {
                cout << "Error querying chief minister for " << provinces[i] << ": " << mysql_error(DatabaseManager::conn) << endl;
            }
        }

        cout << "\n--- Prime Minister ---\n";

        // Step 2: Get winning party in each province for MNA
        string query = "SELECT province, party "
            "FROM (SELECT province, party, COUNT(*) as seats "
            "FROM Candidates WHERE type='MNA' "
            "GROUP BY province, party) as T1 "
            "GROUP BY province "
            "HAVING MAX(seats) "
            "ORDER BY province";

        if (mysql_query(DatabaseManager::conn, query.c_str()) != 0) {
            cout << "Error retrieving PM data: " << mysql_error(DatabaseManager::conn) << endl;
            return;
        }

        MYSQL_RES* res = mysql_store_result(DatabaseManager::conn);
        if (!res) {
            cout << "No data for Prime Minister selection.\n";
            return;
        }
		//Step 1: Get winning party in each province for MNA
        string provinceWins[5];
        int winCount = 0;
        MYSQL_ROW row;
        while ((row = mysql_fetch_row(res)) && winCount < 5) {
            if (row[1])
                provinceWins[winCount++] = row[1];
        }
        mysql_free_result(res);

        // Step 2: Count majority party
        string majorityParty = "";
        int maxCount = 0;
        for (int i = 0; i < winCount; ++i) {
            int count = 0;
            for (int j = 0; j < winCount; ++j) {
                if (provinceWins[i] == provinceWins[j])
                    count++;
            }
            if (count > maxCount) {
                maxCount = count;
                majorityParty = provinceWins[i];
            }
        }

        if (maxCount >= 3)
            cout << "Prime Minister: " << majorityParty << " (" << maxCount << " provinces)\n";
        else
            cout << "No party has majority for Prime Minister.\n";
    }

};

#endif

