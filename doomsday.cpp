#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>

using namespace std;

class DateException : public runtime_error {
  string msg;

public:
  DateException() : runtime_error("Error: date does not exist") {}
};

/*  1) Determine Anchor day for century
    2) Calc weekday of the anchor day for the given year
    3) Evaluate day of week for given day using doomsday
*/
int calcAnchorDay(int);
int calcDoomsday(int);
int calcWeekday(int, int, int);
enum Weekday { Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday };

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
        string token;

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
        cin.clear();
        cin.ignore();
      } catch (const exception &e) {
        dateValid = false;
        std::cerr << "Error: invalid date format\n";
        cin.clear();
        cin.ignore();
      }
    }

    string dayOfWeek = "";
    switch (calcWeekday(month, day, year)) {
    case 0:
      dayOfWeek = "Sunday";
      break;
    case 1:
      dayOfWeek = "Monday";
      break;
    case 2:
      dayOfWeek = "Tuesday";
      break;
    case 3:
      dayOfWeek = "Wednesday";
      break;
    case 4:
      dayOfWeek = "Thursday";
      break;
    case 5:
      dayOfWeek = "Friday";
      break;
    case 6:
      dayOfWeek = "Saturday";
    }
    cout << "Day of week: " << dayOfWeek << endl;
    cout << "Go again? y/n ";
    cin >> choice;
    cin.ignore();
  } while (toupper(choice) == 'Y');

  return 0;
}

int calcAnchorDay(int year) {
  int century = year / 100;
  int anchorDay;
  switch (century % 4) {
  case 0:
    anchorDay = Tuesday;
    break;
  case 1:
    anchorDay = Sunday;
    break;
  case 2:
    anchorDay = Friday;
    break;
  case 3:
    anchorDay = Wednesday;
    break;
  }
  return anchorDay;
}

int calcDoomsday(int year) {
  int last2digits = year - ((year / 100) * 100);
  int calc1 = last2digits / 12;
  int calc2 = last2digits % 12;
  int leapyear = calc2 % 4;
  return (calc1 + calc2 + leapyear + calcAnchorDay(year)) % 7;
}

int calcWeekday(int month, int day, int year) {
  int doomsDays[12] = {3, 28, 0, 4, 9, 6, 11, 8, 5, 10, 7, 12};
  return (day + calcDoomsday(year)) % 7;
}
