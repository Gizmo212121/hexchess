#pragma once

#include "Assets.h"
#include <unordered_map>

class Entity;

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

class CSprite : public Component
{
public:
    sf::Sprite sprite;

    CSprite() {}

    CSprite(const sf::Texture& texture)
    {
        sprite.setTexture(texture);
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

class CPlayer : public Component
{
public:
    std::shared_ptr<Entity> selectedPiece = nullptr;
    bool isClicked = false;

    CPlayer() {}
    CPlayer(const std::shared_ptr<Entity>& e)
        : selectedPiece(e) {}
};
