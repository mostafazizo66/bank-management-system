#include "algorithms.hpp"

// Merge two sorted subvectors
void merge(std::vector<account> &v, int left, int mid, int right, bool compare(account a, account b))
{
    // accountemp vector to store the left and right subvectors
    std::vector<account> L(mid - left + 1);
    std::vector<account> R(right - mid);

    // Copy the data from the original vector to the temporary vectors
    for (int i = 0; i < L.size(); i++)
        L[i] = v[left + i];
    for (int j = 0; j < R.size(); j++)
        R[j] = v[mid + 1 + j];
    int i = 0;    // Left subvector
    int j = 0;    // Right subvector
    int k = left; // Start of temp vector

    // Merge the temp vectors to the original vector in sorted order
    while (i < L.size() && j < R.size())
    {
        if (compare(L[i], R[j])) // Use compare function instead of <=
        {
            v[k] = L[i];
            i++;
        }
        else
        {
            v[k] = R[j];
            j++;
        }
        k++;
    }

    // Add remaining elements of the left subvector if there are any
    while (i < L.size())
    {
        v[k] = L[i];
        i++;
        k++;
    }

    // Add remaining elements of the right subvector if there are any
    while (j < R.size())
    {
        v[k] = R[j];
        j++;
        k++;
    }
}

void mergeSort(std::vector<account> &v, int left, int right, bool compare(account a, account b))
{
    if (left >= right) // Base case if there's one element
        return;
    int mid = (left + right) / 2;
    // Sort left and right by recursion
    mergeSort(v, left, mid, compare); // Pass compare function as argument
    mergeSort(v, mid + 1, right, compare); // Pass compare function as argument
    merge(v, left, mid, right, compare); // Merge after sorting and pass compare function as argument
}

void sortVector(std::vector<account> &v, bool compare(account a, account b))
{
    // Sort the copy using merge sort
    mergeSort(v, 0, v.size() - 1, compare);
}


/************************** SEARCHING FUNCaccountIONS **************************/

std::vector<int> searchAccName(std::vector<account> &accounts, std::string substring)
{
    std::vector<int> indices;
    for (int i = 0; i < accounts.size(); ++i)
    {
        const std::string &current = accounts[i].name;
        
        for (int c = 0; c <= current.size() - substring.size() && c < current.length(); c++) // "abcde" is not a substring of "abc"
        {
            bool found = true;
            for(int s = 0; s < substring.size(); s++)
            {   
                if (tolower(current[c + s]) != tolower(substring[s]))
                {
                    found = false;
                    break;
                }

            }
            if (found)
            {
                indices.push_back(i);
                break;
            }
        }
    }

    return indices;
}

int searchAccNum(std::vector<account> &accounts, long long num)
{
    for (int i = 0; i < accounts.size(); i++)
        if (accounts[i].accNumber == num)
            return i; // Found and return index
    return -1;        // Not found
}

