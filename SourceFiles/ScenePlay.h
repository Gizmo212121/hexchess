#pragma once

#include "Scene.h"
#include "EntityManager.h"
#include <memory>

class ScenePlay : public Scene
{
private:

    struct PlayerConfig
    {
        float SPAWN_X, SPAWN_Y, CX, CY, SPEED, MAXSPEED, JUMP, GRAVITY, DAMPING, FRAMES_TO_DAMAGE;
        std::string WEAPON;
    };

protected:

    std::shared_ptr<Entity> m_player;
    std::string m_levelPath;
    PlayerConfig m_playerConfig;
    bool m_drawTextures = true;
    bool m_drawCollision = false;
    bool m_drawGrid = false;
    const Vec2 m_gridSize = { 64, 64 };
    sf::Text m_gridText;

    void init(const std::string& levelPath);

    void loadLevel(const std::string& path);

    void onEnd() override;


public:

    ScenePlay(GameEngine* gameEngine, const std::string& levelPath);

    void update() override;

    void sMovement();
    void sLifespan();
    void sCollision();
    void sDoAction(const Action& action) override;
    void sAnimation();
    void sRender() override;

    Vec2 gridToMidPixelBoundingBox(float gridX, float gridY, const std::shared_ptr<Entity>& entity);
    Vec2 gridToMidPixelAnimation(float gridX, float gridY, const std::shared_ptr<Entity>& entity);
    void spawnPlayer();
    void spawnEnemy();
    void spawnSword(const std::shared_ptr<Entity>& entity);
};
