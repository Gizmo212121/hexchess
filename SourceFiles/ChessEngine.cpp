#include "ChessEngine.h"
#include <algorithm>
#include <cctype>

#include <SFML/System.hpp>
ChessEngine::ChessEngine()
{
    init();
}

void ChessEngine::init()
{
    initializeBoard(GLINSKI_BOARD);
    initializeDirectionsArray();
    initializeDistanceToEndGrid();
    initializeKnightDirectionsArray();
    initializeKnightMoveExistenceArray();
}

void ChessEngine::initializeBoard(const std::string& fen)
{
    int row = 0;
    int column = 0;
    int nonExistentOffset = 5 - row;

    int piecesIndex = 0;

    for (int i = 0; i < static_cast<int>(fen.length()) ; i++)
    {
        char char1 = fen[i];

        if (char1 == '/')
        {
            row++;
            nonExistentOffset--;
            column = 0;
            continue;
        }

        if (nonExistentOffset >= 0 && column == 0)
        {
            std::fill_n(m_grid.begin() + row * GRID_LENGTH + column, nonExistentOffset, NONEXISTENT);
            column += nonExistentOffset;
        }
        else if (column == 0)
        {
            std::fill_n(m_grid.begin() + GRID_LENGTH * (row + 1) + nonExistentOffset, abs(nonExistentOffset), NONEXISTENT);
        }

        if (isdigit(char1))
        {
            int offset = char1 - '0';

            if (i + 1 < static_cast<int>(fen.length()) && isdigit(fen[i + 1]))
            {
                offset = offset * 10 + (fen[++i] - '0');
            }

            if (offset > GRID_LENGTH || offset + column > GRID_LENGTH)
            {
                std::cerr << "Offset of " << offset << "not allowed for FEN strings! Error at index " << i << '\n';
                exit(1);
            }

            std::fill_n(m_grid.begin() + row * GRID_LENGTH + column, offset, EMPTY);

            column += offset;
        }
        else
        {
            int pieceToPlace;

            switch (std::tolower(char1))
            {
                case 'p': pieceToPlace = PAWN; break;
                case 'n': pieceToPlace = KNIGHT; break;
                case 'b': pieceToPlace = BISHOP; break;
                case 'r': pieceToPlace = ROOK; break;
                case 'q': pieceToPlace = QUEEN; break;
                case 'k': pieceToPlace = KING; break;
                default:
                    std::cerr << "Unkown character in FEN string: " << std::tolower(char1) << '\n';
                    exit(1);
            }

            pieceToPlace |= (std::isupper(char1)) ? WHITE : BLACK;
            int gridIndexOfPlacedPiece = row * GRID_LENGTH + column;

            if (piecesIndex >= TOTAL_PIECE_COUNT)
            {
                std::cerr << "piecesIndex out of bounds: " << piecesIndex << '\n';
                exit(1);
            }

            if (pieceToPlace < 0) { exit(1) ; }

            m_pieces[piecesIndex] = pieceToPlace;
            m_piecePositions[piecesIndex] = gridIndexOfPlacedPiece;
            m_grid[gridIndexOfPlacedPiece] = piecesIndex;

            piecesIndex++;
            column++;
        }
    }
}

void ChessEngine::initializeDistanceToEndGrid()
{
    for (int i = 0; i < GRID_CELL_COUNT; i++)
    {
        if (m_grid[i] == NONEXISTENT) { continue ; }

        int initialRow = i / GRID_LENGTH;

        for (int j = 0; j < 12; j++)
        {
            if (i + m_directions[j] < 0 || i + m_directions[j] >= GRID_CELL_COUNT) { m_distanceToEndGrid[i][j] = 0 ; continue ; }

            int directionMultiplier = 1;

            while (i + m_directions[j] * directionMultiplier >= 0 && i + m_directions[j] * directionMultiplier < GRID_CELL_COUNT)
            {
                if (m_grid[i + m_directions[j] * directionMultiplier] == NONEXISTENT) { break; }

                int currentRow = (i + m_directions[j] * directionMultiplier) / GRID_LENGTH;
                int distanceBetweenCurrentAndLastRow = abs(currentRow - initialRow);

                if (m_directions[j] == SOUTH_WEST || m_directions[j] == NORTH_EAST)
                {
                    if (currentRow != initialRow) { break; }
                }
                else if (m_directions[j] == SOUTH || m_directions[j] == NORTH)
                {
                    if (distanceBetweenCurrentAndLastRow != directionMultiplier) { break; }
                }
                else if (m_directions[j] == WEST_DIAGONAL || m_directions[j] == EAST_DIAGONAL)
                {
                    if (distanceBetweenCurrentAndLastRow != directionMultiplier) { break; }
                }
                else if (m_directions[j] == NORTH_WEST_DIAGONAL || m_directions[j] == SOUTH_EAST_DIAGONAL)
                {
                    if (distanceBetweenCurrentAndLastRow != 2 * directionMultiplier) { break; }
                }
                else if (m_directions[j] == NORTH_EAST_DIAGONAL || m_directions[j] == SOUTH_WEST_DIAGONAL)
                {
                    if (distanceBetweenCurrentAndLastRow != directionMultiplier) { break; }
                }

                directionMultiplier++;
            }

            m_distanceToEndGrid[i][j] = --directionMultiplier;
        }
    }
}

