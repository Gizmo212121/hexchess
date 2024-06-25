#include "GameEngine.h"
#include "ScenePlay.h"

#include <memory>

GameEngine::GameEngine() {}

GameEngine::GameEngine(const std::string& path)
{
    init(path);
}

void GameEngine::init(const std::string& path)
{
    m_assets.loadFromFile(path);

    m_window.create(sf::VideoMode(1280, 768), "HexChessMK2");

    changeScene("Game", std::make_shared<ScenePlay>(this));
}

std::shared_ptr<Scene> GameEngine::currentScene()
{
    std::map<std::string, std::shared_ptr<Scene>>::iterator it = m_sceneMap.find(m_currentScene);
    return (it != m_sceneMap.end()) ? it->second : nullptr ;
}

void GameEngine::changeScene(const std::string& sceneName, const std::shared_ptr<Scene>& scene, bool endCurrentScene)
{
    if (endCurrentScene && !m_currentScene.empty()) { m_sceneMap.erase(m_currentScene) ; }

    m_sceneMap[sceneName] = scene;

    m_currentScene = sceneName;
}

void GameEngine::run()
{
    while (isRunning())
    {
        update();
    }
}

void GameEngine::sUserInput()
{
    sf::Event event;

    while (m_window.pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            quit();
        }

        if (event.type == sf::Event::KeyPressed)
        {
            if (event.key.code == sf::Keyboard::X)
            {
                std::cout << "Screenshot saved to " << "test.png" << std::endl;

                sf::Texture texture;
                texture.create(m_window.getSize().x, m_window.getSize().y);
                texture.update(m_window);
                if (texture.copyToImage().saveToFile("test.png"))
                {
                    std::cout << "screenshot saved to " << "test.png" << std::endl;
                }
            }
        }

        if (event.type == sf::Event::KeyPressed || event.type == sf::Event::KeyReleased)
        {
            if (currentScene()->getActionMap().find(event.key.code) == currentScene()->getActionMap().end())
            {
                continue;
            }

            bool pressed = (sf::Event::KeyPressed) ? 1 : 0 ;

            currentScene()->doAction(Action(pressed, currentScene()->getActionMap().at(event.key.code)));
        }
        else if (event.type == sf::Event::MouseButtonPressed || event.type == sf::Event::MouseButtonReleased)
        {
            if (currentScene()->getActionMap().find(event.key.code + 1000) == currentScene()->getActionMap().end())
            {
                continue;
            }

            bool pressed = (sf::Event::MouseButtonPressed) ? 1 : 0 ;

            currentScene()->doAction(Action(pressed, currentScene()->getActionMap().at(event.key.code + 1000)));
        }
    }
}
