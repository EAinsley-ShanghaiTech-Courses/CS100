#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void stringncpy(char *s1, char *s2, int n) {
  int len = strlen(s2);
  if (n > strlen(s1)) {
    puts("Undefined Behavior.");
    exit(1);
  }
  if (len <= n) {
    for (int i = 0; i < len; ++i)
      s1[i] = s2[i];
    for (int i = len; i < n; ++i)
      s1[i] = NULL;
  } else
    for (int i = 0; i < n; ++i)
      s1[i] = s2[i];
}
int main() {
  char s1[110], s2[110];
  int n;
  scanf("%s%s%d", s1, s2, &n);
  stringncpy(s1, s2, n);
  puts(s1);
  return 0;
}