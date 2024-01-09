#include "question1_def.h"
#include <stdbool.h>
#include <stdio.h>

void MapHands(char *mahjongHand[], short hands[4][9], short length) {
  for (int i = 0; i < length; i++) {
    switch (mahjongHand[i][1]) {
    case 'p':
      hands[0][mahjongHand[i][0] - '1']++;
      break;
    case 's':
      hands[1][mahjongHand[i][0] - '1']++;
      break;
    case 'm':
      hands[2][mahjongHand[i][0] - '1']++;
      break;
    default:
      hands[3][mahjongHand[i][0] - '1']++;
      break;
    }
  }
}

bool RecursionCheck(short hands[4][9], short left_tile, bool has_pair) {
  if (left_tile == 0)
    return true;
  bool result = false;
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 9; j++) {
      // Pung
      if (hands[i][j] >= 3) {
        hands[i][j] -= 3;
        result = RecursionCheck(hands, left_tile - 3, has_pair);
        hands[i][j] += 3;
        if (result)
          return true;
      }
      // pair
      if (!has_pair && hands[i][j] >= 2) {
        hands[i][j] -= 2;
        result = RecursionCheck(hands, left_tile - 2, !has_pair);
        hands[i][j] += 2;
        if (result)
          return true;
      }
      // chow
      if (i < 3 && j < 7 && hands[i][j] >= 1 && hands[i][j + 1] >= 1 &&
          hands[i][j + 2] >= 1) {
        hands[i][j]--;
        hands[i][j + 1]--;
        hands[i][j + 2]--;
        result = RecursionCheck(hands, left_tile - 3, has_pair);
        hands[i][j]++;
        hands[i][j + 1]++;
        hands[i][j + 2]++;
        if (result)
          return true;
      }
    }
  }
  return false;
}

int CheckWin(char *mahjongHand[]) {
  short hands[4][9] = {0};
  MapHands(mahjongHand, hands, 14);
  return RecursionCheck(hands, 14, false);
}

int CountWaitingTiles(char *currentTiles[]) {
  short hands[4][9] = {0};
  int ans = 0;
  MapHands(currentTiles, hands, 13);
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 9; j++) {
      if (hands[i][j] < 4) {
        hands[i][j]++;
        ans += RecursionCheck(hands, 14, false);
        hands[i][j]--;
      }
    }
  }
  return ans;
}
