#include "ScenePlay.h"
#include "ChessEngine.h"
#include "GameEngine.h"
#include <SFML/System/Vector2.hpp>

ScenePlay::ScenePlay(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    init();
}

void ScenePlay::init()
{
    m_gridAxialText.setCharacterSize(12);
    m_gridAxialText.setFont(m_game->assets().getFont("Arial"));
    m_gridIndexText.setCharacterSize(12);
    m_gridIndexText.setFont(m_game->assets().getFont("Arial"));

    registerKeyboardAction(sf::Keyboard::Escape, "EXIT");
    registerKeyboardAction(sf::Keyboard::P, "PAUSE");
    registerKeyboardAction(sf::Keyboard::T, "TOGGLE_TEXTURES");
    registerKeyboardAction(sf::Keyboard::A, "TOGGLE_GRID_AXIAL_POS");
    registerKeyboardAction(sf::Keyboard::G, "TOGGLE_GRID_INDICES");

    registerMouseAction(sf::Mouse::Left, "MOUSE_LEFT");

    initializeIndexToAxialArray();
    initializeHexSpriteBoard();
    initializePieceSpriteBoard();
}

void ScenePlay::initializeIndexToAxialArray()
{
    for (int index = 0; index < GRID_CELL_COUNT; index++)
    {
        int column = index % GRID_LENGTH;
        int row = index / GRID_LENGTH;

        int q = column + row + 1 - GRID_LENGTH;
        int r = 5 - row;

        m_indexToAxial[index] = sf::Vector2f(q, r);
    }
}

void ScenePlay::initializeHexSpriteBoard()
{
    std::array<int, GRID_CELL_COUNT> board = m_chessEngine.getBoard();

    for (size_t i = 0; i < GRID_CELL_COUNT; i++)
    {
        if (board[i] != NONEXISTENT)
        {
            sf::Vector2f screenPos = axialToPixel(m_indexToAxial[i]);

            sf::Vector2f axialPos = m_indexToAxial[i];
            int colorCode = static_cast<int>(axialPos.x) - static_cast<int>(axialPos.y);
            std::string hexAssetName;

            if (colorCode % 3 == 0) { hexAssetName = "Hex1"; }
            else if (colorCode % 3 == -1) { hexAssetName = "Hex2"; }
            else if (colorCode % 3 == 2) { hexAssetName = "Hex2"; }
            else { hexAssetName = "Hex0"; }

            m_hexSpriteBoard[i].setTexture(m_game->assets().getTexture(hexAssetName));

            float hexScale = height() / (GRID_LENGTH * m_hexTextureSize);
            m_hexSpriteBoard[i].setScale(hexScale, hexScale);
            m_hexSpriteBoard[i].setScale(hexSpriteScale, hexSpriteScale);

            sf::FloatRect localBounds = m_hexSpriteBoard[i].getLocalBounds();
            m_hexSpriteBoard[i].setOrigin(localBounds.width / 2, localBounds.height / 2);

            m_hexSpriteBoard[i].setPosition(screenPos.x, screenPos.y);
        }
    }
}

void ScenePlay::initializePieceSpriteBoard()
{
    std::array<int, TOTAL_PIECE_COUNT> pieceArray = m_chessEngine.getPieces();
    std::array<int, TOTAL_PIECE_COUNT> pieceIndexArray = m_chessEngine.getPiecePositions();

    for (int i = 0; i < TOTAL_PIECE_COUNT; i++)
    {
        sf::Sprite pieceSprite;
        sf::Vector2f screenPos = axialToPixel(m_indexToAxial[pieceIndexArray[i]]);

        int piece = pieceArray[i];

        bool isWhite = m_chessEngine.pieceColor(piece);
        int pieceWithoutColor = piece ^ ((isWhite) ? WHITE : BLACK);

        std::string colorString = (isWhite) ? "w" : "b" ;
        std::string pieceString;

        switch (pieceWithoutColor)
        {
            case KING:
                pieceString = "King";
                break;
            case PAWN:
                pieceString = "Pawn";
                break;
            case KNIGHT:
                pieceString = "Knight";
                break;
            case BISHOP:
                pieceString = "Bishop";
                break;
            case ROOK:
                pieceString = "Rook";
                break;
            case QUEEN:
                pieceString = "Queen";
                break;
            default:
                std::cerr << "Unexpected value in piece init switch statement: " << pieceWithoutColor << std::endl;
                exit(1);
        }

        pieceSprite.setTexture(m_game->assets().getTexture(colorString + pieceString));

        sf::FloatRect localBounds = pieceSprite.getLocalBounds();
        pieceSprite.setOrigin(localBounds.width / 2, localBounds.height / 2);
        pieceSprite.setPosition(screenPos.x, screenPos.y);
        pieceSprite.setScale(pieceSpriteScale, pieceSpriteScale);

        m_pieceSpriteBoard[i] = pieceSprite;
    }
}

