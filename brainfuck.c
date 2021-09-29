#include <stdio.h>
#include <stdlib.h>

void printCells(char* cellSpace, int start, int end) {
  for (int i = start; i < end; i++) {
    printf("[%x] ", cellSpace[i]);
  }
  printf("\n");
}

void processInstruction(char* cellSpace, unsigned int* cellPtr, char* programCode, int* i);

void processLoop(char* cellSpace, unsigned int* cellPtr, char* programCode, int* i) {
  int loopOrigination = (*i)++;

  while (programCode[*i] != ']') {
    processInstruction(cellSpace, cellPtr, programCode, i);
    (*i)++;
  }

  if (cellSpace[*cellPtr] == 0) {
    *i++;
  }
  else {
    *i = loopOrigination - 1;
  }
}

void processInstruction(char* cellSpace, unsigned int* cellPtr, char* programCode, int* i) {
  if (programCode[*i] == '<') *cellPtr-=sizeof(char);
  else if (programCode[*i] == '>') *cellPtr+=sizeof(char);
  else if (programCode[*i] == '-') cellSpace[*cellPtr]--;
  else if (programCode[*i] == '+') cellSpace[*cellPtr]++;
  else if (programCode[*i] == '.') printf("%c", cellSpace[*cellPtr]);
  else if (programCode[*i] == ',') scanf("%c", &cellSpace[*cellPtr]);
  else processLoop(cellSpace, cellPtr, programCode, i);
}

int main(int argc, char* argv[]) {
  printf("Brainfuck interpreter. Cell size is 8 bits. 30k cells available.\n");
  printf("Type 'exit' to run your program.\n");
  printt("Provide input to your program after typing 'exit'.\n");

  char finishedReading = 0;
  char* programCode = malloc(0);
  char* cellSpace = malloc(sizeof(char)*30000);

  unsigned int currentCell = 0;
  unsigned int programLength = 0;

  while (finishedReading != 15) {
    char nextChar;
    scanf("%c", &nextChar);

    // handle user typing 'exit'
    if (nextChar == 'e') finishedReading = finishedReading | 8;
    else if (nextChar == 'x' && finishedReading == 8) finishedReading = finishedReading | 4;
    else if (nextChar == 'i' && finishedReading == 12) finishedReading = finishedReading | 2;
    else if (nextChar == 't' && finishedReading == 14) finishedReading = finishedReading | 1;
    else {
      finishedReading = 0;

      if (nextChar == 43 || nextChar == 44 || nextChar == 45 || nextChar == 46 || nextChar == 60 || nextChar == 62 || nextChar == 91 || nextChar == 93) {
        // Add to program code
        programCode = realloc(programCode, sizeof(char) * ++programLength);
        programCode[programLength - 1] = nextChar;
      }
    }
  }

  // Burn off the newline character entered after 'exit'
  scanf("%c", &finishedReading);

  // Run the program
  for (int i = 0; i < programLength; i++) {
    processInstruction(cellSpace, &currentCell, programCode, &i);
  }

  printCells(cellSpace, 0, 5);
  printf("Pointer at: %d\n", currentCell);

  // Clean up
  free(programCode);
  free(cellSpace);
  return 0;
}
