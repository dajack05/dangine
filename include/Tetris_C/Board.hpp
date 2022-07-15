#pragma once

#define BOARD_WIDTH 10
#define BOARD_HEIGHT 20

struct Board {
  int cells[BOARD_WIDTH * BOARD_HEIGHT];

  inline Board() {
    for (int i = 0; i < BOARD_HEIGHT * BOARD_WIDTH; i++) {
      cells[i] = -1;
    }
  }

  static constexpr std::pair<int, int> size = {BOARD_WIDTH, BOARD_HEIGHT};

  inline int at(int x, int y) {
    if (x < 0 || x > BOARD_WIDTH - 1 || y < 0 || y > BOARD_HEIGHT - 1) {
      return -1;
    }
    return cells[x + y * BOARD_WIDTH];
  }

  inline void set(int x, int y, int value) {
    cells[x + y * BOARD_WIDTH] = value;
  }

  inline void checkLines() {
    for (int y = 0; y < BOARD_HEIGHT; y++) {

      bool lineIsFilled = true;
      for (int x = 0; x < BOARD_WIDTH; x++) {
        if (at(x, y) < 0) {
          // If there are ANY -1's our line is not done
          lineIsFilled = false;
          break;
        }
      }

      if (lineIsFilled) {
        for (int x = 0; x < BOARD_WIDTH; x++) {
          for (int _y = 0; _y < y; _y++) {
            set(x, y - _y, at(x, y - _y - 1));
          }
        }
      }
    }
  }
};