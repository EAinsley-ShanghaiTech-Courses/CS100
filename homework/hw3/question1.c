#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

enum Day { MON = 1, TUE, WEN, THU, FRI, SAT, SUN };

/*
 * A function to decide wether the plate is valid.
 * If it is valid, then decide which catagory the plate is in.
 * Return:
 *    If the plate is not valid, return 0.
 *    If the plate is a taxi or a new energy car, return 1.
 *    If the plate is a normal plate with even last digit, return 2.
 *    If the plate is a normal plate with odd last digit, return 3.
 */
short CheckPlate(const char *plate);
/*
 * This function is to handle the input.
 * The functions requires the address where the data to be stored.
 */
void init(char *plate, int *date, enum Day *day);

bool Is_IO(char c) { return (c == 'I' || c == 'O'); }
bool Is_TX(char c) { return (c == 'T' || c == 'X'); }
bool Is_DF(char c) { return (c == 'D' || c == 'F'); }
bool IsEven(int i) { return (i % 2 == 0); }
bool IsSameThree(char a, char b, char c) { return (a == b && b == c); }

int main() {
  const int kLength = 11;
  char plate[kLength];
  int date;
  enum Day day;
  for (int i = 0; i < 3; i++) {
    init(plate, &date, &day);
    switch (CheckPlate(plate)) {
    case 0:
      printf("This number is invalid.\n");
      break;
    case 1:
      printf("This car is allowed to drive.\n");
      break;
    case 2:
      if (day < SAT && !IsEven(date))
        printf("This car is not allowed to drive.\n");
      else
        printf("This car is allowed to drive.\n");
      break;
    case 3:
      if (day < SAT && IsEven(date))
        printf("This car is not allowed to drive.\n");
      else
        printf("This car is allowed to drive.\n");
      break;
    }
  }
  return 0;
}

short CheckPlate(const char *plate) {
  enum eClass { INVALID = 0, TAXINEW, EVEN, ODD };
  size_t length = 0;
  int digitnumber = 0, alphanumber = 0, lastdigit = -1;
  for (length = 0; plate[length] != '\0'; length++) {
    if (Is_IO(plate[length]))
      return INVALID;
    if (isalpha(plate[length]))
      alphanumber++;
    else {
      digitnumber++;
      lastdigit = plate[length] - '0';
    }
  }
  if (lastdigit == -1 || alphanumber > 2)
    return INVALID;
  if (length == 5) {
    switch (alphanumber) {
    case 0:
      return IsEven(lastdigit) ? EVEN : ODD;
    case 1:
      if (Is_TX(plate[0]))
        return TAXINEW;
      else if (isalpha(plate[1]) && IsSameThree(plate[2], plate[3], plate[4]) ||
               isalpha(plate[3]))
        return INVALID;
      else
        return IsEven(lastdigit) ? EVEN : ODD;
    case 2:
      for (int i = 0; i < length; i++) {
        for (int j = i + 2; j < length; j++)
          if (!(i == 0 && j == 4) && isalpha(plate[i]) && isalpha(plate[j]))
            return INVALID;
      }
      if (IsSameThree(plate[0], plate[1], plate[2]) ||
          IsSameThree(plate[1], plate[2], plate[3]) ||
          IsSameThree(plate[2], plate[3], plate[4]))
        return INVALID;
      else
        return IsEven(lastdigit) ? EVEN : ODD;
    }
  } else if (length == 6) {

    if (digitnumber != 5)
      return INVALID;
    if (Is_DF(plate[0]) || Is_DF(plate[5]))
      return TAXINEW;
    return INVALID;
  } else {
    return INVALID;
  }
  return -1;
}
void init(char *plate, int *date, enum Day *day) {
  const char kDayName[7][4] = {"Mon", "Tue", "Wed", "Thu", "Fri", "Sat", "Sun"};
  char sdays[4];
  scanf("%s %*d/%d %s", plate, date, sdays);
  for (int i = MON; i <= SUN; i++) {
    if (strcmp(kDayName[i - 1], sdays) == 0) {
      *day = i;
      break;
    }
  }
}
