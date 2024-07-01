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
const std::string GLINSKI_BOARD = "1prnqb/2p2bk/3p1b1n/4p3r/5ppppp/11/PPPPP5/R3P4/N1B1P3/QB2P2/BKNRP1";

//const int TOTAL_PIECE_COUNT = 4;
//const std::string GLINSKI_BOARD = "4q1/6k/8/9/10/11/10/9/8/Q6/1K4";

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
    NULL_DIRECTION = 0,
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

const int PAWN_MOVE = 0;
const std::pair<int, int> PAWN_TAKES(1, 3);
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

    // Represents the main grid. Contains indices that points to m_piecePositions and m_pieces
    std::array<int, GRID_CELL_COUNT> m_grid;
    // Contains indices for every piece. Said index points to m_grid
    std::array<int, TOTAL_PIECE_COUNT> m_piecePositions;
    // Contains integers representing the pieces in a more meaningful way from m_piecePositions.
    std::array<int, TOTAL_PIECE_COUNT> m_pieces;
    std::array<int, TOTAL_PIECE_COUNT> m_piecesWithoutColor;

    bool m_canDoubleMove[TOTAL_PIECE_COUNT];
    int m_enPessantGridMoveIndex = -1;
    int m_enPessantGridTakeIndex = -1;

    std::array<int, 12> m_directions;
    std::array<int, 12> m_knightDirections;

    int m_distanceToEndGrid[GRID_CELL_COUNT][12];
    bool m_knightMoveExistenceInGrid[GRID_CELL_COUNT][12];

    std::vector<Move> m_moves;
    std::array<int, TOTAL_PIECE_COUNT> m_pins;
    // King positions are stored as m_pieces indices, i.e between 0 and TOTAL_PIECE_COUNT
    int m_whiteKingPosition;
    int m_blackKingPosition;

    bool m_whiteToMove = true;
    bool m_nextTurn = true;

private:

    void init();
    void initializeBoard(const std::string& fen);
    void initializeDistanceToEndGrid();
    void initializeDirectionsArray();
    void initializeKnightDirectionsArray();
    void initializeKnightMoveExistenceArray();
    void initializePinArray();

    void generatePawnMoves(int startPosition, int piece);
    void generateKingMoves(int startPosition);
    void generateSlidingMoves(int startPosition, int piece);
    void generateKnightMoves(int startPosition);

public:

    ChessEngine();

    // Getters
    const std::array<int, GRID_CELL_COUNT>& getBoard() const { return m_grid ; }
    int grid(int index) const { return m_grid[index] ; }
    const std::array<int, TOTAL_PIECE_COUNT>& getPieces() const { return m_pieces ; }
    int getPiece(int index) const { return m_pieces[m_grid[index]] ; }
    const std::array<int, TOTAL_PIECE_COUNT>& getPiecePositions() const { return m_piecePositions ; }
    int getPiecePosition(int index) const { return m_piecePositions[index] ; }
    const std::vector<Move>& getMoves() const { return m_moves ; }
    void distancesToEndGridFromHex(int hex) const;
    bool whiteToMove() const { return m_whiteToMove ; }
    bool pieceColor(int piece) const { return (piece >> 3 & 1) ; } // WHITE = TRUE

    int enPessantGridMoveIndex() const { return m_enPessantGridMoveIndex ; }

    bool isMoveAvailable(const Move& move) const;

    void movePiece(const Move& move);

    void generatePins();
    void updatePieceMoves();

};
