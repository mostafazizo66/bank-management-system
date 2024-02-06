#include "account.hpp"

account::account (long long accNumber_ ,std::string name_ ,std::string mobile_,std::string email_)
{
    name = name_;
    email = email_;
    mobile = mobile_;
    accNumber = accNumber_;
    balance = 0.0;

    
account::dateToPrint();

    auto now = std::chrono::system_clock::now();
    time_t currentTime = std::chrono::system_clock::to_time_t(now);
    tm* gottime = gmtime(&currentTime);

    date.year = gottime->tm_year + 1900;
    date.month = gottime->tm_mon + 1;
}

std::string account::getCurrentDate() {
    auto now = std::chrono::system_clock::now();
    std::time_t currentTime = std::chrono::system_clock::to_time_t(now);

    std::tm *localTime = std::localtime(&currentTime);

    std::ostringstream formattedTime;
    formattedTime << std::put_time(localTime, "%b %d %H:%M %Y");
    // %b Abbriviation of month (December => Dec)

    return formattedTime.str();
}

static std::vector<std::string> months = {
    "January", "February", "March", "April",
    "May", "June", "July", "August", "September",
    "October", "November", "December"
};

std::string account::dateToPrint()
{
    std::string strm, stotal, stry;
    strm = months[date.month - 1];

    stotal = strm + " " + numToString(date.year);

    return stotal;
}

long long strToLL(std::string s)
{
    int i;
    long long sum = 0;
    for (i = 0; s[i] != '\0'; ++i)
    {
        sum *= 10;
        sum += (s[i] - '0');
    }
    return sum;
}

long double strToDouble(std::string s)
{
    long double result = 0.0;
    long double Factor = 1.0;
    int i = 0;
    if (s[i] == '-')
    {
        i++;
    }


    for (; s[i] != '.' && i < s.length(); ++i)
    {
        result *= 10.0;
        result += static_cast<double>(s[i] - '0');
    }


    if (s[i] == '.')
    {
        i++;
        int numOfDigits = i + 2; // 2 => num of digits after float point
        for (; i < numOfDigits; ++i)
        {
            Factor *= 0.1;
            result += static_cast<double>(s[i] - '0') * Factor;
        }
    }

    if (s[0] == '-')
    {
        result *= -1.0;
    }

    return result;
}

std::string doubleToString(double value) {
    long long intPart  = static_cast<long long>(value);
    long long fracPart = static_cast<long long>((value - intPart) * 100);
    std::string intStr = (intPart == 0) ? "0" : "";
    
    while (intPart > 0) {
        char digit = '0' + intPart % 10;
        intStr = digit + intStr;
        intPart /= 10;
    }

    std::string fracStr;
    
    if (fracPart > 0) {
        int trailingZeroes = 1 - floor(log10(fracPart));
        intStr += '.';
        while(trailingZeroes--) intStr += '0';
        while (fracPart > 0) {
            char digit = '0' + fracPart % 10;
            fracStr = digit + fracStr;
            fracPart /= 10;
        }
    }

    return intStr + fracStr;
}

std::string numToString(long long n)
{
    std::string num;
    while (n)
    {
        char c = '0' + (n % 10);
        num = c + num;
        n /= 10;
    }
    return num;
}

account loadAccount(std::string acc_from_file)
{
    account acc_to_be_return;
    int i = 0, last = 0, comma = 0;

    while (acc_from_file[i] != '\0')
    {
        if (acc_from_file[i] == ',')
        {
            if (comma == 0)
            {
                acc_to_be_return.accNumber = strToLL(acc_from_file.substr(last, i));
                comma += 1;
                last = i;
            }
            else if (comma == 1)
            {
                acc_to_be_return.name = acc_from_file.substr(last + 1, i - last - 1);
                comma += 1;
                last = i;
            }
            else if (comma == 2)
            {
                acc_to_be_return.email = acc_from_file.substr(last + 1, i - last - 1);
                comma += 1;
                last = i;
            }
            else if (comma == 3)
            {
                acc_to_be_return.balance = strToDouble(acc_from_file.substr(last + 1, i - last - 1));
                comma += 1;
                last = i;
            }
            else if (comma == 4)
            {
                acc_to_be_return.mobile = (acc_from_file.substr(last + 1, i - last - 1));
                comma += 1;
                last = i;
                break;
            }
        }
        i += 1;
    }
    int siZe=0,j=i+1;

    while (acc_from_file[j]!='-')
    {
        siZe+=1;
        j+=1;
    }
    acc_to_be_return.date.year=strToLL(acc_from_file.substr(j+1,4));
    acc_to_be_return.date.month=strToLL(acc_from_file.substr(i+1,siZe));
    return acc_to_be_return;
}

bool charCompare(char c1, char c2){
    return tolower(c1) < tolower(c2);
}

bool compareByName(account a, account b){
    const std::string& name1 = a.name;
    const std::string& name2 = b.name;

    auto it1 = name1.begin();
    auto it2 = name2.begin();

    while (it1 != name1.end() && it2 != name2.end()) {
        if (charCompare(*it1, *it2)) {
            return true;
        } else if (charCompare(*it2, *it1)) {
            return false;
        }

        ++it1;
        ++it2;
    }

    // shorter comes first
    return name1.size() < name2.size();
}

bool compareByDate(account a, account b){
    if(a.date.year != b.date.year){
        return a.date.year < b.date.year;
    }
    return a.date.month < b.date.month;
}

bool compareByBalance(account a, account b){
    return a.balance <= b.balance ;
}

