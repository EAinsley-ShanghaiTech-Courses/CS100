#include <stdio.h>
#include <stdlib.h>

void DrawX(int length, char ch) {
  for (int i = 0; i < length; i++)
    putchar(ch);
}
void DrawSquare(int size) {
  // YOUR CODE STARTS HERE
  for (int i = 0; i < size; i++) {
    putchar('*');
    if (i != 0 && i != size - 1)
      DrawX(size - 2, ' ');
    else {
      DrawX(size - 2, '*');
    }
    printf("*\n");
  }
  return;
  // YOUR CODE ENDS HERE
}
void DrawDiamond(int size) {
  // YOUR CODE STARTS HERE
  for (int i = -size + 1; i < size; i++) {
    DrawX(abs(i), ' ');
    putchar('*');
    if (i != -size + 1 && i != size - 1) {
      DrawX(2 * (size - abs(i) - 1) - 1, ' ');
      putchar('*');
    }
    putchar('\n');
  }
  return;
  // YOUR CODE ENDS HERE
}
void DrawHexagon(int size) {
  // YOUR CODE STARTS HERE
  for (int i = -size + 1; i < size; i++) {
    DrawX(abs(i), ' ');
    putchar('*');
    if (i != -size + 1 && i != size - 1) {
      DrawX(3 * size - 2 * abs(i) - 4, ' ');
      putchar('*');
    } else {
      DrawX(size - 1, '*');
    }
    putchar('\n');
  }
  // YOUR CODE ENDS HERE
}

int main() {
  int shape, size;
  scanf("%d %d", &shape, &size);

  // 1 for square, 2 for diamond, 3 for hexagon.
  switch (shape) {
  case 1:
    DrawSquare(size);
    break;
  case 2:
    DrawDiamond(size);
    break;
  case 3:
    DrawHexagon(size);
    break;
  default:
    return -1;
  }
  return 0;
}
