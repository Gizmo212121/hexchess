#include "ScenePlay.h"
#include "SceneMenu.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"
#include <memory>
#include <cmath>


ScenePlay::ScenePlay(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    init();
}


void ScenePlay::init()
{
    registerAction(sf::Keyboard::P, "PAUSE");
    registerAction(sf::Keyboard::Escape, "QUIT");
    registerAction(sf::Keyboard::T, "TOGGLE_TEXTURE");
    registerAction(sf::Keyboard::G, "TOGGLE_GRID");

    m_gridText.setCharacterSize(12);
    m_gridText.setFont(m_game->assets().getFont("Arial"));

    m_entityManager = EntityManager();


    initializeAxialCoordinateGrid();
    initializeTiles();
    initializePieces();

}


void ScenePlay::onEnd()
{
    m_game->window().clear(sf::Color(0, 0, 0, 255));
    m_game->changeScene("MENU", std::make_shared<SceneMenu>(m_game));
}


void ScenePlay::update()
{
    m_entityManager.update();
    //std::cout << m_entityManager.totalEntities() << std::endl;

    if (m_paused)
    {
        sRender();
    }
    else
    {
        sRender();
    }

    /*
    for (const std::shared_ptr<Entity>& entity : m_entityManager.getEntities())
    {
        if (entity->tag() == "Player")
        {
            CTransform& playerTransform = entity->getComponent<CTransform>();
            std::cout << playerTransform.pos << ", " << playerTransform.velocity << std::endl;
        }
    }
    */

    m_currentFrame++;
    std::cout << m_currentFrame << std::endl;
}


void ScenePlay::sDoAction(const Action& action)
{
    if (action.type() == "START")
    {
        if (action.name() == "TOGGLE_TEXTURE") { m_drawTextures = !m_drawTextures ; }
        else if (action.name() == "TOGGLE_GRID") { m_drawGrid = !m_drawGrid ; }
        else if (action.name() == "PAUSE") { setPaused(!m_paused) ; }
        else if (action.name() == "QUIT") { onEnd() ; }
    }
    else if (action.type() == "END")
    {
    }
}


void ScenePlay::sRender()
{
    if (!m_paused) { m_game->window().clear(sf::Color(100, 100, 255)) ; }
    else { m_game->window().clear(sf::Color(50, 50, 150)) ; }


    sf::View view = m_game->window().getView();
    m_game->window().setView(view);

    for (const std::shared_ptr<Entity>& entity : m_entityManager.getEntities("Background"))
    {
        if (entity->hasComponent<CSprite>())
        {
            sf::Sprite& backgroundSprite = entity->getComponent<CSprite>().sprite;

            float scaleX = static_cast<float>(m_game->window().getSize().x) / backgroundSprite.getTexture()->getSize().x;
            float scaleY = static_cast<float>(m_game->window().getSize().y) / backgroundSprite.getTexture()->getSize().y;

            backgroundSprite.setScale(scaleX, scaleY);
            backgroundSprite.setPosition(view.getCenter().x,
                                         view.getCenter().y);

            m_game->window().draw(backgroundSprite);
        }
    }

    if (m_drawTextures)
    {
        std::cout << "Drawing textures\n";
        for (const std::shared_ptr<Entity>& entity : m_entityManager.getEntities())
        {
            if (entity->tag() == "Background")
            {
                continue;
            }

            if (entity->hasComponent<CSprite>() && entity->hasComponent<CTransform>())
            {
                if (entity->tag() == "Tile")
                {
                    CTransform& transform = entity->getComponent<CTransform>();

                    sf::Sprite& sprite = entity->getComponent<CSprite>().sprite;

                    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
                    sprite.setRotation(transform.angle);
                    sprite.setPosition(transform.pos.getX(), transform.pos.getY());
                    sprite.setScale(transform.scale.getX() + 0.001, transform.scale.getY() + 0.001);

                    m_game->window().draw(sprite);
                }
                else
                {
                    CTransform& transform = entity->getComponent<CTransform>();

                    sf::Sprite& sprite = entity->getComponent<CSprite>().sprite;

                    sprite.setOrigin(sprite.getLocalBounds().width / 2, sprite.getLocalBounds().height / 2);
                    sprite.setRotation(transform.angle);
                    sprite.setPosition(transform.pos.getX(), transform.pos.getY());
                    sprite.setScale(transform.scale.getX(), transform.scale.getY());

                    m_game->window().draw(sprite);

                }
            }
        }
    }

    if (m_drawGrid)
    {
        for (auto it : m_grid)
        {
            std::string axialCoordinateText = '(' + std::to_string(static_cast<int>(it.first.getX())) + ", " + std::to_string(static_cast<int>(it.first.getY())) + ')';
            m_gridText.setString(axialCoordinateText);
            sf::FloatRect localBounds = m_gridText.getLocalBounds();
            m_gridText.setOrigin(localBounds.width / 2, localBounds.height / 2);

            Vec2 axialCoordinate = axialToPixel(it.first);
            m_gridText.setPosition(axialCoordinate.getX(), axialCoordinate.getY());
            m_game->window().draw(m_gridText);
        }
    }


    m_game->window().display();
}


