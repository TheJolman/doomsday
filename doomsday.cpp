#include <iostream>
#include <sstream>
#include <string>
using namespace std;

/*  1) Determine Anchor day for century
    2) Calc weekday of the anchor day for the given year
    3) Evaluate day of week for given day using doomsday
*/
int calcAnchorDay(int);
int calcDoomsday(int);
int calcWeekday(int, int, int);
void errorMsg();
enum weekDay { Sunday, Monday, Tuesday, Wednesday, Thursday, Friday, Saturday };

int main() {
  string date;
  int month, day, year;
  char choice;
  do {
    cout << "Enter a Date (mm/dd/yyyy)";
    getline(cin, date);
    istringstream iss(date);
    string token;
    getline(iss, token, '/');

    int month = stoi(token);
    if (month > 12 || month < 1) {
      cout << "Invalid date. Exiting program...";
      break;
    }
    getline(iss, token, '/');
    int day = stoi(token);
    if (day > 31 || day < 1) {
      cout << "Invalid date. Exiting program...";
      break;
    }
    getline(iss, token);
    int year = stoi(token);
    if (year < 0) {
      cout << "Invalid date. Exiting program...";
      break;
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
