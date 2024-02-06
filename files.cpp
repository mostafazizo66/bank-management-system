#include "files.hpp"

int login(std::string username, std::string password)
{
    std::string line;
    std::ifstream useraccount;
    useraccount.open("users.txt", std::ios::in);
    if (useraccount.fail())
    {
        return -1;
    }
    std::string word, storedUsername, storedPassword;
    while (std::getline(useraccount, line))
    {
        std::istringstream iss(line);
        iss >> storedUsername >> storedPassword;
        if (username == storedUsername)
        {
            if (password == storedPassword)
            {
                useraccount.close();
                return 0; // Login
            }
            else
            {
                useraccount.close();
                return 2; // wrong password
            }
        }
    }
    useraccount.close();
    return 1; // not found
}

void load(std::vector<account> &accounts)
{
    std::ifstream clientAccounts;
    clientAccounts.open("accounts.txt", std::ios::in);
    std::string line;
    while (std::getline(clientAccounts, line))
    {
        accounts.push_back(loadAccount(line));
    }

    clientAccounts.close();
}

void saveToFile(std::vector<account> &accounts)
{
    std::ofstream clientAccounts;
    clientAccounts.open("accounts.txt", std::ios::trunc);
    if (!clientAccounts.fail())
    {
        for (const auto &account : accounts)
        {
            clientAccounts << account.accNumber << "," << account.name << "," << account.email << "," << 
            account.balance << "," << account.mobile << "," << account.date.month << "-" << account.date.year << "\n";
        }
    }
    clientAccounts.close();
}

void registerReport(std::string accountNumber, std::string process) {
    std::ofstream file;
    file.open("Reports/"+accountNumber+".txt", std::ios::app);
    if (!file.fail()) {
        file << process << "\n";
    }
}

std::deque<std::string> getReport(std::string accountNumber) {
    std::string line;
    std::deque<std::string> report;
    std::ifstream inputFile;
    inputFile.open("Reports/"+accountNumber+".txt");
    while (std::getline(inputFile, line)) {
        if (report.size() == 5) {
            report.pop_front();
            report.push_back(line);
        }
        else {
            report.push_back(line);
        }
    }
    return report;
}

