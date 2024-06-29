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
    std::cout << "After initializeBoard function call\n";
    for (int i = 0; i < TOTAL_PIECE_COUNT; i++)
    {
        std::cout << m_piecePositions[i] << '\n';
    }
    initializeDirectionsArray();
    initializeDistanceToEndGrid();

}

void ChessEngine::initializeBoard(const std::string& fen)
{
    int row = 0;
    int column = 0;
    int nonExistentOffset = 5 - row;

    int piecesIndex = 0;

    std::fill(m_piecePositions.begin(), m_piecePositions.end(), EMPTY);
    std::fill(m_pieces.begin(), m_pieces.end(), EMPTY);
    std::fill(m_grid.begin(), m_grid.end(), NONEXISTENT);

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
            std::fill_n(m_grid.begin() + GRID_LENGTH * (row + 1) + column + nonExistentOffset, GRID_LENGTH, NONEXISTENT);
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

            m_pieces[piecesIndex] = pieceToPlace;
            m_piecePositions[piecesIndex] = gridIndexOfPlacedPiece;
            m_grid[gridIndexOfPlacedPiece] = piecesIndex;

            std::cout << "piece pos inside loop: " << m_piecePositions[piecesIndex] << '\n';
            std::cout << "m_grid inside loop: " << m_grid[gridIndexOfPlacedPiece] << '\n';
            std::cout << "piece inside loop: " << m_pieces[piecesIndex] << '\n';

            piecesIndex++;
            column++;
        }
    }

    std::cout << "Exiting initializeBoard function\n";
    for (int i = 0; i < TOTAL_PIECE_COUNT; i++)
    {
        std::cout << "piece pos inside function: " << m_piecePositions[i] << '\n';
    }
}

void ChessEngine::initializeDistanceToEndGrid()
{
    for (int i = 0; i < GRID_HEX_COUNT; i++)
    {
        if (m_grid[i] == NONEXISTENT) { continue ; }

        int initialRow = i / GRID_LENGTH;

        for (int j = 0; j < 12; j++)
        {
            if (i + m_directions[j] < 0 || i + m_directions[j] >= GRID_HEX_COUNT) { m_distanceToEndGrid[i][j] = 0 ; continue ; }

            int directionMultiplier = 1;

            while (i + m_directions[j] * directionMultiplier >= 0 && i + m_directions[j] * directionMultiplier < GRID_HEX_COUNT)
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
                generatePawnMoves(startPosition, piece);
                break;
            case KING:
                generateKingMoves(startPosition, piece);
                break;
            case KNIGHT:
                generateKnightMoves(startPosition, piece);
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

void ChessEngine::generatePawnMoves(int startPosition, int piece) {}
void ChessEngine::generateKingMoves(int startPosition, int piece) {}
void ChessEngine::generateKnightMoves(int startPosition, int piece) {}
