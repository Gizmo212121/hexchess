#include "ScenePlay.h"
#include "SceneMenu.h"
#include "Assets.h"
#include "GameEngine.h"
#include "Components.h"
#include "Action.h"
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Window/Joystick.hpp>
#include <fstream>
#include <memory>
#include <unordered_map>


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
        if (entity->hasComponent<CAnimation>())
        {
            sf::Sprite& backgroundSprite = entity->getComponent<CAnimation>().animations.at(entity->getComponent<CAnimation>().currentAnimation).getSprite();

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

            if (entity->hasComponent<CAnimation>())
            {
                CTransform& transform = entity->getComponent<CTransform>();
                std::string currentAnimation = entity->getComponent<CAnimation>().currentAnimation;

                auto animationIterator = entity->getComponent<CAnimation>().animations.find(currentAnimation);

                if (animationIterator != entity->getComponent<CAnimation>().animations.end())
                {
                    Animation& animation = animationIterator->second;
                    sf::Sprite& sprite = animation.getSprite();

                    sprite.setRotation(transform.angle);
                    sprite.setPosition(transform.pos.getX(), transform.pos.getY());
                    sprite.setScale(transform.scale.getX(), transform.scale.getY());

                    m_game->window().draw(sprite);
                }
                else
                {
                    std::cerr << "Warning: Animation '" << currentAnimation << "'not found for entity: " << entity->tag();
                }
            }
        }
    }


    // EHHHHH IDK YET ABOUT THIS 
    /*
    if (m_drawGrid)
    {
        float leftX = m_game->window().getView().getCenter().x - static_cast<float>(width()) / 2;
        float rightX = leftX + width() + m_gridSize.getX();
        float nextGridX = leftX - ((int)leftX % (int)m_gridSize.getX());

        for (float x = nextGridX; x < rightX; x += m_gridSize.getX())
        {
            drawLine(Vec2(x, 0), Vec2(x, height()));
        }

        for (float y = 0; y < height(); y += m_gridSize.getY())
        {
            drawLine(Vec2(leftX, height() - y), Vec2(rightX, height() - y));

            for (float x = nextGridX; x < rightX; x += m_gridSize.getX())
            {
                std::string xCell = std::to_string((int)x / (int)m_gridSize.getX());
                std::string yCell = std::to_string((int)y / (int)m_gridSize.getY());

                m_gridText.setString("(" + xCell + "," + yCell + ")");
                m_gridText.setPosition(x + 3, height() - y - m_gridSize.getY() + 2);

                m_game->window().draw(m_gridText);
            }
        }
    }
    */

    m_game->window().display();
}
