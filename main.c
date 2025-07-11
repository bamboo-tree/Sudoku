#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Custom font color pallette
#define FONT_ERROR "\e[38;2;255;0;0m"
#define FONT_WARNING "\e[38;2;255;255;0m"
#define FONT_1 "\e[38;2;51;168;199m"
#define FONT_2 "\e[38;2;82;227;225m"
#define FONT_3 "\e[38;2;160;228;38m"
#define FONT_4 "\e[38;2;253;241;72m"
#define FONT_5 "\e[38;2;255;171;0m"
#define FONT_6 "\e[38;2;247;121;118m"
#define FONT_7 "\e[38;2;240;80;174m"
#define FONT_8 "\e[38;2;216;131;255m"
#define FONT_9 "\e[38;2;147;54;253m"
#define ANSI_COLOR_RESET "\x1b[0m"

// Sudoku definition - classic 9x9
#define SUDOKU_SIZE 9
#define SECTOR_SIZE 3
#define MAX_VALUE 9
#define SHUFFLE_CYCLES 30

// Log functions
void error(char *message);
void warning(char *message);

// Sudoku functions
void printSudoku(int **matrix);
int randomValue();
int *randomArray();
void generateSudoku(int **matrix);
bool isLegalVertical(int **matrix, int value, int columnIndex);
bool isLegalHorizontal(int **matrix, int value, int rowIndex);
bool isLegalSector(int **matrix, int value, int sector);
bool *possibleValues(int **matrix, int x, int y);

int main() {
  // init random values seed
  srand(time(NULL));

  // allocate memory for sudoku matrix (pointer to pointer to int)
  int **matrix = malloc(SUDOKU_SIZE * sizeof(int *));
  for (int i = 0; i < SUDOKU_SIZE; i++) {
    matrix[i] = malloc(SUDOKU_SIZE * sizeof(int));
  }

  // fill with 0 - delete later
  for (int i = 0; i < SUDOKU_SIZE; i++) {
    for (int j = 0; j < SUDOKU_SIZE; j++) {
      matrix[i][j] = 0;
    }
  }

  generateSudoku(matrix);

  possibleValues(matrix, 3, 0);
  printSudoku(matrix);

  free(matrix);
  return 0;
}

// Quick error log function
void error(char *message) {
  printf(FONT_ERROR "ERROR: %s\n" ANSI_COLOR_RESET, message);
}

// Quick warning log function
void warning(char *message) {
  printf(FONT_WARNING "WARNING: %s\n" ANSI_COLOR_RESET, message);
}

// Print sudoku matrix with styling and colors
void printSudoku(int **matrix) {
  for (int i = 0; i < SUDOKU_SIZE; i++) {
    for (int j = 0; j < SUDOKU_SIZE; j++) {
      int value = matrix[i][j];
      switch (value) {
      case 1:
        printf(FONT_1 "%d " ANSI_COLOR_RESET, value);
        break;
      case 2:
        printf(FONT_2 "%d " ANSI_COLOR_RESET, value);
        break;
      case 3:
        printf(FONT_3 "%d " ANSI_COLOR_RESET, value);
        break;
      case 4:
        printf(FONT_4 "%d " ANSI_COLOR_RESET, value);
        break;
      case 5:
        printf(FONT_5 "%d " ANSI_COLOR_RESET, value);
        break;
      case 6:
        printf(FONT_6 "%d " ANSI_COLOR_RESET, value);
        break;
      case 7:
        printf(FONT_7 "%d " ANSI_COLOR_RESET, value);
        break;
      case 8:
        printf(FONT_8 "%d " ANSI_COLOR_RESET, value);
        break;
      case 9:
        printf(FONT_9 "%d " ANSI_COLOR_RESET, value);
        break;
      default:
        printf("%d ", value);
      }
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

// Generate random value for sudoku
int randomValue() {
  return (rand() % MAX_VALUE) + 1;
}

// Generate shuffled array with sudoku values
int *randomArray() {
  // Initialize ordered array
  int *array = (int *)malloc(MAX_VALUE * sizeof(int));
  for (int i = 0; i < MAX_VALUE; i++) {
    array[i] = i + 1;
  }

  // Shuffle values
  int index = 0;
  for (int i = 0; i < SHUFFLE_CYCLES; i++) {
    int randomIndex = rand() % MAX_VALUE;
    int temp = array[0];
    array[0] = array[randomIndex];
    array[randomIndex] = temp;
  }

  return array;
}

// Determine if move is legal in the row
bool isLegalHorizontal(int **matrix, int value, int rowIndex) {
  if (value < 1 || value > MAX_VALUE) {
    error("Value out of range");
    return false;
  }

  for (int i = 0; i < SUDOKU_SIZE; i++) {
    if (matrix[rowIndex][i] == value) {
      warning("Value found in the row. Move is illegal.");
      return false;
    }
  }

  return true;
}

// Determine if move is legal in the column
bool isLegalVertical(int **matrix, int value, int columnIndex) {
  if (value < 1 || value > MAX_VALUE) {
    error("Value out of range");
    return false;
  }

  for (int i = 0; i < SUDOKU_SIZE; i++) {
    if (matrix[i][columnIndex] == value) {
      warning("Value found in the column. Move is illegal.");
      return false;
    }
  }

  return true;
}

// Determine if move is legal in the sector
bool isLegalSector(int **matrix, int value, int sector) {
  if (value < 1 || value > MAX_VALUE) {
    error("Value out of range");
    return false;
  }
  if (sector < 0 || sector > 9) {
    error("Sector index out of range");
    return false;
  }

  // Coordinates of top left sector corner
  int sx = sector % 3 * SECTOR_SIZE;
  int sy = sector / 3 * SECTOR_SIZE;

  // Loop through the sector
  for (int y = sy; y < sy + SECTOR_SIZE; y++) {
    for (int x = sx; x < sx + SECTOR_SIZE; x++) {
      if (matrix[y][x] == value) {
        warning("Value found in the sector. Move is illegal.");
        return false;
      }
    }
  }

  return true;
}

// Find all legal values in provided coordinates
bool *possibleValues(int **matrix, int x, int y) {
  // Create 9 element array
  bool *array = (bool *)malloc(MAX_VALUE * sizeof(bool));

  // Check each value if it is legal
  for (int i = 0; i < MAX_VALUE; i++) {
    array[i] = isLegalHorizontal(matrix, i + 1, y) && isLegalVertical(matrix, i + 1, x) && isLegalSector(matrix, i + 1, (x / 3) + (y / 3));
  }

  return array;
}

// Generate sudoku
void generateSudoku(int **matrix) {
  // STAGE 1: Fill diagonal sectors with random values
  for (int n = 0; n < SECTOR_SIZE; n++) {
    // Random array
    int *rarr = randomArray();
    // Fill sector with random not repeating values
    for (int i = 0; i < SECTOR_SIZE; i++) {
      for (int j = 0; j < SECTOR_SIZE; j++) {
        matrix[i + (n * SECTOR_SIZE)][j + (n * SECTOR_SIZE)] = rarr[(i * SECTOR_SIZE) + j];
      }
    }
    free(rarr);
  }

  // STAGE 2: Recursively fill remaining values
  int index = 0;
  while (index < (SUDOKU_SIZE * SUDOKU_SIZE)) {
    int x = index / SUDOKU_SIZE;
    int y = index % SUDOKU_SIZE;

    // if value is filled skip it
    if (matrix[x][y] != 0) {
      index++;
      continue;
    }

    // printf("x: %d, y: %d\n", x, y);
    // int *arr = possibleValues(matrix, x, y);

    index++;
  }
}