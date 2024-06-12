#include "Assets.h"
#include <fstream>
#include <memory>

void Assets::addTexture(std::string& name, std::string& path)
{
    sf::Texture texture;

    if (!texture.loadFromFile(path))
    {
        std::cout << "Texture at path " << path << " failed to load!\n";
        exit(1);
    }
    else
    {
        std::cout << "Texture at path " << path << " loaded successfully!\n";
    }

    m_textures[name] = std::make_shared<sf::Texture>(texture);
}

void Assets::addAnimation(std::string& name, Animation animation)
{
    m_animations[name] = std::make_shared<Animation>(animation);
}

void Assets::addSound(std::string& name, std::string& path)
{
    sf::SoundBuffer buffer;

    sf::Sound sound;

    if (!buffer.loadFromFile(path))
    {
        std::cout << "Audio file at path " << path << " failed to load!\n";
        exit(1);
    }

    sound.setBuffer(buffer);

    m_sounds[name] = sound;
}

void Assets::addFont(std::string& name, std::string& path)
{
    std::cout << "Adding font!\n";
    sf::Font font;

    if (!font.loadFromFile(path))
    {
        std::cout << "Font at path " << path << " failed to load!\n";
        std::cout << "IDIO";
        exit(1);
    }

    m_fonts[name] = font;
}

void Assets::loadFromFile(const std::string& path)
{
    std::cout << "Loading assets\n";

    std::ifstream fin;

    fin.open(path);

    std::string assetType;

    while (fin >> assetType)
    {
        if (assetType == "Texture")
        {
            std::string name, path;

            fin >> name >> path;

            addTexture(name, path);
        }
        else if (assetType == "Animation")
        {
            std::string animationName, textureName;

            float startingX, startingY, endingX, endingY, frameSizeX, frameSizeY;

            size_t speed;

            bool repeat;

            fin >> animationName >> textureName >> startingX >> startingY
                >> endingX >> endingY >> frameSizeX >> frameSizeY >> speed >> repeat;

            std::cout << "Loading animation of texture: " << textureName << std::endl;

            std::shared_ptr<sf::Texture> texture = getTexture(textureName);
            
            Animation animation(animationName, texture, Vec2(startingX, startingY), Vec2(endingX, endingY), Vec2(frameSizeX, frameSizeY), speed, repeat);

            addAnimation(animationName, animation);
        }
        else if (assetType == "Sound")
        {
            std::cout << "Sound asset not implemented\n";
        }
        else if (assetType == "Font")
        {
            std::string name, path;

            fin >> name >> path;

            addFont(name, path);
        }
    }

    fin.close();
}

const std::shared_ptr<sf::Texture> Assets::getTexture(const std::string& name) const
{
    auto it = m_textures.find(name);
    if (it == m_textures.end())
    {
        std::cerr << "Error: Texture " << name << " not found!" << std::endl;
        exit(1);
    }
    return it->second;
}

const std::shared_ptr<Animation> Assets::getAnimation(const std::string& name) const
{
    auto it = m_animations.find(name);
    if (it == m_animations.end())
    {
        std::cerr << "Error in Assets.cpp: Animation " << name << " not found!" << std::endl;
        exit(1);
    }
    return it->second;
}

const sf::Sound& Assets::getSound(const std::string& name) const
{
    auto it = m_sounds.find(name);
    if (it == m_sounds.end())
    {
        std::cerr << "Error: Sound " << name << " not found!" << std::endl;
        exit(1);
    }
    return it->second;
}

const sf::Font& Assets::getFont(const std::string& name) const
{
    auto it = m_fonts.find(name);
    if (it == m_fonts.end())
    {
        std::cerr << "Error: Font " << name << " not found!" << std::endl;
        exit(1);
    }
    return it->second;
}
