#pragma once
#ifndef LOCALELECTION_H
#define LOCALELECTION_H

#include "Election.h"
#include <string>
#include <iostream>
using namespace std;

class LocalElection : public Election {
private:
    string province;
public:
    LocalElection(const string& prov) : province(prov) {}

    void start() override {
        MYSQL* conn = DatabaseManager::getConnection();
        string q = "UPDATE election_status SET status='start', level='district' WHERE province='" + province + "'";
        mysql_query(conn, q.c_str());
        cout << "Local election started for " << province << endl;
    }

    void end() override {
        MYSQL* conn = DatabaseManager::getConnection();
        string q = "UPDATE election_status SET status='stop', level='district' WHERE province='" + province + "'";
        mysql_query(conn, q.c_str());
        cout << "Local election ended for " << province << endl;
    }

    int countWinner() override {
        MYSQL* conn = DatabaseManager::getConnection();
        string q = "SELECT party, COUNT(*) as seats FROM candidates WHERE type='MPA' AND province='" + province +
            "' AND votes=(SELECT MAX(votes) FROM candidates WHERE type='MPA' AND province='" + province + "') GROUP BY party";
        mysql_query(conn, q.c_str());
        MYSQL_RES* res = mysql_store_result(conn);
        MYSQL_ROW row;
        if ((row = mysql_fetch_row(res))) {
            cout << "Chief Minister from party: " << row[0] << endl;
            mysql_free_result(res);
            return 0;
        }
        mysql_free_result(res);
        return -1;
    }
};

#endif
