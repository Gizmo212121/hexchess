#include "Scene.h"
#include "GameEngine.h"

Scene::Scene()
{

}

Scene::Scene(GameEngine* gameEngine)
    : m_game(gameEngine)
{

}

void Scene::doAction(const Action& action)
{
    std::string type = (action.state()) ? "Pressed " : "Released ";
    std::cout << "Action: " << type << action.name() << std::endl;

    sDoAction(action);
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
