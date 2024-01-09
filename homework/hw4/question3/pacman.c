#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

typedef enum gameState { losing, onGoing, winning } GameState;
typedef enum direction { up, down, left, right, idle } Direction;
typedef enum gridtype {
  empty = ' ',
  pacman = 'C',
  ghost = '@',
  food = '.',
  wall = '#'
} Gridtype;
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
const int MAX_GHOST = 30;
const int FOOD_SCORE = 10;
const char *Board[7] = {"....#........#....", ".##.#.######.#.##.",
                        ".#..............#.", ".#.....##.##.##.#.",
                        "...........#......", ".#.##.######.##.#.",
                        "....#...C....#...."

};
const Ghost Ghosts[3] = {{2, 4, right}, {3, 6, left}, {3, 5, up}};

Game *NewGame(int rows, int columns);
void EndGame(Game *game);
Direction Getinput();
void PrintGame(const Game *game);
void IninitialGame(Game *game, const char *board[], const Ghost *ghosts,
                   int rows, int columns, int ghostnum);

bool AddWall(Game *game, int r, int c);
bool AddFood(Game *game, int r, int c);
bool AddPacman(Game *game, int r, int c);
bool AddGhost(Game *game, int r, int c, Direction direction);

Direction ReverseDirection(Direction *direction);
Gridtype TryMove(Game *game, Direction direction, int *r, int *c,
                 Gridtype trail);
void MovePacman(Game *game, Direction direction);
void MoveGhosts(Game *game);

int main() {
  char again;
  do {
    Game *game = NewGame(7, 18);
    IninitialGame(game, Board, Ghosts, 7, 18, 3);
    PrintGame(game);
    while (game->state == onGoing) {
      Direction usermoving = Getinput();
      MovePacman(game, usermoving);
      MoveGhosts(game);
      PrintGame(game);
    }
    if (game->state == losing)
      puts("lost!");
    else
      puts("win!");
    puts("Try again? ('y' to restart)");
    EndGame(game);
    again = getchar();
    while (getchar() != '\n')
      continue;
  } while (again == 'y');
  puts("Goodbye!");
  return 0;
}

void PrintGame(const Game *game) {
  system("clear");
  for (int i = 0; i < game->rows; i++) {
    for (int j = 0; j < game->columns; j++)
      putchar(game->grid[i][j]);
    putchar('\n');
  }
  puts("Use w/a/s/d to move, others to stay");
  printf("Your score: %d, food left: %d\n", game->score, game->foodCount);
}

Direction Getinput() {
  char ch;
  Direction di;
  ch = getchar();
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
  while ((getchar()) != '\n')
    continue;
  return di;
}

Game *NewGame(int rows, int columns) {
  Game *newgame = (Game *)malloc(sizeof(Game));
  char **gridrows = (char **)malloc(sizeof(char *) * rows);
  for (int i = 0; i < rows; i++) {
    gridrows[i] = (char *)malloc(sizeof(char) * columns);
    for (int j = 0; j < columns; j++)
      gridrows[i][j] = empty;
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

void IninitialGame(Game *game, const char *board[], const Ghost *ghosts,
                   int rows, int columns, int ghostnum) {
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < columns; j++)
      switch (board[i][j]) {
      case food:
        AddFood(game, i, j);
        break;
      case wall:
        AddWall(game, i, j);
        break;
      case pacman:
        AddPacman(game, i, j);
      default:
        break;
      }
  for (int i = 0; i < ghostnum; i++) {
    AddGhost(game, ghosts[i].r, ghosts[i].c, ghosts[i].direction);
  }
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
      game->grid[r][c] == empty) {
    game->grid[r][c] = wall;
    return true;
  }
  return false;
}

bool AddFood(Game *game, int r, int c) {
  if (r >= 0 && r < game->rows && c >= 0 && c < game->columns &&
      game->grid[r][c] == empty) {
    game->foodCount++;
    game->grid[r][c] = food;
    return true;
  }
  return false;
}

bool AddPacman(Game *game, int r, int c) {
  if (game->pacmanr == -1 && game->pacmanc == -1 && r >= 0 && r < game->rows &&
      c >= 0 && c < game->columns && game->grid[r][c] == empty) {
    game->grid[r][c] = pacman;
    game->pacmanr = r;
    game->pacmanc = c;
    return true;
  }
  return false;
}

void MovePacman(Game *game, Direction direction) {
  if (game->pacmanr == -1 && game->pacmanc == -1)
    return;
  game->score--;
  Gridtype nextgrid =
      TryMove(game, direction, &game->pacmanr, &game->pacmanc, empty);
  if (nextgrid == ghost) {
    game->state = losing;
    game->pacmanc = game->pacmanr = -1;
  } else if (nextgrid == food) {
    game->score += FOOD_SCORE;
    game->foodCount--;
    if (!game->foodCount)
      game->state = winning;
  }
}

bool AddGhost(Game *game, int r, int c, Direction direction) {
  if (game->ghostnum == 30)
    return false;
  if (r < 0 || r >= game->rows || c < 0 || c >= game->columns)
    return false;
  if (game->grid[r][c] != food && game->grid[r][c] != empty)
    return false;
  Ghost *newghost = (Ghost *)malloc(sizeof(Ghost));
  newghost->r = r;
  newghost->c = c;
  newghost->direction = direction;
  newghost->next = NULL;
  newghost->possesfood = (game->grid[r][c] == food);
  game->grid[r][c] = ghost;
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
  while (currentghost) {
    Gridtype trail = currentghost->possesfood ? food : empty;
    Gridtype nextgrid = TryMove(game, currentghost->direction, &currentghost->r,
                                &currentghost->c, trail);
    if (nextgrid == ghost || nextgrid == wall) {
      nextgrid = TryMove(game, ReverseDirection(&currentghost->direction),
                         &currentghost->r, &currentghost->c, trail);
    }
    switch (nextgrid) {
    case pacman:
      game->state = losing;
      break;
    case empty:
      currentghost->possesfood = false;
      break;
    case food:
      currentghost->possesfood = true;
      break;
    case wall:
    case ghost:
      currentghost->direction = ReverseDirection(&currentghost->direction);
      break;
    default:
      break;
    }
    currentghost = currentghost->next;
  }
}

Gridtype TryMove(Game *game, Direction direction, int *r, int *c,
                 Gridtype trail) {
  if (direction == idle)
    return game->grid[*r][*c];
  const int DELTAC[5] = {[left] = -1, [right] = 1};
  const int DELTAR[5] = {[up] = -1, [down] = 1};
  int aimr = *r + DELTAR[direction], aimc = *c + DELTAC[direction];
  Gridtype movetype = game->grid[*r][*c];
  if (aimr >= 0 && aimc >= 0 && aimr < game->rows && aimc < game->columns) {
    Gridtype aimtype = game->grid[aimr][aimc];
    if (aimtype != wall && aimtype != ghost) {
      game->grid[aimr][aimc] = game->grid[*r][*c];
      game->grid[*r][*c] = trail;
      *r = aimr;
      *c = aimc;
    } else if (aimtype == ghost && movetype == pacman) {
      game->grid[*r][*c] = trail;
      *r = *c = -1;
    }
    return aimtype;
  }
  return wall;
}

Direction ReverseDirection(Direction *direction) {
  if (*direction == idle)
    return idle;
  Direction predirection = *direction;
  switch (predirection) {
  case up:
    *direction = down;
    break;
  case down:
    *direction = up;
    break;
  case left:
    *direction = right;
    break;
  case right:
    *direction = left;
    break;
  default:
    break;
  }
  return *direction;
}