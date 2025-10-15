#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

class DateException : public runtime_error {
  string msg;

public:
  DateException() : runtime_error("Error: date does not exist.") {}
};

/*  1) Determine Anchor day for century
    2) Calc weekday of the anchor day for the given year
    3) Evaluate day of week for given day using doomsday
*/

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
  switch (w) {
  case Weekday::Sunday:
    return "Sunday";
  case Weekday::Monday:
    return "Monday";
  case Weekday::Tuesday:
    return "Tuesday";
  case Weekday::Wednesday:
    return "Wednesday";
  case Weekday::Thursday:
    return "Thursday";
  case Weekday::Friday:
    return "Friday";
  case Weekday::Saturday:
    return "Saturday";
  }
}

Weekday calcAnchorDay(int);
int calcDoomsday(int);
Weekday calcWeekday(int, int, int);

int main() {
  string input;
  int month, day, year;
  char choice;

  // main loop
  do {
    input = {};
    choice = {};
    month = 0;
    day = 0;
    year = 0;
    bool dateValid = false;

    // input validation loop
    while (!dateValid) {
      dateValid = true;

      cout << "Enter a Date (mm/dd/yyyy): ";

      try {
        getline(cin, input);
        istringstream iss(input);
        string token{};

        // Month
        getline(iss, token, '/');
        int month = stoi(token);
        if (month > 12 || month < 1) {
          throw DateException();
        }

        // Day
        getline(iss, token, '/');
        int day = stoi(token);
        if (day > 31 || day < 1) {
          throw DateException();
        }

        // Year
        getline(iss, token);
        int year = stoi(token);
        if (year < 0) {
          throw DateException();
        }

      } catch (const DateException &e) {
        dateValid = false;
        std::cerr << e.what() << "\n";
      } catch (const exception &e) {
        dateValid = false;
        std::cerr << "Error: could not read date. Try again.\n";
      }
    }

    string weekdayStr = "";
    cout << "Day of week: " << to_string(calcWeekday(month, day, year)) << "\n";
    cout << "Go again? y/n ";
    cin >> choice;
    cin.ignore();
  } while (toupper(choice) == 'Y');

  return 0;
}

Weekday calcAnchorDay(int year) {
  int century = year / 100;
  Weekday anchorDay;
  switch (century % 4) {
  case 0:
    anchorDay = Weekday::Tuesday;
    break;
  case 1:
    anchorDay = Weekday::Sunday;
    break;
  case 2:
    anchorDay = Weekday::Friday;
    break;
  case 3:
    anchorDay = Weekday::Wednesday;
    break;
  }
  return anchorDay;
}

int calcDoomsday(int year) {
  int last2digits = year - ((year / 100) * 100);
  int calc1 = last2digits / 12;
  int calc2 = last2digits % 12;
  int leapyear = calc2 % 4;
  return (calc1 + calc2 + leapyear + (int)calcAnchorDay(year)) % 7;
}

Weekday calcWeekday(int month, int day, int year) {
  constexpr int doomsDays[12] = {3, 28, 0, 4, 9, 6, 11, 8, 5, 10, 7, 12};
  return Weekday((day + calcDoomsday(year)) % 7);
}
