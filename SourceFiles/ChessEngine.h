#pragma once

#include <stddef.h>
#include <array>

const int GRID_HEX_COUNT = 121;
const int GRID_LENGTH = 11;

enum {
    Null = -1,
    None = 0,
    King = 1,
    Pawn = 2,
    Knight = 3,
    Bishop = 4,
    Rook = 5,
    Queen = 6,

    White = 8,
    Black = 16,
};

enum {
    NorthWest = - GRID_LENGTH,
    SouthWest = -1,
    North = - GRID_LENGTH + 1,
    South = GRID_LENGTH - 1,
    NorthEast = 1,
    SouthEast = GRID_LENGTH,

    EastDiagonal = GRID_LENGTH + 1,
    WestDiagonal = - GRID_LENGTH - 1,
    NorthWestDiagonal = - 2 * GRID_LENGTH + 1,
    NorthEastDiagonal = - GRID_LENGTH + 2,
    SouthWestDiagonal = GRID_LENGTH - 2,
    SouthEastDiagonal = 2 * GRID_LENGTH + 1
};

struct Move
{
    int startingHex;
    int targetHex;
};

class ChessEngine
{

private:

    std::array<int, GRID_HEX_COUNT> m_grid;

    bool m_whiteToMove = true;

public:

    ChessEngine();

    void init();
    void initializeGlinskiBoard();

    void movePiece(size_t startIndex, size_t targetIndex);

    const std::array<int, GRID_HEX_COUNT>& getBoard() const { return m_grid ; };
    int getPiece(size_t index) const;
    bool whiteToMove() const { return m_whiteToMove ; }
    bool pieceColor(int piece) const; // White = true

};
