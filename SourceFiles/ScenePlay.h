#pragma once

#include "Scene.h"
#include "ChessEngine.h"

class ScenePlay : public Scene
{

private:

    ChessEngine m_chessEngine;
    // PLAYER VARIABLES
    // Something to store a held piece, also:
    bool currentlyClicking = false;

public:

    // PUBLIC MEMBER VARIABLES

private:

    void init();

    void update() override;
    void sDoAction(const Action& action) override;
    void sRender() override;

    void onEnd() override;

public:

    ScenePlay(GameEngine* gameEngine = nullptr);

};
