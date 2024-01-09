#include <stdio.h>

int ColorMap(float minVal, float maxVal, int maxValColor, float val) {
  // YOUR CODE STARTS HERE
  const int kColorBase = 0xff;
  double ratio = (val - minVal) / (maxVal - minVal);
  int ans = 0;
  for (int i = 0; i < 3; i++) {
    int colorVal = (maxValColor >> (2 - i) * 8) & kColorBase;
    double mappedVal = ratio * colorVal;
    ans = (ans << 8) | (int)mappedVal;
  }
  return ans;
  // YOUR CODE ENDS HERE
}

int main() {
  float minVal, maxVal, val;
  int maxValColor;
  scanf("%f %f %x %f", &minVal, &maxVal, &maxValColor, &val);
  int color = ColorMap(minVal, maxVal, maxValColor, val);
  printf("0x%x\n", color);
  return 0;
}
