#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

const char *Board[7] = {"....#........#....", ".##.#.######.#.##.",
                        ".#..............#.", ".#.....##.##.##.#.",
                        "...........#......", ".#.##.######.##.#.",
                        "....#...C....#...."

};

const int MAX_GHOST = 30;
typedef enum gameState { losing, onGoing, winning } GameState;
typedef enum direction { up, down, left, right, idle } Direction;
const int DELTAC[4] = {[left] = -1, [right] = 1};
const int DELTAR[4] = {[up] = -1, [down] = 1};
typedef struct ghost {
  int r, c;
  Direction direction;
  bool possesfood;
  struct ghost *next;
} Ghost;
typedef struct game {
  char **grid;
  int columns, rows;
  int pacmanr, pacmanc;
  int ghostnum;
  Ghost *headghost, *tailghost;
  int foodCount;
  int score;
  GameState state;
} Game;

Game *NewGame(int rows, int columns);
bool AddWall(Game *game, int r, int c);
bool AddFood(Game *game, int r, int c);
bool AddPacman(Game *game, int r, int c);
void MovePacman(Game *game, Direction direction);
void EndGame(Game *game);
bool AddGhost(Game *game, int r, int c, Direction direction);
void MoveGhosts(Game *game);

int main() {
  Game *game = NewGame(7, 18);
  for (int i = 0; i < 7; i++)
    for (int j = 0; j < 18; j++) {
      switch (Board[i][j]) {
      case '.':
        AddFood(game, i, j);
        break;
      case '#':
        AddWall(game, i, j);
        break;
      case 'C':
        AddPacman(game, i, j);
      default:
        break;
      }
    }
  AddGhost(game, 2, 4, right);
  AddGhost(game, 3, 6, left);
  AddGhost(game, 3, 5, up);
  for (int i = 0; i < game->rows; i++) {
    for (int j = 0; j < game->columns; j++)
      putchar(game->grid[i][j]);
    putchar('\n');
  }
  while (game->state == onGoing) {
    char ch;
    Direction di;
    ch = getchar();
    getchar();
    switch (ch) {
    case 'w':
      di = up;
      break;
    case 's':
      di = down;
      break;
    case 'a':
      di = left;
      break;
    case 'd':
      di = right;
      break;
    default:
      di = idle;
      break;
    }
    MovePacman(game, di);
    MoveGhosts(game);
    system("clear");
    for (int i = 0; i < game->rows; i++) {
      for (int j = 0; j < game->columns; j++)
        putchar(game->grid[i][j]);
      putchar('\n');
    }
  }
  if (game->state == losing)
    puts("lost!");
  else
    puts("win!");
  EndGame(game);
  return 0;
}

Game *NewGame(int rows, int columns) {
  Game *newgame = (Game *)malloc(sizeof(Game));
  char **gridrows = (char **)malloc(sizeof(char *) * rows);
  for (int i = 0; i < rows; i++) {
    gridrows[i] = (char *)malloc(sizeof(char) * columns);
    for (int j = 0; j < columns; j++)
      gridrows[i][j] = ' ';
  }
  newgame->grid = gridrows;
  newgame->columns = columns;
  newgame->rows = rows;
  newgame->foodCount = newgame->score = 0;
  newgame->state = onGoing;
  newgame->pacmanr = newgame->pacmanc = -1;
  newgame->headghost = newgame->tailghost = NULL;
  newgame->ghostnum = 0;
  return newgame;
}

void EndGame(Game *game) {
  char **grid = game->grid;
  for (int i = 0; i < game->rows; i++)
    free(grid[i]);
  free(grid);
  grid = NULL;
  while (!game->headghost) {
    Ghost *tempghost = game->headghost;
    game->headghost = tempghost->next;
    free(tempghost);
  }
  game->headghost = game->tailghost = NULL;
  free(game);
}

bool AddWall(Game *game, int r, int c) {
  if (r >= 0 && r < game->rows && c >= 0 && c < game->columns &&
      game->grid[r][c] == ' ') {
    game->grid[r][c] = '#';
    return true;
  }
  return false;
}

