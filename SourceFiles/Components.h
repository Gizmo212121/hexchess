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
    Vec2 prevPos = pos;
    Vec2 velocity = { 0.0, 0.0 };
    Vec2 scale = { 1.0, 1.0 };
    float angle = 0;

    CTransform() {}
    CTransform(const Vec2& p)
        : pos(p) {}
    CTransform(const Vec2& p, const Vec2& sp, const Vec2& sc, float a)
        : pos(p), prevPos(p), velocity(sp), scale(sc), angle(a) {}
};

class CLifespan : public Component
{
public:
    int lifespan = 0;
    int frameCreated = 0;

    CLifespan()
        : lifespan(0), frameCreated(0) {}
    CLifespan(const int lifespan, const int frame)
        : lifespan(lifespan), frameCreated(frame) {}
};

class CDamage : public Component
{
public:
    int damage = 0;
    int frameCreated = 0;
    int framesToDamage = 0;

    CDamage()
        : damage(0), frameCreated(0), framesToDamage(0) {}

    CDamage(const int d, const int fc, const int fd)
        : damage(d), frameCreated(fc), framesToDamage(fd) {}
};

class CHealth : public Component
{
public:
    int health = 1;
    
    CHealth()
        : health(1) {}
    CHealth(const int h)
        : health(h) {}
};

class CInput : public Component
{
public:
    bool up = false;
    bool down = false;
    bool left = false;
    bool right = false;
    bool swingSword = false;
    bool canSwingSword = true;
    bool canJump = true;
    bool canSlide = true;

    CInput() {}
};

class CBoundingBox : public Component
{
public:
    Vec2 size;
    Vec2 halfSize;

    CBoundingBox() {}
    CBoundingBox(const Vec2& s)
        : size(s), halfSize(s / 2) { has = true ; }
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

class CGravity : public Component
{
public:
    float gravity = 0;

    CGravity() {}
    CGravity(const float gravity)
        : gravity(gravity) {}
};

class CState : public Component
{
public:
    std::string state = "Idle";
    bool flipped = false;
    bool midAir = false;
    size_t jumpTimer = 0;
    size_t slideTimer = 0;
    size_t swingSwordTimer = 0;
    
    CState() {}
    CState(const std::string& state)
        : state(state) {}
};
