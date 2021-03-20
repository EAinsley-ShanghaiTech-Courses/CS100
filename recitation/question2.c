#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void stringncpy(char *s1, char *s2, int n);

int main() {
  char s1[110], s2[110];
  int n;
  scanf("%s%s%d", s1, s2, &n);
  stringncpy(s1, s2, n);
  puts(s1);
  return 0;
}
void stringncpy(char *s1, char *s2, int n) {
  int length1 = sizeof(s1), length2 = strlen(s2);
  if (length1 < n) {
    printf("Error:s1 is too short!\n");
    exit(1);
  }
  for (int i = 0; i < n; i++) {
    if (i > length2) {
      s1[i] = '\0';
    } else {
      s1[i] = s2[i];
    }
  }
  printf("Done.\n");
}
