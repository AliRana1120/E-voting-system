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
			else if (op == 3) addCandidate();//Need to define it also
			else if (op == 4) removeCandidate();//Need to define it also
			else if (op == 5) viewResults();//Need to define it also
			else if (op == 4) removeCandidate(); //Need to define it also
			else if (op == 5) viewResults();//Need to define it also
			else if (op == 6) cout << "Logging out...\n";
			else cout << "Invalid choice. Try again.\n";
        } while (op != 6);
    }
};

#endif