void ScenePlay::initializeAxialCoordinateGrid()
{
    for (int q = -5; q <= 5; q++)
    {
        int r1 = std::max(-5, -q - 5);
        int r2 = std::min(5, -q + 5);
        for (int r = r1; r <= r2; r++)
        {
            m_grid[Vec2(q, r)] = nullptr;
        }
    }
}

void ScenePlay::initializePieces()
{
    std::vector<std::vector<Vec2>> pieces;

    std::vector kingPosition = {Vec2(1, -5)};
    pieces.push_back(kingPosition);

    std::vector pawnPositions =
    { 
        Vec2(-4, -1), Vec2(-3, -1), Vec2(-2, -1),
        Vec2(-1, -1), Vec2( 0, -1), Vec2( 1, -2),
        Vec2( 2, -3), Vec2( 3, -4), Vec2( 4, -5),
    };
    pieces.push_back(pawnPositions);

    std::vector knightPositions =
    {
        Vec2(-2, -3), Vec2(2, -5)
    };
    pieces.push_back(knightPositions);

    std::vector bishopPositions =
    {
        Vec2(0, -3), Vec2(0, -4), Vec2(0, -5)
    };
    pieces.push_back(bishopPositions);

    std::vector rookPositions =
    {
        Vec2(-3, -2), Vec2(3, -5)
    };
    pieces.push_back(rookPositions);

    std::vector queenPositions = {Vec2(-1, -4)};
    pieces.push_back(queenPositions);


    std::string intToType[6] = { "King", "Pawn", "Knight", "Bishop", "Rook", "Queen" };
    char color[2] = { 'b', 'w' };

    for (size_t i = 0; i < pieces.size(); i++)
    {
        for (const Vec2& axialPos : pieces[i])
        {
            for (int colorIndex = 0; colorIndex < 2; colorIndex++)
            {
                std::shared_ptr<Entity> piece = m_entityManager.addEntity("Piece");

                piece->addComponent<CSprite>(*m_game->assets().getTexture(color[colorIndex] + intToType[i]));
                piece->addComponent<CPiece>(colorIndex, i + 1);
                piece->addComponent<CState>();

                CTransform& pieceTransform = piece->addComponent<CTransform>();
                pieceTransform.scale /= 14;

                if (!colorIndex)
                {
                    pieceTransform.pos = axialToPixel(axialPos);
                    m_grid[axialPos] = piece;
                }
                else
                {
                    Vec2 blackAxialPos = axialPos * -1;
                    pieceTransform.pos = axialToPixel(blackAxialPos);
                    m_grid[blackAxialPos] = piece;
                }
            }
        }
    }
}

Vec2 ScenePlay::axialToPixel(const Vec2& vec)
{
    float x = m_size * (3.0 / 2 * vec.getX());
    float y = m_size * (sqrt(3) / 2 * vec.getX() + sqrt(3) * vec.getY());

    float viewportXHalf = m_game->window().getSize().x / 2.0;
    float viewportYHalf = m_game->window().getSize().y / 2.0;

    return Vec2(viewportXHalf + x, viewportYHalf - y);
}

void ScenePlay::initializeTiles()
{
    for (auto it : m_grid)
    {
        std::shared_ptr<Entity> hex = m_entityManager.addEntity("Tile");

        std::string hexAssetName;

        int code = (int)it.first.getX() - (int)it.first.getY();

        if (code % 3 == 0) {  hexAssetName = "Hex1"; }
        else if (code % 3 == -1) { hexAssetName = "Hex2"; }
        else if (code % 3 == 2) { hexAssetName = "Hex2"; }
        else { hexAssetName = "Hex0"; }

        hex->addComponent<CSprite>(*m_game->assets().getTexture(hexAssetName));
        CTransform& transform = hex->addComponent<CTransform>();
        transform.pos = axialToPixel(it.first);
        transform.scale /= 3;

    }
}
