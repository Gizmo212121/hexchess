#include "ChessEngine.h"

ChessEngine::ChessEngine()
{
    init();
}

void ChessEngine::init()
{
    initializeGlinskiBoard();
}

void ChessEngine::initializeGlinskiBoard()
{
    m_grid = {

        Null, Null, Null, Null, Null, None, Black | Pawn, Black | Rook, Black | Knight, Black | Queen, Black | Bishop,
        Null, Null, Null, Null, None, None, Black | Pawn, None, None, Black | Bishop, Black | King,
        Null, Null, Null, None, None, None, Black | Pawn, None, Black | Bishop, None, Black | Knight,
        Null, Null, None, None, None, None, Black | Pawn, None, None, None, Black | Bishop,
        Null, None, None, None, None, None, Black | Pawn, Black | Pawn, Black | Pawn, Black | Pawn, Black | Pawn,
        None, None, None, None, None, None, None, None, None, None, None,
        White | Pawn, White | Pawn, White | Pawn, White | Pawn, White | Pawn, None, None, None, None, None, Null,
        White | Rook, None, None, None, White | Pawn, None, None, None, None, Null, Null,
        White | Knight, None, White | Bishop, None, White | Pawn, None, None, None, Null, Null, Null,
        White | Queen, White | Bishop, None, None, White | Pawn, None, None, Null, Null, Null, Null,
        White | Bishop, White | King, White | Knight, White | Rook, White | Pawn, None, Null, Null, Null, Null, Null

    };
}

int ChessEngine::getPiece(size_t index) const
{
    return m_grid[index];
}

bool ChessEngine::pieceColor(int piece) const
{
    return (piece >> 3 & 1);
}

void ChessEngine::movePiece(size_t startIndex, size_t targetIndex)
{
    if (m_grid[targetIndex] != Null)
    {
        int piece = m_grid[startIndex];
        m_grid[startIndex] = None;
        m_grid[targetIndex] = piece;
    }

    m_whiteToMove = !m_whiteToMove;
}
