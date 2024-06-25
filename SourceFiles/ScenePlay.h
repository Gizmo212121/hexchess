#pragma once

#include "Scene.h"

class ScenePlay : public Scene
{

private:



public:

    ScenePlay(GameEngine* gameEngine);

    void update() override;
    void sDoAction(const Action& action) override;
    void sRender() override;

};
