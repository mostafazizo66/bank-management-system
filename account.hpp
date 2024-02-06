#ifndef ACCOUNTS_H_
#define ACCOUNTS_H_
#include <ctime> //library of time & date
#include <iostream>
#include <chrono> // for ::now() to get currentTime
#include <string>
#include <vector>
#include <cmath>
#include <sstream>
#include <iomanip> // for put_time() in string
#include <string.h>
#include <iostream>

struct Date
{
    int month;
    int year;
};
struct account
{
    long long accNumber;
    std::string name;
    std::string email;
    double balance;
    std::string mobile;
    Date date;

    account() = default;
    account (long long accNumber_ ,std::string name_ ,std::string mobile_,std::string email_);
    std::string dateToPrint();
    static std::string getCurrentDate();

};


account loadAccount(std::string acc_from_file);

std::string doubleToString(double value);
long long strToLL(std::string s);
long double strToDouble(std::string s);
std::string doubleToString(double value);
std::string numToString(long long n);

bool charCompare(char c1, char c2);
bool compareByName(account a, account b);
bool compareByDate(account a, account b);
bool compareByBalance(account a, account b);

#endif
