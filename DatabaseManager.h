#pragma once
#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <mysql.h>
#include <iostream>
using namespace std;

class DatabaseManager {
public:
    static MYSQL* conn;
    static void connect() {
        conn = mysql_init(nullptr);
        if (!conn) {
            cout << "MySQL initialization failed!\n";
            exit(1);
        }

        conn = mysql_real_connect(conn, "localhost", "root", "umair@sql", "votingsystem", 3306, nullptr, 0);
        if (!conn) {
            cout << "Connection to database failed: " << mysql_error(conn) << "\n";
            exit(1);
        }
        cout << "Database connection successful.\n";
    }

    static MYSQL* getConnection() {
        return conn;
    }

    static void disconnect() {
        if (conn) {
            mysql_close(conn);
            cout << "Database disconnected.\n";
        }
    }
};
#endif