#ifndef FILES_H_
#define FILES_H_

#include <string>
#include <vector>
#include <deque>
#include <fstream>
#include <sstream>
#include "account.hpp"

int login(std::string username, std::string password);

void load(std::vector<account> &accounts);

void saveToFile(std::vector<account> &accounts);

void registerReport(std::string accountNumber, std::string process);

std::deque<std::string> getReport(std::string accountNumber);

#endif