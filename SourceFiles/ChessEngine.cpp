#include "ChessEngine.h"

ChessEngine::ChessEngine()
{
    init();
}

void ChessEngine::init()
{
    initializeGlinskiBoard();
    initializeDistanceToEndGrid();
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

void ChessEngine::initializeDistanceToEndGrid()
{
    int directions[12] = {
        North, South, NorthEast, SouthEast, NorthWest, SouthWest,
        EastDiagonal, WestDiagonal, NorthWestDiagonal, NorthEastDiagonal, SouthWestDiagonal, SouthEastDiagonal
    };

    for (int i = 0; i < GRID_HEX_COUNT; i++)
    {
        if (m_grid[i] == Null) { continue ; }

        int initialRow = i / GRID_LENGTH;

        for (int j = 0; j < 12; j++)
        {
            if (i + directions[j] < 0 || i + directions[j] >= GRID_HEX_COUNT) { m_distanceToEndGrid[i][j] = 0 ; continue ; }

            int directionMultiplier = 1;

            while (i + directions[j] * directionMultiplier >= 0 && i + directions[j] * directionMultiplier < GRID_HEX_COUNT)
            {
                if (m_grid[i + directions[j] * directionMultiplier] == Null) { break; }

                int currentRow = (i + directions[j] * directionMultiplier) / GRID_LENGTH;
                int distanceBetweenCurrentAndLastRow = abs(currentRow - initialRow);

                if (directions[j] == SouthWest || directions[j] == NorthEast)
                {
                    if (currentRow != initialRow) { break; }
                }
                else if (directions[j] == South || directions[j] == North)
                {
                    if (distanceBetweenCurrentAndLastRow != directionMultiplier) { break; }
                }
                else if (directions[j] == WestDiagonal || directions[j] == EastDiagonal)
                {
                    if (distanceBetweenCurrentAndLastRow != directionMultiplier) { break; }
                }
                else if (directions[j] == NorthWestDiagonal || directions[j] == SouthEastDiagonal)
                {
                    if (distanceBetweenCurrentAndLastRow != 2 * directionMultiplier) { break; }
                }
                else if (directions[j] == NorthEastDiagonal || directions[j] == SouthWestDiagonal)
                {
                    if (distanceBetweenCurrentAndLastRow != directionMultiplier) { break; }
                }

                directionMultiplier++;
            }

            m_distanceToEndGrid[i][j] = --directionMultiplier;
        }
    }
}

void ChessEngine::distancesToEndGridFromHex(int hex) const
{
    int directions[12] = {
        North, South, NorthEast, SouthEast, NorthWest, SouthWest,
        EastDiagonal, WestDiagonal, NorthWestDiagonal, NorthEastDiagonal, SouthWestDiagonal, SouthEastDiagonal
    };

    for (int i = 0; i < 12; i++)
    {
        switch (directions[i])
        {
        case North:
            std::cout << "North: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case South:
            std::cout << "South: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case NorthWest:
            std::cout << "NorthWest: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case NorthEast:
            std::cout << "NorthEast: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case SouthWest:
            std::cout << "SouthWest: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case SouthEast:
            std::cout << "SouthEast: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case EastDiagonal:
            std::cout << "East Diagonal: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case WestDiagonal:
            std::cout << "West Diagonal: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case NorthWestDiagonal:
            std::cout << "North West Diagonal: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case SouthWestDiagonal:
            std::cout << "South West Diagonal: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case NorthEastDiagonal:
            std::cout << "North East Diagonal: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case SouthEastDiagonal:
            std::cout << "South East Diagonal: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        default:
            std::cerr << "UNEXPECTED DIRECTION" << std::endl;
        }
    }
}
