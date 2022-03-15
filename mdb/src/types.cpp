#include "extensions/types.h"

using namespace ext;

vector<short> Date::daysInMonth = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

bool Date::checkDateValidity(const int& day, const int& month, const int& year) {
    if (day < 1 || day > 31)
        return false;
    if (month < 1 || month > 12)
        return false;
    if (year < 0 || year > 9999)
        return false;
    if (day > daysInMonth[month])
        return false;
    if (month == 2 && day == 29 && !(year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)))
        return false;
    return true;
}

Date::Date() {
    time_t t = time(0);
    tm* cur = localtime(&t);
    day = cur->tm_year + 1900;
    month = cur->tm_mon + 1;
    year = cur->tm_mday;
}

Date::Date(const string& s) {
    if (s.length() != 10)
        throw "Invalid date string format!\n";
    for (int i = 0;i < 10;++i) {
        if (i == 2 || i == 5)
            continue;
        if (s[i] < '0' || s[i] > '9')
            throw "Invalid date string format!\n";
    }
    string dayStr = s.substr(0, 2),
        monthStr = s.substr(3, 2),
        yearStr = s.substr(6, 4);
    int day = stoi(dayStr),
        month = stoi(monthStr),
        year = stoi(yearStr);
    if (!checkDateValidity(day, month, year))
        throw "Invalid date!\n";
    this->day = day;
    this->month = month;
    this->year = year;
    date = dayStr + "-" + monthStr + "-" + yearStr;
}

Date::Date(const int& day, const int& month, const int& year) {
    if (!checkDateValidity(day, month, year))
        throw "Invalid date!\n";
    this->day = day;
    this->month = month;
    this->year = year;
    date = to_string(day) + "-" + to_string(month) + "-" + to_string(year);
}

int Date::getDay()const {
    return day;
}

int Date::getMonth()const {
    return month;
}

int Date::getYear()const {
    return year;
}

string Date::getDate()const {
    return date;
}

void Date::setDay(const int& day) {
    if (day < 1 || day>31)
        throw "Invalid day!\n";
    if (!checkDateValidity(day, month, year))
        throw "Invalid date!\n";
    this->day = day;
}

void Date::setMonth(const int& month) {
    if (month < 1 || month>12)
        throw "Invalid month!\n";
    if (!checkDateValidity(day, month, year))
        throw "Invalid date!\n";
    this->month = month;
}

void Date::setYear(const int& year) {
    if (year < 1 || year>9999)
        throw "Invalid year!\n";
    if (!checkDateValidity(day, month, year))
        throw "Invalid date!\n";
    this->year = year;
}

void Date::setDate(const string& s) {
    if (s.length() != 10)
        throw "Invalid date string format!\n";
    for (int i = 0;i < 10;++i) {
        if (i == 2 || i == 5)
            continue;
        if (s[i] < '0' || s[i] > '9')
            throw "Invalid date string format!\n";
    }
    string dayStr = s.substr(0, 2),
        monthStr = s.substr(3, 2),
        yearStr = s.substr(6, 4);
    day = stoi(dayStr);
    month = stoi(monthStr);
    year = stoi(yearStr);
    if (!checkDateValidity(day, month, year))
        throw "Invalid date!\n";
    date = dayStr + "-" + monthStr + "-" + yearStr;
}