void ScenePlay::update()
{
    if (m_paused)
    {
        sRender();
    }
    else
    {
        m_chessEngine.generatePins();

        if (m_profileUpdateFunction)
        {
            sf::Time time = m_debugClock.getElapsedTime();
            std::cout << "Profile: m_chessEngine.generatePins(): " << time.asMicroseconds() << std::endl;
        }

        m_chessEngine.updatePieceMoves();

        if (m_profileUpdateFunction)
        {
            sf::Time time = m_debugClock.getElapsedTime();
            std::cout << "Profile: m_chessEngine.updatePieceMoves(): " << time.asMicroseconds() << std::endl;
        }

        sMouseInput();

        if (m_profileUpdateFunction)
        {
            sf::Time time = m_debugClock.getElapsedTime();
            std::cout << "Profile: sMouseInput(): " << time.asMicroseconds() << std::endl;
        }

        sRender();

        if (m_profileUpdateFunction)
        {
            sf::Time time = m_debugClock.getElapsedTime();
            std::cout << "Profile: sRender(): " << time.asMicroseconds() << std::endl;

            m_debugClock.restart();
        }
    }
}

void ScenePlay::sDoAction(const Action& action)
{
    if (action.state() == Action::START)
    {
        if (action.name() == "EXIT") { onEnd() ; }
        else if (action.name() == "PAUSE") { setPaused() ; }
        else if (action.name() == "MOUSE_LEFT") { m_currentlyClicking = true ; }
        else if (action.name() == "TOGGLE_TEXTURES") { m_renderTextures = !m_renderTextures ; }
        else if (action.name() == "TOGGLE_GRID_AXIAL_POS") { m_renderGridAxialPositions = !m_renderGridAxialPositions ; }
        else if (action.name() == "TOGGLE_GRID_INDICES") { m_renderGridIndices = !m_renderGridIndices ; }
    }
    else if (action.state() == Action::END)
    {
        if (action.name() == "PAUSE") { setUnpaused() ; }
        else if (action.name() == "MOUSE_LEFT") { m_currentlyClicking = false ; }
    }
}

