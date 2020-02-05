#include <cstdint>
#include <iostream>
#include <string>

const std::string red = "\033[0;31m";
const std::string green = "\033[1;32m";
const std::string reset = "\033[0m";

/*
    White:  0000 0001
    Black:  0000 0010
    Pawn:   0000 0100
    Knight: 0000 1000
    Bishop: 0001 0000
    Rook:   0010 0000
    Queen:  0100 0000
    King:   1000 0000
*/

class Board {
private:
    uint8_t board[64] = {
        0b00100010, 0b00001010, 0b00010010, 0b01000010, 0b10000010, 0b00010010, 0b00001010, 0b00100010,
        0b00000110, 0b00000110, 0b00000110, 0b00000110, 0b00000110, 0b00000110, 0b00000110, 0b00000110,
        0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
        0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
        0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
        0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000, 0b00000000,
        0b00000101, 0b00000101, 0b00000101, 0b00000101, 0b00000101, 0b00000101, 0b00000101, 0b00000101,
        0b00100001, 0b00001001, 0b00010001, 0b01000001, 0b10000001, 0b00010001, 0b00001001, 0b00100001
    };
    uint64_t movements[64] = {
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0
    };
    bool sameColor(uint8_t a, uint8_t b);
    uint8_t step(uint8_t mvt, int x, int y, int dir, bool forwardPawn = false);
    void calcMovement(int index);
public:
    Board();
    void displayBoard();
};

bool Board::sameColor(uint8_t a, uint8_t b) {
    if (!a || !b) {
        return false;
    }
    return ((a & 1) == (b & 1));
}

uint8_t Board::step(uint8_t mvt, int x, int y, int dir, bool forwardPawn) {
    if (x < 0 || x > 7 || y < 0 || y > 7) {
        return mvt;
    }
    uint8_t piece = board[x * 8 + y];
    if ((piece && ((piece >> 1) & 1) != dir) || !piece) {
        mvt |= (1 << (63 - (x * 8 + y)));
    }
    return mvt;
}

