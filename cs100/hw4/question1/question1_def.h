#include <stdbool.h>

int CheckWin(char *mahjongHand[]);
bool RecursionCheck(short hands[4][9], short left_tile, bool has_pair);
int CountWaitingTiles(char *currentTiles[]);
void MapHands(char *mahjongHand[], short hands[4][9], short length);
