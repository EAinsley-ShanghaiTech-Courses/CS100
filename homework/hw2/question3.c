#include <stdio.h>
#include <stdlib.h>

void FindSecondMaxAndMin(int *secondMax, int *secondMin) {
  // YOUR CODE STARTS HERE
  int n = 0, number = 0;
  int maxs[2] = {-1e4 - 5, -1e4}, mins[2] = {1e4 + 5, 1e4};
  scanf("%d", &n);
  for (int i = 0; i < n; i++) {
    scanf("%d", &number);
    if (number > maxs[0]) {
      maxs[1] = maxs[0];
      maxs[0] = number;
    } else if (number > maxs[1]) {
      maxs[1] = number;
    }
    if (number < mins[0]) {
      mins[1] = mins[0];
      mins[0] = number;
    } else if (number < mins[1]) {
      mins[1] = number;
    }
  }
  *secondMax = maxs[1];
  *secondMin = mins[1];
  return;
  // YOUR CODE ENDS HEREx
}

int main() {
  int secondMax, secondMin;
  FindSecondMaxAndMin(&secondMax, &secondMin);
  printf("%d %d\n", secondMax, secondMin);
  return 0;
}
