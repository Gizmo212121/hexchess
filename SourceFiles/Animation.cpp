#include "Animation.h"
#include <memory>

Animation::Animation()
{

}

Animation::Animation(const std::string& name, const std::shared_ptr<sf::Texture> texture, const Vec2& startingFrame, const Vec2& frameSize, bool repeats)
    : Animation(name, texture, startingFrame, startingFrame, frameSize, 0, repeats) 
{
    m_sheetRows = texture->getSize().x / frameSize.getX();
    m_sheetColumns = texture->getSize().y / frameSize.getY();

    m_frameCount = 1;
}

Animation::Animation(const std::string& name, const std::shared_ptr<sf::Texture> texture, const Vec2& startingFrame, const Vec2& endingFrame, const Vec2& frameSize, size_t speed, bool repeats)
    : m_name(name)
    , m_sprite(*texture)
    , m_startingFrame(startingFrame)
    , m_endingFrame(endingFrame)
    , m_frameSize(frameSize)
    , m_currentFrame(0)
    , m_speed(speed)
    , m_repeats(repeats)
{   
    std::cout << "Animation constructor called with texture size: " << texture->getSize().x << "x" << texture->getSize().y << std::endl;

    m_sheetRows = texture->getSize().y / frameSize.getY();
    m_sheetColumns = texture->getSize().x / frameSize.getX();

    std::cout << "Rows: " << m_sheetRows << std::endl;
    std::cout << "Columns: " << m_sheetColumns << std::endl;

    Vec2 differenceInGridPositions = m_endingFrame - m_startingFrame;

    std::cout << "Difference in texture sheet positions" << differenceInGridPositions << std::endl;

    int test = differenceInGridPositions.getY() * m_sheetColumns + differenceInGridPositions.getX() + 1;

    std::cout << "This is actual m_frames: " << test << std::endl;

    m_frameCount = (size_t)(differenceInGridPositions.getY() * m_sheetColumns + differenceInGridPositions.getX() + 1);

    std::cout << "Frame count: " << m_frameCount << std::endl;

    m_sprite.setOrigin(m_frameSize.getX() / 2, m_frameSize.getY() / 2);
    m_sprite.setTexture(*texture);

    m_sprite.setTextureRect(sf::IntRect(std::floor(m_startingFrame.getX()) * m_frameSize.getX(), std::floor(m_startingFrame.getY()) * m_frameSize.getY(), m_frameSize.getX(), m_frameSize.getY()));

    std::cout << "Animation size: " << m_frameSize.getX() << "x" << m_frameSize.getY() << std::endl;
}

void Animation::update()
{
    size_t animationFrame = m_currentFrame / m_speed;

    //std::cout << "Current animation frame: " << animationFrame << std::endl;
    //std::cout << "Frame count: " << m_frameCount << std::endl;

    // Calculate the correct frame position
    size_t currentColumn = (static_cast<size_t>(m_startingFrame.getX()) + animationFrame) % m_sheetColumns;
    size_t currentRow = static_cast<size_t>(m_startingFrame.getY()) + (static_cast<size_t>(m_startingFrame.getX()) + animationFrame) / m_sheetColumns;

    sf::IntRect textureRect(currentColumn * m_frameSize.getX(), currentRow * m_frameSize.getY(), m_frameSize.getX(), m_frameSize.getY());

    //std::cout << "Animation coords: (" << textureRect.left << ", " << textureRect.top << ")" << std::endl;

    m_sprite.setTextureRect(textureRect);

    m_currentFrame++;
}

bool Animation::hasEnded() const
{
    return ((m_currentFrame / m_speed) == m_frameCount);
}
