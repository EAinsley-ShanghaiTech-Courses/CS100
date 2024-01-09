#include <stdio.h>

const int kHourToMin = 60;
const double Eps = 1e-6;

int main() {
  double mood = 100;
  int hour1 = 8, hour2 = 0, hour3 = 0, min1 = 0, min2 = 0, min3 = 0, num = 0,
      timerest = 0;
  printf("How many lectures today?\n");
  scanf("%d", &num);
  for (int i = 0; i < num; ++i) {
    scanf("%d:%d-%d:%d", &hour2, &min2, &hour3, &min3);
    if (hour1 != 8) {
      timerest = (hour2 - hour1) * kHourToMin + min2 - min1;
      mood += timerest * 0.5;
      mood = mood > 100 ? 100 : mood;
    }
    int timelast = (hour3 - hour2) * kHourToMin + min3 - min2;
    if (timelast <= 60) {
      mood -= timelast * 0.4;
    } else {
      mood -= 60 * 0.4 + (timelast - 60) * 0.8;
    }
    if (mood <= Eps) {
      printf("Gezi Wang has been sent to hospital.\n");
      return 0;
    }
    hour1 = hour3;
    min1 = min3;
  }
  timerest = (22 - hour1) * kHourToMin - min1;
  mood += timerest * 0.5;
  mood = mood > 100 ? 100 : mood;
  printf("His mood level is %0.1f at the end of the day.\n", mood);
  return 0;
}