void Board::calcMovement(int index) {
    uint8_t piece = board[index];
    if (!piece) {
        movements[index] = 0;
        return;
    }

    uint64_t mvt = 0;
    int x = index / 8, y = index % 8;
    int dir;
    (piece >> 1) & 1 ? dir = 1 : dir = -1;

    switch (piece & 0b11111100) {
        case 0b00000100:
            mvt = step(mvt, x + dir, y, dir, true);
            mvt = step(mvt, x + dir, y + 1, dir);
            mvt = step(mvt, x + dir, y - 1, dir);
            if ((dir && x == 1) || (!dir && x == 6)) {
                mvt = step(mvt, x + dir, y, dir, true);
            }
            break;
        case 0b00001000:
            mvt = step(mvt, x + 2, y + 1, dir);
            mvt = step(mvt, x + 2, y - 1, dir);
            mvt = step(mvt, x - 2, y + 1, dir);
            mvt = step(mvt, x - 2, y - 1, dir);
            mvt = step(mvt, x + 1, y + 2, dir);
            mvt = step(mvt, x + 1, y - 2, dir);
            mvt = step(mvt, x - 1, y + 2, dir);
            mvt = step(mvt, x - 1, y - 2, dir);
            break;
        case 0b00010000:
            for (int i = 1; i < 8; i++) {
                uint8_t next = step(mvt, x + i, y + i, dir);
                if (next == mvt) {
                    break;
                } else {
                    mvt = next;
                }
            }
            for (int i = 1; i < 8; i++) {
                uint8_t next = step(mvt, x + i, y - i, dir);
                if (next == mvt) {
                    break;
                } else {
                    mvt = next;
                }
            }
            for (int i = 1; i < 8; i++) {
                uint8_t next = step(mvt, x - i, y + i, dir);
                if (next == mvt) {
                    break;
                } else {
                    mvt = next;
                }
            }
            for (int i = 1; i < 8; i++) {
                uint8_t next = step(mvt, x - i, y - i, dir);
                if (next == mvt) {
                    break;
                } else {
                    mvt = next;
                }
            }
            break;
        case 0b00100000:
            for (int i = 1; i < 8; i++) {
                uint8_t next = step(mvt, x + i, y, dir);
                if (next == mvt) {
                    break;
                } else {
                    mvt = next;
                }
            }
            for (int i = 1; i < 8; i++) {
                uint8_t next = step(mvt, x - i, y, dir);
                if (next == mvt) {
                    break;
                } else {
                    mvt = next;
                }
            }
            for (int i = 1; i < 8; i++) {
                uint8_t next = step(mvt, x, y + i, dir);
                if (next == mvt) {
                    break;
                } else {
                    mvt = next;
                }
            }
            for (int i = 1; i < 8; i++) {
                uint8_t next = step(mvt, x, y - i, dir);
                if (next == mvt) {
                    break;
                } else {
                    mvt = next;
                }
            }
            break;
        case 0b01000000:
            for (int i = 1; i < 8; i++) {
                uint8_t next = step(mvt, x + i, y + i, dir);
                if (next == mvt) {
                    break;
                } else {
                    mvt = next;
                }
            }
            for (int i = 1; i < 8; i++) {
                uint8_t next = step(mvt, x + i, y - i, dir);
                if (next == mvt) {
                    break;
                } else {
                    mvt = next;
                }
            }
            for (int i = 1; i < 8; i++) {
                uint8_t next = step(mvt, x - i, y + i, dir);
                if (next == mvt) {
                    break;
                } else {
                    mvt = next;
                }
            }
            for (int i = 1; i < 8; i++) {
                uint8_t next = step(mvt, x - i, y - i, dir);
                if (next == mvt) {
                    break;
                } else {
                    mvt = next;
                }
            }
            for (int i = 1; i < 8; i++) {
                uint8_t next = step(mvt, x + i, y, dir);
                if (next == mvt) {
                    break;
                } else {
                    mvt = next;
                }
            }
            for (int i = 1; i < 8; i++) {
                uint8_t next = step(mvt, x - i, y, dir);
                if (next == mvt) {
                    break;
                } else {
                    mvt = next;
                }
            }
            for (int i = 1; i < 8; i++) {
                uint8_t next = step(mvt, x, y + i, dir);
                if (next == mvt) {
                    break;
                } else {
                    mvt = next;
                }
            }
            for (int i = 1; i < 8; i++) {
                uint8_t next = step(mvt, x, y - i, dir);
                if (next == mvt) {
                    break;
                } else {
                    mvt = next;
                }
            }
            break;
        case 0b10000000:
            mvt = step(mvt, x + 1, y + 1, dir);
            mvt = step(mvt, x + 1, y - 1, dir);
            mvt = step(mvt, x - 1, y + 1, dir);
            mvt = step(mvt, x - 1, y - 1, dir);
            mvt = step(mvt, x + 1, y + 1, dir);
            mvt = step(mvt, x + 1, y - 1, dir);
            mvt = step(mvt, x - 1, y + 1, dir);
            mvt = step(mvt, x - 1, y - 1, dir);
    }

    movements[index] = mvt;
}

Board::Board() {
    calcMovement(0);
}

void Board::displayBoard() {
    for (int i = 0; i < 64; i++) {
        uint8_t piece = board[i];
        if (piece) {
            std::string color = ((piece >> 1) & 1) ? red : green;
            switch (piece & 0b11111100) {
                case 0b00000100:
                    std::cout << color << "P" << reset;
                    break;
                case 0b00001000:
                    std::cout << color << "N" << reset;
                    break;
                case 0b00010000:
                    std::cout << color << "B" << reset;
                    break;
                case 0b00100000:
                    std::cout << color << "R" << reset;
                    break;
                case 0b01000000:
                    std::cout << color << "Q" << reset;
                    break;
                case 0b10000000:
                    std::cout << color << "K" << reset;
            }
        } else {
            std::cout << "e";
        }
        std::cout << reset << " ";
        if ((i + 1) % 8 == 0) {
            std::cout << "\n";
        }
    }
    std::cout << "\n";
}

int main() {
    Board b;
    b.displayBoard();
    return 0;
}