#include "BitcoinExchange.hpp"

bool Date::operator<(const Date& other) const {
    if (year != other.year) return year < other.year;
    if (month != other.month) return month < other.month;
    return day < other.day;
}

bool Date ::operator==(const Date& other) const {
    return year == other.year && month == other.month && day == other.day;
}

std::ostream& operator<<(std::ostream& os, const Date& date) {
    os << date.year << "-" << (date.month < 10 ? "0" : "") << date.month << "-"
       << (date.day < 10 ? "0" : "") << date.day;
    return os;
}

static bool verif_day(Date date) {
    if (date.month == 2) {
        if (date.year % 4 == 0 && (date.year % 100 != 0 || date.year % 400)) return date.day <= 29;
        else return date.day <= 28;
    }
    if (date.month == 4 || date.month == 6 || date.month == 9 || date.month == 11)
        return date.day <= 30;
    return date.day <= 31;
}

static void parseDate(const std::string& date, Date& parsed_date) {
    std::stringstream ss(date);
    char dash1, dash2;
    if (!(ss >> parsed_date.year >> dash1 >> parsed_date.month >> dash2 >> parsed_date.day))
        throw std::runtime_error("Invalid date format");
    if (ss.fail()) throw std::runtime_error("Invalid date format");
    if (dash1 != '-' || dash2 != '-') throw std::runtime_error("Invalid date format");
    if (parsed_date.year < 2009 || parsed_date.year > 2025 || parsed_date.month < 1 ||
        parsed_date.month > 12 || !verif_day(parsed_date))
        throw std::runtime_error("Invalid date");
}

static float parseValue(const std::string& value, float max) {
    float parsed_value;
    if (value.length() == 0) throw std::runtime_error("Invalid value");
    std::stringstream ss(value);
    ss >> parsed_value;
    if (ss.fail() || !ss.eof()) throw std::runtime_error("Invalid value");
    if (parsed_value < 0) throw std::runtime_error("not a positive value");
    if (max >= 0 && parsed_value > max) throw std::runtime_error("too large a value");
    return parsed_value;
}

std::map<Date, float> read_database(std::ifstream& data) {
    std::map<Date, float> ret;
    std::string line;
    std::getline(data, line);
    if (line != DATA_FIRST)
        print_color("First line invalid for database, should be " DATA_FIRST, std::cerr);
    while (std::getline(data, line)) {
        std::stringstream ss(line);
        std::string date, value;
        std::getline(ss, date, ',');
        std::getline(ss, value);
        Date parsed_date;
        try {
            parseDate(date, parsed_date);
            float parsed_value = parseValue(value, -1);
            ret.insert(std::make_pair(parsed_date, parsed_value));
        } catch (const std::exception& e) {
            print_color("Error in line: " + line + ": " + e.what(), std::cerr);
            continue;
        }
    }
    return ret;
}

static void print_value(
    std::string date,
    const Date& parsed_date,
    float value,
    const std::map<Date, float>& data
) {
    std::map<Date, float>::const_iterator it = data.lower_bound(parsed_date);
    if (it == data.end()) {
        print_color("Error: date not found in database", std::cerr);
        return;
    }
    if (!(parsed_date == it->first) && it != data.begin()) --it;
    get_color();
    std::cout << date << " => " << value << " = " << value * it->second << std::endl;
    std::cout << ANSI_RESET;
}

static std::string trim(const std::string& s) {
    size_t start = s.find_first_not_of(" \t\r\n");
    size_t end = s.find_last_not_of(" \t\r\n");
    if (start == std::string::npos) return "";
    return s.substr(start, end - start + 1);
}

int exchange(std::ifstream& in, std::ifstream& database) {
    std::map<Date, float> data = read_database(database);
    std::string line;
    std::getline(in, line);
    if (line != IN_FIRST)
        print_color("First line invalid for input, should be " IN_FIRST, std::cerr);
    while (std::getline(in, line)) {
        try {
            std::stringstream ss(line);
            size_t pos = line.find(" | ");
            if (pos == std::string::npos) throw std::runtime_error("Invalid format");
            std::string date = trim(line.substr(0, pos));
            std::string value = trim(line.substr(pos + 3));
            Date parsed_date;
            parseDate(date, parsed_date);
            float parsed_value = parseValue(value, 1000);
            print_value(date, parsed_date, parsed_value, data);
        } catch (const std::exception& e) {
            print_color("Error in line: " + line + ": " + e.what(), std::cerr);
            continue;
        }
    }
    return 0;
}
