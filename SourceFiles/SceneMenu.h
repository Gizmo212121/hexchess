#pragma once

#include "Scene.h"
#include "EntityManager.h"

class SceneMenu : public Scene
{

protected:

    std::string m_title;
    std::vector<std::string> m_menuStrings;
    std::vector<std::string> m_levelPaths;
    sf::Text m_menuText;
    size_t m_selectedMenuIndex = 0;

    void init();
    void update() override;
    void onEnd() override;
    void sDoAction(const Action& action) override;

public:

    SceneMenu(GameEngine* gameEngine = nullptr);
    void sRender() override;
    
    // TODO: IMPLEMENT REST OF FUNCTIONALITY BELOW

};
