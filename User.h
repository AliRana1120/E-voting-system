#pragma once
#ifndef USER_H
#define USER_H
class User {
public:
    virtual bool login() = 0;
    virtual void showMenu() = 0;
    virtual ~User() {}
};
#endif
