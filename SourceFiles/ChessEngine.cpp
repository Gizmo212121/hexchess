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
    initializePiecePositionsArray();
}

void ChessEngine::initializeBoard(const std::string& fen)
{
    int row = 0;
    int column = 0;

    int strLen = static_cast<int>(fen.length());

    int nonExistentOffset = 5 - row;

    for (int i = 0; i < strLen ; i++)
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
            for (int j = 0; j < nonExistentOffset; j++)
            {
                m_grid[row * GRID_LENGTH + column + j] = NONEXISTENT;
            }

            column += nonExistentOffset;
        }
        else if (column == 0)
        {
            for (int j = nonExistentOffset + GRID_LENGTH; j < GRID_LENGTH; j++)
            {
                m_grid[row * GRID_LENGTH + column + j] = NONEXISTENT;
            }
        }

        if (isdigit(char1))
        {
            char char2 = fen[i + 1];

            int offset = 0;

            int char1int = char1 - '0';

            if (isdigit(char2))
            {
                int char2int = char2 - '0';
                offset += (char1int) * 10 + char2int;
                i++; // DONT WANT TO CONSIDER SAME NUMBER TWICE
            }
            else
            {
                offset = char1int;
            }

            if (offset > GRID_LENGTH || offset + column > GRID_LENGTH)
            {
                std::cerr << "Offset of " << offset << "not allowed for FEN strings! Error at index " << i << '\n';
                exit(1);
            }

            for (int j = 0; j < offset; j++)
            {
                m_grid[row * GRID_LENGTH + column + j] = EMPTY;
            }

            column += offset;
        }
        else
        {
            bool isUppercase = std::isupper(char1);
            char piece = std::tolower(char1);

            int pieceToPlace;

            switch (piece)
            {
                case 'p':
                    pieceToPlace = PAWN;
                    break;
                case 'n':
                    pieceToPlace = KNIGHT;
                    break;
                case 'b':
                    pieceToPlace = BISHOP;
                    break;
                case 'r':
                    pieceToPlace = ROOK;
                    break;
                case 'q':
                    pieceToPlace = QUEEN;
                    break;
                case 'k':
                    pieceToPlace = KING;
                    break;
                default:
                    std::cerr << "Unkown character in FEN string: " << piece << '\n';
                    exit(1);
            }

            pieceToPlace |= (isUppercase) ? WHITE : BLACK;

            m_grid[row * GRID_LENGTH + column] = pieceToPlace;

            column++;
        }
    }
}

void ChessEngine::initializeGlinskiBoard()
{
    m_grid = {

        NONEXISTENT, NONEXISTENT, NONEXISTENT, NONEXISTENT, NONEXISTENT, EMPTY, BLACK | PAWN, BLACK | ROOK, BLACK | KNIGHT, BLACK | QUEEN, BLACK | BISHOP,
        NONEXISTENT, NONEXISTENT, NONEXISTENT, NONEXISTENT, EMPTY, EMPTY, BLACK | PAWN, EMPTY, EMPTY, BLACK | BISHOP, BLACK | KING,
        NONEXISTENT, NONEXISTENT, NONEXISTENT, EMPTY, EMPTY, EMPTY, BLACK | PAWN, EMPTY, BLACK | BISHOP, EMPTY, BLACK | KNIGHT,
        NONEXISTENT, NONEXISTENT, EMPTY, EMPTY, EMPTY, EMPTY, BLACK | PAWN, EMPTY, EMPTY, EMPTY, BLACK | BISHOP,
        NONEXISTENT, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, BLACK | PAWN, BLACK | PAWN, BLACK | PAWN, BLACK | PAWN, BLACK | PAWN,
        EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY,
        WHITE | PAWN, WHITE | PAWN, WHITE | PAWN, WHITE | PAWN, WHITE | PAWN, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, NONEXISTENT,
        WHITE | ROOK, EMPTY, EMPTY, EMPTY, WHITE | PAWN, EMPTY, EMPTY, EMPTY, EMPTY, NONEXISTENT, NONEXISTENT,
        WHITE | KNIGHT, EMPTY, WHITE | BISHOP, EMPTY, WHITE | PAWN, EMPTY, EMPTY, EMPTY, NONEXISTENT, NONEXISTENT, NONEXISTENT,
        WHITE | QUEEN, WHITE | BISHOP, EMPTY, EMPTY, WHITE | PAWN, EMPTY, EMPTY, NONEXISTENT, NONEXISTENT, NONEXISTENT, NONEXISTENT,
        WHITE | BISHOP, WHITE | KING, WHITE | KNIGHT, WHITE | ROOK, WHITE | PAWN, EMPTY, NONEXISTENT, NONEXISTENT, NONEXISTENT, NONEXISTENT, NONEXISTENT

    };
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

void ChessEngine::initializePiecePositionsArray()
{
    int count = 0;

    for (int i = 0; i < GRID_HEX_COUNT; i++)
    {
        if (m_grid[i] != NONEXISTENT && m_grid[i] != EMPTY)
        {
            m_piecePositions[count++] = i;
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

int ChessEngine::getPiece(size_t index) const
{
    return m_grid[index];
}

bool ChessEngine::pieceColor(int piece) const
{
    return (piece >> 3 & 1);
}

void ChessEngine::movePiece(const Move& move)
{
    // Move piece in the array of piece positions
    auto it = std::find(m_piecePositions.begin(), m_piecePositions.end(), move.start);
    m_piecePositions[it[0]] = move.target;
    std::cout << "Changed m_piecePositions: Index: " << it[0] << " Position: " << move.target << std::endl;


    // Move the piece in the main grid
    m_grid[move.target] = m_grid[move.start];
    m_grid[move.start] = EMPTY;

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

    for (int startPosition : m_piecePositions)
    {
        int piece = m_grid[startPosition];

        if (pieceColor(piece) == m_whiteToMove)
        {
            piece = piece ^ ((m_whiteToMove) ? 0b1000 : 0b10000);

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
            int index = startPosition + direction * distance;
            int pieceAtHex = m_grid[index];

            if (pieceAtHex != EMPTY)
            {
                if (pieceColor(pieceAtHex) == !m_whiteToMove)
                {
                    m_moves.push_back(Move(startPosition, index));
                    break;
                }
                else
                {
                    break;
                }
            }
            else
            {
                m_moves.push_back(Move(startPosition, index));
            }
        }
    }
}

void ChessEngine::generatePawnMoves(int startPosition, int piece) {}
void ChessEngine::generateKingMoves(int startPosition, int piece) {}
void ChessEngine::generateKnightMoves(int startPosition, int piece) {}
