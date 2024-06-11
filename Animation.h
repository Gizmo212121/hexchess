#pragma once

#include <cmath>

#include "Vec2.h"

#include <memory>
#include <vector>
#include <SFML/Graphics.hpp>

class Animation
{
private:
    std::string m_name = "default";
    sf::Sprite m_sprite;

    Vec2 m_startingFrame;
    Vec2 m_endingFrame;
    Vec2 m_frameSize;

    size_t m_sheetRows = 1;
    size_t m_sheetColumns = 1;

    size_t m_frameCount = 1; // Total frames the animation uses
    size_t m_currentFrame = 0; // Current frame the animation is on
    size_t m_speed = 0;

    bool m_repeats;

public:

    Animation();
    Animation(const std::string& name, const std::shared_ptr<sf::Texture> texture, const Vec2& startingFrame, const Vec2& frameSize, bool repeats);
    Animation(const std::string& name, const std::shared_ptr<sf::Texture> texture, const Vec2& startingFrame, const Vec2& endingFrame, const Vec2& frameSize, size_t speed, bool repeats);

    void update();
    void reset() { m_currentFrame = 0 ; }
    bool hasEnded() const;

    const std::string& getName() const { return m_name ; }
    const Vec2& getFrameSize() const { return m_frameSize ; }
    sf::Sprite& getSprite() { return m_sprite ; };
    bool repeats() { return m_repeats ; }
};
