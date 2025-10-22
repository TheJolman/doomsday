/*
 * Doomsday Algorithm
 * 1) Determine Anchor day for century
 * 2) Calc weekday of the anchor day for the given year
 * 3) Evaluate day of week for given day using known doomsday as jumping-off
 *    point
 */

#include <exception>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

class DateException : public runtime_error {
  string msg;

public:
  DateException()
      : runtime_error("Error: date does not exist or unsupported.") {}
};

using Month = int;
using Day = int;
enum class Weekday {
  Sunday = 0,
  Monday = 1,
  Tuesday = 2,
  Wednesday = 3,
  Thursday = 4,
  Friday = 5,
  Saturday = 6
};

string to_string(Weekday w) {
  vector<string> days = {"Sunday",   "Monday", "Tuesday", "Wednesday",
                         "Thursday", "Friday", "Saturday"};
  return days[(int)w];
}

namespace dday {
/**
 * Anchor days are known and can be memorized, but here we use the formula:
 *   5 * (c mod 4) mod 7 + Tuesday = anchor
 *   where c = floor(year / 100)
 */
Weekday calcAnchorDay(const int year) noexcept {
  int century = year / 100;
  int res = 5 * (century % 4) % 7 + (int)Weekday::Tuesday;
  return Weekday(res);
}

/**
 * Doomsdays are a day of the week that certain dates will always fall on in a
 * given year.
 */
Weekday calcDoomsday(const int year, const Weekday anchor) noexcept {
  int last2digits = year - ((year / 100) * 100);
  int quotient = last2digits / 12;
  int remainder = last2digits % 12;
  int res = (quotient + remainder + (int)anchor) % 7;
  return Weekday(res);
}

bool is_leap_year(int year) noexcept {
  return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

/**
 * Final step - gets the weekday of the input date using the calculated doomsday
 */
Weekday calcWeekday(const int month, const int day, const int year) {
  const unordered_map<Month, Day> referenceDoomsdays{
      {1, is_leap_year(year) ? 3 : 4},
      {2, is_leap_year(year) ? 3 : 4},
      {3, 0}, // last day of Feb + 7
      {4, 4},
      {5, 9},
      {6, 6},
      {7, 11},
      {8, 8},
      {9, 5},
      {10, 10},
      {11, 7},
      {12, 12},
  };

  int offset{};

  if (month == 3) {
    int daysInFeb = is_leap_year(year) ? 29 : 28;
    offset = (day - daysInFeb) % 7;
  } else {
    offset = (day - referenceDoomsdays.at(month)) & 7;
  }

  auto anchor = calcAnchorDay(year);
  auto doomsday = calcDoomsday(year, anchor);
  int res = ((int)doomsday + offset) % 7;
  return Weekday(res);
}

} // namespace dday

std::tuple<int, int, int> parse_date(std::string date_str) {
  int month, day, year = 0;

  istringstream iss(date_str);
  string token{};

  // Month
  getline(iss, token, '/');
  month = std::stoi(token);
  if (month > 12 || month < 1) {
    throw DateException();
  }

  // Day
  getline(iss, token, '/');
  day = std::stoi(token);
  if (day > 31 || day < 1) {
    throw DateException();
  }

  // Year
  getline(iss, token);
  year = std::stoi(token);
  if (year < 1600 || year > 2300) {
    throw DateException();
  }

  return std::make_tuple(month, day, year);
}

int main(int argc, char **argv) {

  if (argc < 2) {
    std::cerr << "Error: argument expected\n"
              << "Usage: " << argv[0] << " [-h|--help] mm/dd/yyyy\n";
    std::exit(1);
  }

  // argparse loop
  for (int i = 0; i < argc; i++) {
    std::string arg = argv[i];

    if (arg == "--help" || arg == "-h") {
      std::cout
          << "Usage: " << argv[0] << " [-h|--help] mm/dd/yyyy\n\n"
          << "This program calculates the day of the week that (nearly) any "
             "date falls on. It uses the Doomsday rule derived by John Conway. "
             "For more information, see the Wikipedia article on the topic: "
             "https://en.wikipedia.org/wiki/Doomsday_rule\n";
      std::exit(0);

    } else {
      try {
        auto [month, day, year] = parse_date(std::string(argv[1]));
        auto res = dday::calcWeekday(month, day, year);
        cout << "Day of week: " << to_string(res) << "\n";

      } catch (const DateException &e) {
        std::cerr << e.what() << "\n";
        std::exit(1);

      } catch (const std::invalid_argument &e) {
        std::cerr << "Error: failed to parse date: " << e.what() << "\n";

      } catch (const std::out_of_range &e) {
        std::cerr << "Error: failed to parse date: " << e.what() << "\n";

      } catch (const exception &e) {
        std::cerr << "Error: an unexpected error occurred: " << e.what()
                  << "\n";
      }
    }
  }

  return 0;
}
