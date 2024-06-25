#pragma once

#include "Action.h"
#include <map>

class GameEngine;

typedef std::map<int, std::string> ActionMap;

class Scene
{

protected:

    GameEngine* m_game = nullptr;
    ActionMap m_actionMap;

    bool m_paused = false;


    virtual void onEnd();
    void setPaused(bool paused) { m_paused = paused ; }

public:

    Scene();
    Scene(GameEngine* gameEngine);

    virtual void update() = 0;
    virtual void sDoAction(const Action& action) = 0;
    virtual void sRender() = 0;

    void doAction(const Action& action);
    void registerKeyboardAction(int inputKey, const std::string& actionName);
    void registerMouseAction(int inputKey, const std::string& actionName);

    size_t width() const;
    size_t height() const;
    size_t currentFrame() const;
    const ActionMap& getActionMap() const { return m_actionMap ; }

};