void ScenePlay::sRender()
{
    if (!m_paused) { m_game->window().clear(sf::Color(100, 100, 255)) ; }
    else { m_game->window().clear(sf::Color(50, 50, 150)) ; }

    std::array<int, GRID_CELL_COUNT> board = m_chessEngine.getBoard();

    if (m_renderTextures)
    {
        for (int i = 0; i < GRID_CELL_COUNT; i++)
        {
            if (board[i] != NONEXISTENT)
            {
                m_game->window().draw(m_hexSpriteBoard[i]);
            }
        }

        for (int i = 0; i < TOTAL_PIECE_COUNT; i++)
        {
            if (m_selectedPieceSprite && board[m_selectedPieceIndex] == i) { continue ; }

            if (m_chessEngine.getPiecePosition(i) != EMPTY)
            {
                m_game->window().draw(m_pieceSpriteBoard[i]);
            }
        }

        // If we have a held piece, draw it last so it overlaps
        if (m_selectedPieceSprite)
        { 
            // Draw move spots
            for (const Move& move : m_chessEngine.getMoves())
            {
                if (move.start == m_selectedPieceIndex)
                {
                    if (m_chessEngine.grid(move.target) != EMPTY)
                    {
                        sf::Vector2f screenPos = axialToPixel(m_indexToAxial[move.target]);

                        sf::CircleShape circle;

                        circle.setRadius(30);
                        circle.setOrigin(30, 30);
                        circle.setFillColor(sf::Color(0, 0, 0, 0));
                        circle.setOutlineColor(sf::Color(145, 153, 227));
                        circle.setOutlineThickness(4);
                        circle.setPosition(screenPos.x, screenPos.y);

                        m_game->window().draw(circle);
                    }
                    else
                    {
                        sf::Vector2f screenPos = axialToPixel(m_indexToAxial[move.target]);

                        sf::CircleShape circle;

                        circle.setRadius(15);
                        circle.setOrigin(15, 15);
                        circle.setFillColor(sf::Color(145, 153, 227));
                        circle.setPosition(screenPos.x, screenPos.y);

                        m_game->window().draw(circle);
                    }
                }
            }

            m_game->window().draw( m_pieceSpriteBoard[ m_chessEngine.grid(m_selectedPieceIndex) ] ) ; 
        }
    }

    if (m_renderGridAxialPositions)
    {
        for (size_t i = 0; i < GRID_CELL_COUNT; i++)
        {
            if (board[i] == NONEXISTENT) { continue ; }

            sf::Vector2f axialPos = m_indexToAxial[i];

            std::string axialCoordinateText = '(' + std::to_string(static_cast<int>(axialPos.x)) + ", " + std::to_string(static_cast<int>(axialPos.y)) + ')';

            m_gridAxialText.setString(axialCoordinateText);

            sf::FloatRect localBounds = m_gridAxialText.getLocalBounds();
            m_gridAxialText.setOrigin(localBounds.width / 2, localBounds.height / 2);

            sf::Vector2f screenPos = axialToPixel(axialPos);
            m_gridAxialText.setPosition(screenPos);

            m_game->window().draw(m_gridAxialText);
        }
    }

    if (m_renderGridIndices)
    {
        for (int index = 0; index < GRID_CELL_COUNT; index++)
        {
            sf::Vector2f screenPos = axialToPixel(m_indexToAxial[index]);

            std::string indexString = std::to_string(m_chessEngine.grid(index));

            m_gridIndexText.setString(indexString);

            sf::FloatRect localBounds = m_gridIndexText.getLocalBounds();
            m_gridIndexText.setOrigin(localBounds.width / 2, localBounds.height / 2);

            m_gridIndexText.setPosition(screenPos);

            m_game->window().draw(m_gridIndexText);
        }
    }


    m_game->window().display();
}

