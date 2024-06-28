#include "ScenePlay.h"
#include "ChessEngine.h"
#include "GameEngine.h"

ScenePlay::ScenePlay(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    init();
}

void ScenePlay::init()
{
    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->assets().getFont("Arial"));

    registerKeyboardAction(sf::Keyboard::Escape, "EXIT");
    registerKeyboardAction(sf::Keyboard::P, "PAUSE");
    registerKeyboardAction(sf::Keyboard::T, "TOGGLE_TEXTURES");
    registerKeyboardAction(sf::Keyboard::G, "TOGGLE_GRID");
    registerMouseAction(sf::Mouse::Left, "MOUSE_LEFT");

    initializeHexSpriteBoard();
    initializePieceSpriteBoard();
}

void ScenePlay::initializeHexSpriteBoard()
{
    std::array<int, GRID_HEX_COUNT> board = m_chessEngine.getBoard();

    for (size_t i = 0; i < GRID_HEX_COUNT; i++)
    {
        if (board[i] != Null)
        {
            sf::Vector2f screenPos = axialToPixel(indexToAxial(i));

            sf::Vector2f axialPos = indexToAxial(i);
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
    std::array<int, GRID_HEX_COUNT> board = m_chessEngine.getBoard();

    for (size_t i = 0; i < GRID_HEX_COUNT; i++)
    {
        sf::Sprite pieceSprite;

        sf::Vector2f screenPos = axialToPixel(indexToAxial(i));

        int piece = board[i];

        if (piece == Null || piece == None) 
        { 
            pieceSprite.setPosition(screenPos.x, screenPos.y);
            continue ; 
        }
        else
        {
            bool isWhite = m_chessEngine.pieceColor(piece);
            int pieceWithoutColor;

            // Gives us the enum for the piece without the color bits
            int colorExtractBool = (isWhite) ? 0b1000 : 0b10000 ;
            pieceWithoutColor = piece ^ colorExtractBool;

            std::string colorString = (isWhite) ? "w" : "b" ;
            std::string pieceString;

            switch (pieceWithoutColor)
            {
                case King:
                    pieceString = "King";
                    break;
                case Pawn:
                    pieceString = "Pawn";
                    break;
                case Knight:
                    pieceString = "Knight";
                    break;
                case Bishop:
                    pieceString = "Bishop";
                    break;
                case Rook:
                    pieceString = "Rook";
                    break;
                case Queen:
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
}

void ScenePlay::update()
{
    sf::Clock clock;


    if (m_paused)
    {
        sRender();
    }
    else
    {
        sMouseInput();

        if (profileUpdateFunction)
        {
            sf::Time time = clock.getElapsedTime();
            std::cout << "Profile: sMouseInput(): " << time.asSeconds() << std::endl;
        }

        sRender();

        if (profileUpdateFunction)
        {
            sf::Time time = clock.getElapsedTime();
            std::cout << "Profile: sRender(): " << time.asSeconds() << std::endl;
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
        else if (action.name() == "TOGGLE_GRID") { m_renderGridValues = !m_renderGridValues ; }
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

    std::array<int, GRID_HEX_COUNT> board = m_chessEngine.getBoard();

    if (m_renderTextures)
    {
        for (size_t i = 0; i < GRID_HEX_COUNT; i++)
        {
            if (board[i] != Null)
            {
                m_game->window().draw(m_hexSpriteBoard[i]);
            }
        }

        for (size_t i = 0; i < GRID_HEX_COUNT; i++)
        {
            if (m_selectedPieceSprite && m_selectedPieceIndex == i) { continue ; }

            if (m_pieceSpriteBoard[i].getTexture())
            {
                m_game->window().draw(m_pieceSpriteBoard[i]);
            }
        }

        // If we have a held piece, draw it last so it overlaps
        if (m_selectedPieceSprite) { m_game->window().draw( m_pieceSpriteBoard[ m_selectedPieceIndex ] ) ; }
    }

    if (m_renderGridValues)
    {
        for (size_t i = 0; i < GRID_HEX_COUNT; i++)
        {
            if (board[i] == Null) { continue ; }

            sf::Vector2f axialPos = indexToAxial(i);

            std::string axialCoordinateText = '(' + std::to_string(static_cast<int>(axialPos.x)) + ", " + std::to_string(static_cast<int>(axialPos.y)) + ')';

            m_gridText.setString(axialCoordinateText);

            sf::FloatRect localBounds = m_gridText.getLocalBounds();
            m_gridText.setOrigin(localBounds.width / 2, localBounds.height / 2);

            sf::Vector2f screenPos = axialToPixel(axialPos);
            m_gridText.setPosition(screenPos);

            m_game->window().draw(m_gridText);
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
        originalSelectedAxialPosition = indexToAxial(m_selectedPieceIndex);
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
        size_t pieceOnClickedHexIndex = axialToIndex(mouseAxialPos);
        sf::Sprite* pieceOnClickedHex = onBoard(mouseAxialPos) ? &m_pieceSpriteBoard[pieceOnClickedHexIndex] : nullptr ;
        bool clickedOnValidPiece = pieceOnClickedHex && pieceOnClickedHex->getTexture();

        if (m_currentlyClicking)
        {
            if (m_selectedPieceSprite)
            {
                if (m_currentlyDragging)
                {
                    m_selectedPieceSprite->setPosition(absoluteMousePosition.x, absoluteMousePosition.y);
                }
                //else if (vectorHasObject(selectedPieceComponent.moveSet, mouseAxialPos) || vectorHasObject(selectedPieceComponent.takeSet, mouseAxialPos))
                // TODO: NEED A WAY TO CHECK FOR VALID MOVES
                else if (onBoard(mouseAxialPos) && mouseAxialPos != originalSelectedAxialPosition)
                {
                    movePiece(m_selectedPieceIndex, axialToIndex(mouseAxialPos));
                    m_selectedPieceSprite = nullptr;
                }
                else if (pieceOnClickedHex)
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
                            m_selectedPieceSprite = pieceOnClickedHex;
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
            else if (clickedOnValidPiece)
            {
                bool pieceOnClickedHexColor = m_chessEngine.pieceColor(m_chessEngine.getPiece(pieceOnClickedHexIndex));

                if (pieceOnClickedHexColor == m_chessEngine.whiteToMove())
                {
                    // pick up piece -> cause dragging somehow
                    pieceOnClickedHex->setPosition(absoluteMousePosition.x, absoluteMousePosition.y);
                    m_selectedPieceSprite = pieceOnClickedHex;
                    m_selectedPieceIndex = axialToIndex(mouseAxialPos);
                }
            }
        }
        else if (m_selectedPieceSprite && m_currentlyDragging)
        {
            // Drop the piece if mouse on move/take hex
            //if (vectorHasObject(selectedPieceComponent.takeSet, mouseAxialPos) || vectorHasObject(selectedPieceComponent.moveSet, mouseAxialPos))
            if (onBoard(mouseAxialPos) && mouseAxialPos != originalSelectedAxialPosition)
            {
                m_selectedPieceSprite->setScale(pieceSpriteScale, pieceSpriteScale);
                movePiece(m_selectedPieceIndex, axialToIndex(mouseAxialPos));
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

sf::Vector2f ScenePlay::indexToAxial(const size_t index) const
{
    int column = index % GRID_LENGTH;
    int row = index / GRID_LENGTH;

    int q = column + row + 1 - GRID_LENGTH;
    int r = 5 - row;

    return sf::Vector2f(q, r);
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

void ScenePlay::movePiece(size_t startIndex, size_t targetIndex)
{
    m_pieceSpriteBoard[targetIndex] = m_pieceSpriteBoard[startIndex];
    m_pieceSpriteBoard[startIndex] = sf::Sprite();

    sf::Vector2f newPos = axialToPixel(indexToAxial(targetIndex));
    m_pieceSpriteBoard[targetIndex].setPosition(newPos);

    m_chessEngine.movePiece(startIndex, targetIndex);
}
