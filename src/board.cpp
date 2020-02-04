#include <cstdint>
#include <iostream>

class Board {
private:
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
    void calcMovement(int index);
public:
    Board();
};

bool Board::sameColor(uint8_t a, uint8_t b) {
    if (!a || !b) {
        return false;
    }
    return ((a & 1) == (b & 1));
}

void Board::calcMovement(int index) {
    uint8_t piece = board[index];
    if (!piece) {
        movements[index] = 0;
        return;
    }

    uint64_t mvt = 0;
    int x = index / 8, y = index % 8;

    switch (piece && 0b11111100) {
        case 0b00000100:
            int dir = -1;
            if ((piece >> 1) & 1) {
                dir = 1;
            }
            x += dir;
            if (x < 0 || x > 7) {
                break;
            }
            if (!board[x * 8 + y]) {
                mvt |= (1 << (63 - (x * 8 + y)));
            }
            y -= dir;
            if (y >= 0 && y <= 7) {
                if (sameColor(piece, board[x * 8 + y])) {
                    mvt |= (1 << (63 - (x * 8 + y)));
                }
            }
            y += 2 * dir;
            if (y >= 0 && y <= 7) {
                if (sameColor(piece, board[x * 8 + y])) {
                    mvt |= (1 << (63 - (x * 8 + y)));
                }
            }
            y -= dir;
            x += dir;
            if (x < 0 || x > 7) {
                break;
            }
            if (!board[x * 8 + y]) {
                mvt |= (1 << (63 - (x * 8 + y)));
            }
            break;
        case 0b00001000:
            break;
        case 0b00010000:
            break;
        case 0b00100000:
            break;
        case 0b01000000:
            break;
        case 0b10000000:
    }

    movements[index] = mvt;
}

Board::Board() {
    calcMovement(0);
}

int main() {
    Board b;
    return 0;
}