#include "question1_def.h"
#include <stdio.h>

int main() {
  char *hand[14] = {"1s", "1s", "1s", "2s", "3s", "4s", "5s",
                    "6s", "7s", "8s", "9s", "9s", "9s", "3s"};
  printf("%d\n", CheckWin(hand));
  char *hand1[14] = {"1s", "1s", "1s", "2s", "3s", "4s", "5s",
                     "6s", "7s", "8s", "9s", "9s", "9s", "3z"};
  printf("%d\n", CheckWin(hand1));

  char *hand2[13] = {"3p", "3p", "3p", "7p", "8p", "9p", "2s",
                     "2s", "4s", "5s", "7s", "8s", "9s"};
  printf("%d\n", CountWaitingTiles(hand2));

  return 0;
}