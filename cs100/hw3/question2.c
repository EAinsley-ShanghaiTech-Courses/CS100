#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int kMaxLine = 20;
const int kRegNumber = 5;
enum eOperator { LET = 0, ADD, SUB, MUL, DIV, BGE, PRINT };
struct CodeLine {
  enum eOperator operation;
  int value[3];
};

struct CodeLine *CodePaser(struct CodeLine *parsecode, const char *code);
void CodeExe(int lines, const struct CodeLine *codes);
int StrToInt(const char *header, const char *tail);

int main() {
  struct CodeLine gencode[kMaxLine];
  size_t length;
  char *inputs = NULL;
  int lines = 0;
  scanf("%d", &lines);
  while (getchar() != '\n')
    continue;
  for (int i = 0; i < lines; i++) {
    getline(&inputs, &length, stdin);
    CodePaser(gencode + i, inputs);
  }
  free(inputs);
  CodeExe(lines, gencode);
  return 0;
}
struct CodeLine *CodePaser(struct CodeLine *parsecode, const char *code) {
  const int kKeywordLength = 5;
  const char *header_position = code, *tail_position = NULL;
  int valueslot = 0;
  while (((tail_position = strchr(header_position, ' ')) != NULL) ||
         ((tail_position = strchr(header_position, '\n')) != NULL)) {
    switch (*header_position) {
    case 'l':
      parsecode->operation = LET;
      break;
    case 'a':
      parsecode->operation = ADD;
      break;
    case 's':
      parsecode->operation = SUB;
      break;
    case 'm':
      parsecode->operation = MUL;
      break;
    case 'd':
      parsecode->operation = DIV;
      break;
    case 'b':
      parsecode->operation = BGE;
      break;
    case 'p':
      parsecode->operation = PRINT;
      break;
    case 'x':
      parsecode->value[valueslot++] = *(header_position + 1) - '1';
      break;
    default:
      parsecode->value[valueslot++] = StrToInt(header_position, tail_position);
      break;
    }
    header_position = tail_position + 1;
  }
  return parsecode;
}

void CodeExe(int lines, const struct CodeLine *codes) {
  int currentline = 0;
  int regesters[kRegNumber];
  memset(regesters, 0, sizeof(regesters));
  while (currentline < lines) {
    const struct CodeLine *codeunit = &codes[currentline];
    switch (codeunit->operation) {
    case LET:
      regesters[codeunit->value[0]] = codeunit->value[1];
      break;
    case ADD:
      regesters[codeunit->value[0]] =
          regesters[codeunit->value[1]] + regesters[codeunit->value[2]];
      break;
    case SUB:
      regesters[codeunit->value[0]] =
          regesters[codeunit->value[1]] - regesters[codeunit->value[2]];
      break;
    case MUL:
      regesters[codeunit->value[0]] =
          regesters[codeunit->value[1]] * regesters[codeunit->value[2]];
      break;
    case DIV:
      regesters[codeunit->value[0]] =
          regesters[codeunit->value[1]] / regesters[codeunit->value[2]];
      break;
    case BGE:
      if (regesters[codeunit->value[0]] >= regesters[codeunit->value[1]]) {
        currentline =
            codeunit->value[2] - 2; /* code line starts at 1, but the index of
                                       the array starts at 0 */
      }
      break;
    case PRINT:
      printf("x%d = %d\n", codeunit->value[0] + 1,
             regesters[codeunit->value[0]]);
      break;
    }
    currentline++;
  }
}

int StrToInt(const char *header, const char *tail) {
  int value = 0;
  while (header < tail) {
    value = value * 10 + *(header++) - '0';
  }
  return value;
}