void ChessEngine::initializeKnightMoveExistenceArray()
{
    for (int i = 0; i < GRID_CELL_COUNT; i++)
    {
        int initialRow = i / GRID_LENGTH;

        if (m_grid[i] == NONEXISTENT) 
        { 
            for (int j = 0; j < 12; j++) { m_knightMoveExistenceInGrid[i][j] = false; }
            continue ; 
        }

        for (int knightDirectionIndex = 0; knightDirectionIndex < 12; knightDirectionIndex++)
        {
            int index = i + m_knightDirections[knightDirectionIndex];
            int targetRow = index / GRID_LENGTH;
            int rowDistance = abs(initialRow - targetRow);

            if (knightDirectionIndex == 0 || knightDirectionIndex == 2 || knightDirectionIndex == 5 || knightDirectionIndex == 7)
            {
                if (rowDistance != 2) { m_knightMoveExistenceInGrid[i][knightDirectionIndex] = false; continue; }
            }
            else if (knightDirectionIndex == 1 || knightDirectionIndex == 3 || knightDirectionIndex == 4 || knightDirectionIndex == 6)
            {
                if (rowDistance != 1) { m_knightMoveExistenceInGrid[i][knightDirectionIndex] = false; continue; }
            }
            else if (knightDirectionIndex == 8 || knightDirectionIndex == 9 || knightDirectionIndex == 10 || knightDirectionIndex == 11)
            {
                if (rowDistance != 3) { m_knightMoveExistenceInGrid[i][knightDirectionIndex] = false; continue; }
            }

            bool onGrid = !(index < 0 || index >= GRID_CELL_COUNT);
            m_knightMoveExistenceInGrid[i][knightDirectionIndex] = onGrid;
        }
    }
}

void ChessEngine::initializeDirectionsArray()
{
    m_directions[0] = NORTH;
    m_directions[1] = NORTH_WEST;
    m_directions[2] = NORTH_EAST;
    m_directions[3] = SOUTH;
    m_directions[4] = SOUTH_WEST;
    m_directions[5] = SOUTH_EAST;

    m_directions[6] = WEST_DIAGONAL;
    m_directions[7] = EAST_DIAGONAL;
    m_directions[8] = NORTH_WEST_DIAGONAL;
    m_directions[9] = NORTH_EAST_DIAGONAL;
    m_directions[10] = SOUTH_WEST_DIAGONAL;
    m_directions[11] = SOUTH_EAST_DIAGONAL;
}

void ChessEngine::initializeKnightDirectionsArray()
{
    m_knightDirections[0] = 2 * GRID_LENGTH - 3;
    m_knightDirections[1] = GRID_LENGTH - 3;
    m_knightDirections[2] = - m_knightDirections[0];
    m_knightDirections[3] = - m_knightDirections[1];
    m_knightDirections[4] = GRID_LENGTH + 2;
    m_knightDirections[5] = 2 * GRID_LENGTH + 1;
    m_knightDirections[6] = - m_knightDirections[4];
    m_knightDirections[7] = - m_knightDirections[5];
    m_knightDirections[8] = 3 * GRID_LENGTH - 1;
    m_knightDirections[9] = 3 * GRID_LENGTH - 2;
    m_knightDirections[10] = - m_knightDirections[8];
    m_knightDirections[11] = - m_knightDirections[9];
}

void ChessEngine::movePiece(const Move& move)
{
    if (m_grid[move.target] != -1) { m_piecePositions[m_grid[move.target]] = -1 ; }

    m_piecePositions[m_grid[move.start]] = move.target;

    m_grid[move.target] = m_grid[move.start];
    m_grid[move.start] = -1;


    m_whiteToMove = !m_whiteToMove;
    m_nextTurn = true;
}