void ScenePlay::sMouseInput()
{
    // Initialized variables for selected piece
    sf::Vector2f originalSelectedAxialPosition(100, 100);
    bool m_currentlyDragging = false;

    if (m_selectedPieceSprite)
    {
        originalSelectedAxialPosition = m_indexToAxial[m_selectedPieceIndex];
        m_currentlyDragging = m_selectedPieceSprite->getPosition() != axialToPixel(originalSelectedAxialPosition);
    }

    // Increase piece size that is being held and dragged
    if (m_selectedPieceSprite)
    {
        const float scaleFactor = (m_currentlyDragging) ? 1.5 * pieceSpriteScale : pieceSpriteScale ;
        m_selectedPieceSprite->setScale(scaleFactor, scaleFactor);
    }


    if (m_currentlyClicking || (m_selectedPieceSprite && m_currentlyDragging))
    {
        // Abs and relative mouse positions
        sf::Vector2i absoluteMousePosition = sf::Mouse::getPosition(m_game->window());
        sf::Vector2f relativeMousePosition(absoluteMousePosition.x - width() / 2.0, height() / 2.0 - absoluteMousePosition.y);
        sf::Vector2f mouseAxialPos = pixelToAxial(relativeMousePosition);

        // Determine if there's a piece on the clicked hex
        int pieceOnClickedHexIndex = axialToIndex(mouseAxialPos);
        bool clickedOnPiece = (onBoard(mouseAxialPos) && m_chessEngine.grid(pieceOnClickedHexIndex) != EMPTY);
        sf::Sprite* spriteOfClickedPiece = nullptr;
        spriteOfClickedPiece = (clickedOnPiece) ? &m_pieceSpriteBoard[m_chessEngine.grid(pieceOnClickedHexIndex)] : nullptr ;

        if (m_currentlyClicking)
        {
            if (m_selectedPieceSprite)
            {
                Move candidateMove(m_selectedPieceIndex, pieceOnClickedHexIndex);

                if (m_currentlyDragging)
                {
                    m_selectedPieceSprite->setPosition(absoluteMousePosition.x, absoluteMousePosition.y);
                }
                else if (onBoard(mouseAxialPos) && mouseAxialPos != originalSelectedAxialPosition && m_chessEngine.isMoveAvailable(candidateMove))
                {
                    movePiece(candidateMove);
                    m_selectedPieceSprite = nullptr;
                }
                else if (clickedOnPiece)
                {
                    bool pieceOnClickedHexColor = m_chessEngine.pieceColor(m_chessEngine.getPiece(pieceOnClickedHexIndex));

                    // If you're clicking on one of your own pieces:
                    if (pieceOnClickedHexColor == m_chessEngine.whiteToMove())
                    {
                        // If that piece is the one you have selected:
                        if (mouseAxialPos == originalSelectedAxialPosition)
                        {
                            // Start dragging the piece again:
                            m_selectedPieceSprite->setPosition(absoluteMousePosition.x, absoluteMousePosition.y);
                            m_currentlyDragging = true;
                        }
                        else
                        {
                            // Select another one of your own pieces
                            m_selectedPieceSprite = spriteOfClickedPiece;
                            m_selectedPieceIndex = axialToIndex(mouseAxialPos);
                        }
                    }
                    else
                    {
                        m_selectedPieceSprite->setPosition(axialToPixel(originalSelectedAxialPosition));
                        m_selectedPieceSprite = nullptr;
                    }
                }
                else
                {
                    m_selectedPieceSprite->setPosition(axialToPixel(originalSelectedAxialPosition));
                    m_selectedPieceSprite = nullptr;
                }
            }
            else if (clickedOnPiece)
            {
                bool pieceOnClickedHexColor = m_chessEngine.pieceColor(m_chessEngine.getPiece(pieceOnClickedHexIndex));

                if (pieceOnClickedHexColor == m_chessEngine.whiteToMove())
                {
                    // pick up piece -> cause dragging somehow
                    spriteOfClickedPiece->setPosition(absoluteMousePosition.x, absoluteMousePosition.y);
                    m_selectedPieceSprite = spriteOfClickedPiece;
                    m_selectedPieceIndex = axialToIndex(mouseAxialPos);
                }
            }
        }
        else if (m_selectedPieceSprite && m_currentlyDragging)
        {
            Move candidateMove(m_selectedPieceIndex, axialToIndex(mouseAxialPos));
            // Drop the piece if mouse on move/take hex
            if (onBoard(mouseAxialPos) && mouseAxialPos != originalSelectedAxialPosition && m_chessEngine.isMoveAvailable(candidateMove))
            {
                m_selectedPieceSprite->setScale(pieceSpriteScale, pieceSpriteScale);
                movePiece(candidateMove);
                m_selectedPieceSprite = nullptr;
            }
            else
            {
                m_selectedPieceSprite->setPosition(axialToPixel(originalSelectedAxialPosition));
            }
        }
    }

}

void ScenePlay::onEnd()
{
    m_game->quit();
}

sf::Vector2f ScenePlay::axialToPixel(const sf::Vector2f& vec) const
{
    float x = m_hexTextureSize * (3.0 / 2 * vec.x);
    float y = m_hexTextureSize * (sqrt(3) / 2 * vec.x + sqrt(3) * vec.y);

    float viewportXHalf = m_game->window().getSize().x / 2.0;
    float viewportYHalf = m_game->window().getSize().y / 2.0;

    return sf::Vector2f(viewportXHalf + x, viewportYHalf - y);
}

sf::Vector2f ScenePlay::pixelToAxial(const sf::Vector2f& vec) const
{
    float x = (2.0 / 3 * vec.x) / m_hexTextureSize;
    float y = (-1.0 / 3 * vec.x + sqrt(3) / 3 * vec.y) / m_hexTextureSize;

    return sf::Vector2f(round(x), round(y));
}

size_t ScenePlay::axialToIndex(const sf::Vector2f& axial) const
{
    float row = 5 - axial.y;
    float column = axial.x + axial.y + GRID_LENGTH - 6;

    return static_cast<size_t>(row) * GRID_LENGTH + static_cast<size_t>(column);
}

bool ScenePlay::onBoard(const sf::Vector2f& vec) const
{
    return ((fabs(vec.x) + fabs(vec.y) + fabs(- vec.x - vec.y)) / 2 <= 5);
}

void ScenePlay::movePiece(const Move& move)
{
    // These are in "piece indices", as opposed to "grid indices"
    int startPieceIndex = m_chessEngine.grid(move.start);
    sf::Vector2f screenPos = axialToPixel(m_indexToAxial[move.target]);
    m_pieceSpriteBoard[startPieceIndex].setPosition(screenPos.x, screenPos.y);

    m_chessEngine.movePiece(move);
}
