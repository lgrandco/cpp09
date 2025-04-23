#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include "colors.hpp"
#include <fstream>
#include <iostream>
#include <map>
#include <sstream>

#define DATA_FIRST "date,exchange_rate"
#define IN_FIRST "date | value"

struct Date {
    int year;
    int month;
    int day;
    bool operator<(const Date& other) const;
    bool operator==(const Date& other) const;
};

int exchange(std::ifstream& in, std::ifstream& database);

#endif
