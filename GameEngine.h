#pragma once

#include "Scene.h"
#include "Assets.h"

#include <memory>

typedef std::map<std::string, std::shared_ptr<Scene>> SceneMap;

class GameEngine
{

protected:
    
    sf::RenderWindow m_window;
    Assets m_assets;
    std::string m_currentScene;
    SceneMap m_sceneMap;
    size_t m_simulationSpeed = 1;
    bool m_running = true;

    void init(const std::string& path);
    void update();
    
    void sUserInput();

    std::shared_ptr<Scene> currentScene();

public:

    GameEngine(const std::string& path);

    void changeScene(const std::string& sceneName, const std::shared_ptr<Scene>& scene, bool endCurrentScene = false);

    void run();
    void quit();

    sf::RenderWindow& window() { return m_window ; }
    const Assets& assets() const { return m_assets ; }
    bool isRunning() { return m_running & m_window.isOpen() ; }
};
