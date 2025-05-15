#pragma once
#ifndef ELECTION_H
#define ELECTION_H

#include <mysql.h>
#include <iostream>
#include "DatabaseManager.h"
using namespace std;

class Election {
public:
    virtual void start() = 0;
    virtual void end() = 0;
    virtual int countWinner() = 0; 
    virtual ~Election() {}
};

#endif
