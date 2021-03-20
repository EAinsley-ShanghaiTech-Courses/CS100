#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void CollectNumber(int *arr, int n);
void DrawHistogram(int *arr);

int main() {
  int n = 0;
  int numbers[100];
  memset(numbers, 0, sizeof(numbers));
  scanf("%d", &n);
  CollectNumber(numbers, n);
  DrawHistogram(numbers);
}
void CollectNumber(int arr[], int n) {
  for (int i = 0; i < n; i++) {
    int tmp = rand() % 100;
    arr[tmp]++;
  }
}
void DrawHistogram(int *arr) {
  for (int i = 0; i < 10; i++) {
    printf("%-2d - %-2d   |", i * 10, (i + 1) * 10 - 1);
    int total = 0;
    for (int j = i * 10; j < (i + 1) * 10; j++) {
      total += arr[j];
    }
    for (int j = 0; j < total; j++)
      putchar('*');
    putchar('\n');
  }
}
