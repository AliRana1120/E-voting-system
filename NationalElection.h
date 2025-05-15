#pragma once
#ifndef NATIONALELECTION_H
#define NATIONALELECTION_H

#include "Election.h"
#include <iostream>
using namespace std;

class NationalElection : public Election {
public:
    void start() override {
        MYSQL* conn = DatabaseManager::getConnection();
        string query = "UPDATE election_status SET status='start', level='national', province=''";
        if (mysql_query(conn, query.c_str())) {
            cout << "Error starting national election: " << mysql_error(conn) << "\n";
        }
        else {
            cout << "National election started.\n";
        }
    }
    void end() override {
        MYSQL* conn = DatabaseManager::getConnection();
        mysql_query(conn, "UPDATE election_status SET status='stop', level='provincial'");
        cout << "National election ended.\n";
    }

    int countWinner() override {
        MYSQL* conn = DatabaseManager::getConnection();
        mysql_query(conn, "SELECT province, party FROM (SELECT province, party, RANK() OVER (PARTITION BY province ORDER BY votes DESC) as rnk FROM candidates WHERE type='MNA') ranked WHERE rnk=1");
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;
        int countA = 0, countB = 0;

        cout << "\n--- Prime Minister Selection ---\n";
        while ((row = mysql_fetch_row(res))) {
            cout << "Province: " << row[0] << " won by " << row[1] << "\n";
            if (row[1] == "PartyA") countA++;
            else if (row[1] == "PartyB") countB++;
        }
        mysql_free_result(res);
        if (countA > countB) {
            cout << "Prime Minister from PartyA\n";
            return 0;
        }
        else if (countB > countA) {
            cout << "Prime Minister from PartyB\n";
            return 1;
        }
        cout << "Tie detected. Manual intervention required.\n";
        return -1;
    }
};

#endif