bool AddFood(Game *game, int r, int c) {
  if (r >= 0 && r < game->rows && c >= 0 && c < game->columns &&
      game->grid[r][c] == ' ') {
    game->foodCount++;
    game->grid[r][c] = '.';
    return true;
  }
  return false;
}

bool AddPacman(Game *game, int r, int c) {
  if (game->pacmanr == -1 && game->pacmanc == -1 && r >= 0 && r < game->rows &&
      c >= 0 && c < game->columns && game->grid[r][c] == ' ') {
    game->grid[r][c] = 'C';
    game->pacmanr = r;
    game->pacmanc = c;
    return true;
  }
  return false;
}

void MovePacman(Game *game, Direction direction) {
  if (game->pacmanr == -1 && game->pacmanc == -1)
    return;
  int pacmanr = game->pacmanr + DELTAR[direction];
  int pacmanc = game->pacmanc + DELTAC[direction];
  game->score--;
  if (pacmanr < game->rows && pacmanr >= 0 && pacmanc < game->columns &&
      pacmanc >= 0 && game->grid[pacmanr][pacmanc] != '#') {
    game->grid[game->pacmanr][game->pacmanc] = ' ';
    if (game->grid[pacmanr][pacmanc] != '@') {
      if (game->grid[pacmanr][pacmanc] == '.') {
        game->score += 10;
        game->foodCount--;
        if (!game->foodCount)
          game->state = winning;
      }
      game->grid[pacmanr][pacmanc] = 'C';
      game->pacmanr = pacmanr;
      game->pacmanc = pacmanc;
    } else {
      game->state = losing;
      game->pacmanr = game->pacmanc = -1;
    }
  }
}

bool AddGhost(Game *game, int r, int c, Direction direction) {
  if (game->ghostnum == 30)
    return false;
  if (r < 0 || r >= game->rows || c < 0 || c >= game->columns)
    return false;
  if (game->grid[r][c] != '.' && game->grid[r][c] != ' ')
    return false;
  Ghost *newghost = (Ghost *)malloc(sizeof(Ghost));
  newghost->r = r;
  newghost->c = c;
  newghost->direction = direction;
  newghost->next = NULL;
  newghost->possesfood = (game->grid[r][c] == '.');
  game->grid[r][c] = '@';
  if (!game->headghost)
    game->headghost = game->tailghost = newghost;
  else {
    game->tailghost->next = newghost;
    game->tailghost = newghost;
  }
  game->ghostnum++;
  return true;
}

void MoveGhosts(Game *game) {
  Ghost *currentghost = game->headghost;
  bool movedtwice = false;
  while (currentghost) {
    Direction direction = currentghost->direction;
    int ghostr = currentghost->r + DELTAR[direction];
    int ghostc = currentghost->c + DELTAC[direction];
    if (ghostr >= 0 && ghostr < game->rows && ghostc >= 0 &&
        ghostc < game->columns && game->grid[ghostr][ghostc] != '#' &&
        game->grid[ghostr][ghostc] != '@') {
      // recover food.
      if (currentghost->possesfood)
        game->grid[currentghost->r][currentghost->c] = '.';
      else
        game->grid[currentghost->r][currentghost->c] = ' ';
      switch (game->grid[ghostr][ghostc]) {
      case 'C':
        game->state = losing;
        break;
      case ' ':
        currentghost->possesfood = false;
        break;
      case '.':
        currentghost->possesfood = true;
        break;
      default:
        break;
      }
      // move ghost
      game->grid[ghostr][ghostc] = '@';
      currentghost->r = ghostr;
      currentghost->c = ghostc;
    } else {
      Direction prederection = currentghost->direction;
      switch (prederection) {
      case up:
        currentghost->direction = down;
        break;
      case down:
        currentghost->direction = up;
        break;
      case left:
        currentghost->direction = right;
        break;
      case right:
        currentghost->direction = left;
        break;
      default:
        break;
      }
      // try again
      if (!movedtwice) {
        movedtwice = true;
        continue;
      }
    }
    currentghost = currentghost->next;
    movedtwice = false;
  }
}