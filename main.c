#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define SUDOKU_SIZE 9
#define SECTOR_SIZE 3
#define MAX_VALUE 9

#define SHUFFLE_CYCLES 20

void printSudoku(int **matrix);
int randomValue();
int *randomArray();
void generateSudoku(int **matrix);

int main() {
  // start seed
  srand(time(NULL));

  // allocate memory for sudoku matrix (pointer to pointer to int)
  int **matrix = malloc(SUDOKU_SIZE * sizeof(int *));
  for (int i = 0; i < SUDOKU_SIZE; i++) {
    matrix[i] = malloc(SUDOKU_SIZE * sizeof(int));
  }

  // delete later
  for (int i = 0; i < SUDOKU_SIZE; i++) {
    for (int j = 0; j < SUDOKU_SIZE; j++) {
      matrix[i][j] = 0;
    }
  }

  generateSudoku(matrix);
  printSudoku(matrix);

  free(matrix);
  return 0;
}

void printSudoku(int **matrix) {
  for (int i = 0; i < SUDOKU_SIZE; i++) {
    for (int j = 0; j < SUDOKU_SIZE; j++) {
      printf("%d ", matrix[i][j]);
      if ((j + 1) % SECTOR_SIZE == 0) {
        printf(" ");
      }
    }
    printf("\n");
    if ((i + 1) % SECTOR_SIZE == 0 && i + 1 != SUDOKU_SIZE) {
      printf("\n");
    }
  }
}

int randomValue() {
  return (rand() % MAX_VALUE) + 1;
}

int *randomArray() {
  // initialize ordered array
  int *array = (int *)malloc(MAX_VALUE);
  for (int i = 0; i < MAX_VALUE; i++) {
    array[i] = i + 1;
  }

  // shuffle values
  int index = 0;
  for (int i = 0; i < SHUFFLE_CYCLES; i++) {
    int randomIndex = rand() % MAX_VALUE;
    int temp = array[0];
    array[0] = array[randomIndex];
    array[randomIndex] = temp;
  }

  return array;
}

void generateSudoku(int **matrix) {
  // first fill diagonal sectors with random values
  for (int n = 0; n < SECTOR_SIZE; n++) {
    for (int i = 0 + (n * SECTOR_SIZE); i < (n + 1) * SECTOR_SIZE; i++) {
      for (int j = 0 + (n * SECTOR_SIZE); j < (n + 1) * SECTOR_SIZE; j++) {
        matrix[i][j] = randomValue();
      }
    }
  }
}