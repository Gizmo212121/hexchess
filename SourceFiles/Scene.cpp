#include "Scene.h"
#include "GameEngine.h"
#include <SFML/Graphics/VertexArray.hpp>

Scene::Scene()
{

}

Scene::Scene(GameEngine* gameEngine)
    : m_game(gameEngine)
{

}

void Scene::doAction(const Action& action)
{
    std::cout << "Action recieved: " << action.name() << " of type: " << action.type() << std::endl;

    sDoAction(action);
}

void Scene::simulate(const size_t frames)
{
    // TODO: Possible not what this function is supposed to be used for

    for (size_t i = 0; i < frames; i++, m_currentFrame++)
    {
        update();
    }
}

void Scene::registerKeyboardAction(int inputKey, const std::string& actionName)
{
    std::cout << "Registering action with key: " << inputKey << " and name: " << actionName << std::endl;
    m_actionMap[inputKey] = actionName;
}

void Scene::registerMouseAction(int inputKey, const std::string& actionName)
{
    std::cout << "Registering action with key: " << inputKey << " and name: " << actionName << std::endl;
    m_actionMap[inputKey + 1000] = actionName;
}

size_t Scene::height() const
{
    return m_game->window().getSize().y;
}

size_t Scene::width() const
{
    return m_game->window().getSize().x;
}

void Scene::drawLine(const Vec2& p1, const Vec2& p2)
{
    sf::VertexArray line(sf::Lines, 2);

    line[0].position = sf::Vector2f(p1.getX(), p1.getY());
    line[1].position = sf::Vector2f(p2.getX(), p2.getY());

    m_game->window().draw(line);
}




