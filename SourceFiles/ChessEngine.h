#pragma once

#include <stddef.h>
#include <array>
#include <cmath>
#include <iostream>
#include <vector>

const int GRID_CELL_COUNT = 121;
const int GRID_HEX_COUNT = 91;
const int GRID_LENGTH = 11;

const int TOTAL_PIECE_COUNT = 36;
const std::string GLINSKI_BOARD = "1prnqb/2p2bk/3p1b1n/4p3b/5ppppp/11/PPPPP5/R3P4/N1B1P3/QB2P2/BKNRP1";

enum {
    NONEXISTENT = -2,
    EMPTY = -1,
    KING = 1,
    PAWN = 2,
    KNIGHT = 3,
    BISHOP = 4,
    ROOK = 5,
    QUEEN = 6,

    WHITE = 8,
    BLACK = 16,
};

enum {
    NORTH_WEST = - GRID_LENGTH,
    SOUTH_WEST = -1,
    NORTH = - GRID_LENGTH + 1,
    SOUTH = GRID_LENGTH - 1,
    NORTH_EAST = 1,
    SOUTH_EAST = GRID_LENGTH,

    EAST_DIAGONAL = GRID_LENGTH + 1,
    WEST_DIAGONAL = - GRID_LENGTH - 1,
    NORTH_WEST_DIAGONAL = - 2 * GRID_LENGTH + 1,
    NORTH_EAST_DIAGONAL = - GRID_LENGTH + 2,
    SOUTH_WEST_DIAGONAL = GRID_LENGTH - 2,
    SOUTH_EAST_DIAGONAL = 2 * GRID_LENGTH - 1
};

const int PAWN_TAKE = 0;
const std::pair<int, int> PAWN_MOVES(1, 3);
const std::pair<int, int> QUEEN_DIAGONAL_MOVES(6, 8);
const std::pair<int, int> BISHOP_DIAGONAL_MOVES(8, 12);
const std::pair<int, int> ALL_ADJACENT_MOVES(0, 6);
const std::pair<int, int> ALL_DIAGONAL_MOVES(6, 12);
const std::pair<int, int> ALL_MOVES(0, 12);

struct Move
{
    int start;
    int target;

    Move(int s, int t)
        : start(s), target(t) {}
};

class ChessEngine
{

private:

    std::array<int, GRID_CELL_COUNT> m_grid;
    std::array<int, TOTAL_PIECE_COUNT> m_piecePositions;
    std::array<int, TOTAL_PIECE_COUNT> m_pieces;

    std::array<int, 12> m_directions;
    std::vector<Move> m_moves;
    int m_distanceToEndGrid[121][12];

    bool m_whiteToMove = true;
    bool m_nextTurn = true;

private:

    void init();
    void initializeBoard(const std::string& fen);
    void initializeDistanceToEndGrid();
    void initializeDirectionsArray();

    void generatePawnMoves(int startPosition, int piece);
    void generateKingMoves(int startPosition, int piece);
    void generateSlidingMoves(int startPosition, int piece);
    void generateKnightMoves(int startPosition, int piece);

public:

    ChessEngine();

    // Getters
    const std::array<int, GRID_CELL_COUNT>& getBoard() const { return m_grid ; };
    const std::array<int, TOTAL_PIECE_COUNT>& getPieces() const { return m_pieces ; };
    const std::array<int, TOTAL_PIECE_COUNT>& getPiecePositions() const { return m_piecePositions ; };
    const std::vector<Move>& getMoves() const { return m_moves ; }
    int getPiece(size_t index) const { return m_grid[index] ; }
    void distancesToEndGridFromHex(int hex) const;
    bool whiteToMove() const { return m_whiteToMove ; }
    bool pieceColor(int piece) const { return (piece >> 3 & 1) ; } // WHITE = TRUE

    bool isMoveAvailable(const Move& move) const;

    void movePiece(const Move& move);

    void updatePieceMoves();

};
