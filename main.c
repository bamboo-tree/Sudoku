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

#define SHUFFLE_CYCLES 30
// Sudoku definition - classic 9x9
#define SUDOKU_SIZE 9
#define SECTOR_SIZE 3
#define MAX_VALUE 9

// Log functions
void error(char *message);
void warning(char *message);
// Sudoku functions
void fillDiagonalSectors(int **matrix);
bool fillSudoku(int **matrix, int row, int col);
int *randomArray();
void printSudoku(int **matrix);
// Validation functions
bool isLegalVertical(int **matrix, int value, int col);
bool isLegalHorizontal(int **matrix, int value, int row);
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

  // Stage 1 - randomly fill diagonal sectors with legal values, these sectors don't interact with each other
  fillDiagonalSectors(matrix);
  // Stage 2 - recursively fill remaining cells with legal values
  fillSudoku(matrix, 0, 0);
  // Stage 3 - print generated sudoku matrix :)
  printSudoku(matrix);

  free(matrix);
  return 0;
}

// Quick error log function
void error(char *message) {
  // printf(FONT_ERROR "ERROR: %s\n" ANSI_COLOR_RESET, message);
}

// Quick warning log function
void warning(char *message) {
  // printf(FONT_WARNING "WARNING: %s\n" ANSI_COLOR_RESET, message);
}

// Print sudoku matrix with styling and colors
void printSudoku(int **matrix) {
  for (int row = 0; row < SUDOKU_SIZE; row++) {
    for (int col = 0; col < SUDOKU_SIZE; col++) {
      int value = matrix[row][col];

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
      // Space between sectors
      if ((col + 1) % SECTOR_SIZE == 0) {
        printf(" ");
      }
    }
    printf("\n");
    // Space between sectors
    if ((row + 1) % SECTOR_SIZE == 0 && row + 1 != SUDOKU_SIZE) {
      printf("\n");
    }
  }
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
bool isLegalHorizontal(int **matrix, int value, int row) {
  if (value < 1 || value > MAX_VALUE) {
    error("Value out of range");
    return false;
  }
  if (row < 0 || row > SUDOKU_SIZE) {
    error("Row index out of range");
    return false;
  }

  for (int i = 0; i < SUDOKU_SIZE; i++) {
    if (matrix[row][i] == value) {
      warning("Value found in the row. Move is illegal.");
      return false;
    }
  }

  return true;
}

// Determine if move is legal in the column
bool isLegalVertical(int **matrix, int value, int col) {
  if (value < 1 || value > MAX_VALUE) {
    error("Value out of range");
    return false;
  }
  if (col < 0 || col > SUDOKU_SIZE) {
    error("Column index out of range");
    return false;
  }

  for (int i = 0; i < SUDOKU_SIZE; i++) {
    if (matrix[i][col] == value) {
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
  int col = sector % 3 * SECTOR_SIZE;
  int row = sector / 3 * SECTOR_SIZE;
  // Loop through the sector
  for (int y = row; y < row + SECTOR_SIZE; y++) {
    for (int x = col; x < col + SECTOR_SIZE; x++) {
      if (matrix[y][x] == value) {
        warning("Value found in the sector. Move is illegal.");
        return false;
      }
    }
  }

  return true;
}

// Find all legal values in provided coordinates
bool *possibleValues(int **matrix, int row, int col) {
  // Create 9 element array
  bool *array = (bool *)malloc(MAX_VALUE * sizeof(bool));

  // Check each value if it is legal
  for (int i = 0; i < MAX_VALUE; i++) {
    array[i] = isLegalHorizontal(matrix, i + 1, row) && isLegalVertical(matrix, i + 1, col) && isLegalSector(matrix, i + 1, (row / 3) * 3 + (col / 3));
  }

  return array;
}

// Fill diagonal sectors with random values
void fillDiagonalSectors(int **matrix) {
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
}

bool fillSudoku(int **matrix, int row, int col) {
  // End of sudoku
  if (row == 9) {
    return true;
  }
  // End of row, go to next one
  if (col == 9) {
    return fillSudoku(matrix, row + 1, 0);
  }
  // Skip if cell is already filled
  if (matrix[row][col] != 0) {
    return fillSudoku(matrix, row, col + 1);
  }

  // Generate possible values
  bool *parr = possibleValues(matrix, row, col);
  // Recursively try all possible values
  for (int i = 0; i < MAX_VALUE; i++) {
    if (parr[i]) {
      matrix[row][col] = i + 1;
      if (fillSudoku(matrix, row, col + 1)) {
        return true;
      }
      matrix[row][col] = 0;
    }
  }
  free(parr);

  return false;
}