void ChessEngine::distancesToEndGridFromHex(int hex) const
{
    int directions[12] = {
        NORTH, SOUTH, NORTH_EAST, SOUTH_EAST, NORTH_WEST, SOUTH_WEST,
        EAST_DIAGONAL, WEST_DIAGONAL, NORTH_WEST_DIAGONAL, NORTH_EAST_DIAGONAL, SOUTH_WEST_DIAGONAL, SOUTH_EAST_DIAGONAL
    };

    for (int i = 0; i < 12; i++)
    {
        switch (directions[i])
        {
        case NORTH:
            std::cout << "NORTH: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case SOUTH:
            std::cout << "SOUTH: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case NORTH_WEST:
            std::cout << "NORTH_WEST: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case NORTH_EAST:
            std::cout << "NORTH_EAST: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case SOUTH_WEST:
            std::cout << "SOUTH_WEST: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case SOUTH_EAST:
            std::cout << "SOUTH_EAST: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case EAST_DIAGONAL:
            std::cout << "East Diagonal: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case WEST_DIAGONAL:
            std::cout << "West Diagonal: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case NORTH_WEST_DIAGONAL:
            std::cout << "NORTH West Diagonal: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case SOUTH_WEST_DIAGONAL:
            std::cout << "SOUTH West Diagonal: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case NORTH_EAST_DIAGONAL:
            std::cout << "NORTH East Diagonal: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        case SOUTH_EAST_DIAGONAL:
            std::cout << "SOUTH East Diagonal: " << m_distanceToEndGrid[hex][i] << '\n';
            break;
        default:
            std::cerr << "UNEXPECTED DIRECTION" << std::endl;
        }
    }
}

bool ChessEngine::isMoveAvailable(const Move& move) const
{
    for (const Move& m : m_moves)
    {
        if (m.start == move.start && m.target == move.target) { return true ; }
    }

    return false;
}

void ChessEngine::updatePieceMoves()
{
    if (m_nextTurn) { m_nextTurn = false ; }
    else { return ; }

    m_moves.clear();

    for (int startPosition : m_piecePositions)
    {
        int piece = m_pieces[m_grid[startPosition]];

        if (pieceColor(piece) == m_whiteToMove)
        {
            piece = piece ^ ((m_whiteToMove) ? WHITE : BLACK);

            switch (piece)
            {
            case PAWN:
                generatePawnMoves(startPosition);
                break;
            case KING:
                generateKingMoves(startPosition);
                break;
            case KNIGHT:
                generateKnightMoves(startPosition);
                break;
            default:
                generateSlidingMoves(startPosition, piece);
                break;
            }
        }
    }
}

void ChessEngine::generateSlidingMoves(int startPosition, int piece)
{
    std::pair<int, int> directionIndex;

    switch (piece)
    {
        case QUEEN:
            directionIndex = ALL_MOVES;
            break;
        case BISHOP:
            directionIndex = BISHOP_DIAGONAL_MOVES;
            break;
        case ROOK:
            directionIndex = ALL_ADJACENT_MOVES;
            break;
        default:
            std::cerr << "Unexpected piece number: " << piece << std::endl;
            exit(1);
    }

    for (int dirIndex = directionIndex.first; dirIndex < directionIndex.second; dirIndex++)
    {
        int direction = m_directions[dirIndex];

        for (int distance = 1; distance <= m_distanceToEndGrid[startPosition][dirIndex]; distance++)
        {
            int indexInGrid = startPosition + direction * distance;
            int indexInPieceArray = m_grid[indexInGrid];

            if (indexInPieceArray != EMPTY)
            {
                int pieceAtHex = m_pieces[indexInPieceArray];

                if (pieceColor(pieceAtHex) == !m_whiteToMove)
                {
                    m_moves.push_back(Move(startPosition, indexInGrid));
                    break;
                }
                else
                {
                    break;
                }
            }
            else
            {
                m_moves.push_back(Move(startPosition, indexInGrid));
            }
        }
    }
}

void ChessEngine::generateKnightMoves(int startPosition)
{
    for (int knightDirectionIndex = 0; knightDirectionIndex < 12; knightDirectionIndex++)
    {
        if (!m_knightMoveExistenceInGrid[startPosition][knightDirectionIndex]) { continue ; }

        int indexInGrid = startPosition + m_knightDirections[knightDirectionIndex];

        int indexInPieceArray = m_grid[indexInGrid];

        if (indexInPieceArray != EMPTY)
        {
            int pieceAtHex = m_pieces[indexInPieceArray];

            if (pieceColor(pieceAtHex) == !m_whiteToMove)
            {
                m_moves.push_back(Move(startPosition, indexInGrid));
            }
        }
        else
        {
            m_moves.push_back(Move(startPosition, indexInGrid));
        }
    }
}

void ChessEngine::generateKingMoves(int startPosition)
{

}

void ChessEngine::generatePawnMoves(int startPosition)
{

}
