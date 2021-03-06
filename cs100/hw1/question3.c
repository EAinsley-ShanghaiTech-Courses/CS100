#include <math.h>
#include <stdio.h>

const int E = (1 << 6) - 1;

int main() {
  unsigned long long hexNumber = 0;
  scanf("%llx", &hexNumber);
  double denominator = 1.0 / (1 << 24);
  int fraction = 0;
  double ans = 1.0;
  for (int i = 0; i < 24; i++) {
    ans += (hexNumber & 1) * denominator;
    denominator *= 2.0;
    hexNumber = hexNumber >> 1;
  }
  int exponent = hexNumber & ((1 << 7) - 1);
  ans *= pow(2.0, exponent - E);
  hexNumber >>= 7;
  ans = ((hexNumber & 1) == 1) ? -ans : ans;
  printf("The number converts to %f\n", ans);
  return 0;
}