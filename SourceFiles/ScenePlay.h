#pragma once

#include "Scene.h"
#include "EntityManager.h"
#include <memory>

class ScenePlay : public Scene
{

protected:

    bool m_drawTextures = true;

    bool m_drawGrid = false;
    sf::Text m_gridText;

    std::shared_ptr<Entity> m_grid[91];

    void init();

    void onEnd() override;


public:

    ScenePlay(GameEngine* gameEngine);

    void update() override;
    void sDoAction(const Action& action) override;

    void sRender() override;
};
