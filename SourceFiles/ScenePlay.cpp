#include "ScenePlay.h"
#include "GameEngine.h"

ScenePlay::ScenePlay(GameEngine* gameEngine)
    : Scene(gameEngine)
{
    init();
}

void ScenePlay::init()
{
    registerKeyboardAction(sf::Keyboard::Escape, "EXIT");
    registerKeyboardAction(sf::Keyboard::P, "PAUSE");
    registerMouseAction(sf::Mouse::Left, "MOUSE_LEFT");
}

void ScenePlay::update()
{
    if (m_paused)
    {
        sRender();
    }
    else
    {
        sRender();
    }
}

#pragma region systems

void ScenePlay::sDoAction(const Action& action)
{
    if (action.state() == Action::START)
    {
        if (action.name() == "EXIT") { onEnd() ; }
        else if (action.name() == "PAUSE") { setPaused() ; }
        else if (action.name() == "MOUSE_LEFT") { currentlyClicking = true ; }
    }
    else if (action.state() == Action::END)
    {
        if (action.name() == "PAUSE") { setUnpaused() ; }
        else if (action.name() == "MOUSE_LEFT") { currentlyClicking = false ; }
    }
}

void ScenePlay::sRender()
{
    if (!m_paused) { m_game->window().clear(sf::Color(100, 100, 255)) ; }
    else { m_game->window().clear(sf::Color(50, 50, 150)) ; }



    m_game->window().display();
}

#pragma endregion systems

void ScenePlay::onEnd()
{
    m_game->quit();
}
