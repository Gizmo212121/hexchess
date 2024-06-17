#include "SceneMenu.h"
#include "ScenePlay.h"
#include "GameEngine.h"

SceneMenu::SceneMenu(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    init();
}

void SceneMenu::init()
{
    registerKeyboardAction(sf::Keyboard::Escape, "QUIT");
    registerKeyboardAction(sf::Keyboard::B, "START_LEVEL");
}

void SceneMenu::update()
{
    m_entityManager.update();
    sRender();
}

void SceneMenu::onEnd()
{
    m_game->quit();
}

void SceneMenu::sDoAction(const Action& action)
{
    if (action.type() == "START")
    {
        if (action.name() == "QUIT") { onEnd() ; }
        else if (action.name() == "START_LEVEL") { m_game->changeScene("PLAY", std::make_shared<ScenePlay>(m_game)) ; }
    }
    else if (action.type() == "END")
    {

    }
}

void SceneMenu::sRender()
{

}
