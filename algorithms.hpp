#ifndef ALGORITHMIS_HPP_
#define ALGORITHMIS_HPP_

#include <vector>
#include <iostream>
#include "account.hpp"

// Merge two sorted subvectors
void merge(std::vector<account> &v, int left, int mid, int right, bool compare(account a, account b));

void mergeSort(std::vector<account> &v, int left, int right, bool compare(account a, account b));

void sortVector(std::vector<account> &v, bool compare(account a, account b));

// Searching

std::vector<int> searchAccName(std::vector<account> &accounts, std::string substring);

int searchAccNum(std::vector<account> &accounts, long long num);

#endif
