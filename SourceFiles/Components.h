#pragma once

#include "Animation.h"
#include "Assets.h"
#include <unordered_map>

class Component
{
public:
    bool has = false;
};

class CTransform : public Component
{
public:
    Vec2 pos = { 0.0, 0.0 };
    Vec2 scale = { 1.0, 1.0 };
    float angle = 0;

    CTransform() {}
    CTransform(const Vec2& p)
        : pos(p) {}
    CTransform(const Vec2& p, const Vec2& sc, float a)
        : pos(p), scale(sc), angle(a) {}
};

class CAxialPos : public Component
{
public:
    Vec2 pos = { 0.0, 0.0 };
    Vec2 prevPos = pos;

    CAxialPos() {}
    CAxialPos(const Vec2& p)
        : pos(p), prevPos(p) {}
};

class CPiece : public Component
{
public:
    bool color = 0;

    // Types:
    //  None = 0
    //  King = 1
    //  Pawn = 2
    //  Knight = 3
    //  Bishop = 4
    //  Rook = 5
    //  Queen = 6
    int type = 0;

    std::vector<Vec2> moveSet;

    CPiece() {}
    CPiece(const bool color, const int type)
        : color(color), type(type) {}
};

class CAnimation : public Component
{
public:
    std::unordered_map<std::string, Animation> animations;
    std::string currentAnimation;

    CAnimation() {}

    CAnimation(const std::unordered_map<std::string, Animation>& anims, const std::string& currentAnim)
        : animations(anims), currentAnimation(currentAnim)
    {
    }

    CAnimation(std::string animationName, const std::shared_ptr<Animation>& animation)
    {
        animations[animationName] = *animation;
        currentAnimation = animationName;
    }
};

class CState : public Component
{
public:
    bool isLifted = false;
    
    CState() {}
    CState(const bool lift)
        : isLifted(lift) {}
};
