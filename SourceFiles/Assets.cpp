#include "Assets.h"

Assets::Assets() { }

Assets::Assets(const TextureMap& textureMap, const SoundMap& soundMap, const SoundBufferMap& soundBufferMap, const FontMap& fontMap)
    : m_textures(textureMap), m_sounds(soundMap), m_soundBuffers(soundBufferMap), m_fonts(fontMap) {}

void Assets::addTexture(const std::string& name, const std::string& path)
{
    sf::Texture texture;

    texture.setSmooth(true);

    if (!texture.loadFromFile(path))
    {
        std::cerr << "Could not load texture " << name << " from path " << path << std::endl;
        exit(1);
    }

    m_textures[name] = std::make_shared<sf::Texture>(texture);
}

void Assets::addSound(const std::string& name, const std::string& path)
{
    sf::SoundBuffer buffer;

    if (!buffer.loadFromFile(path))
    {
        std::cerr << "Could not load sound " << name << " from path " << path << std::endl;
        exit(1);
    }

    m_soundBuffers[name] = buffer;

    sf::Sound sound;

    sound.setBuffer(m_soundBuffers[name]);

    m_sounds[name] = sound;
}

void Assets::addFont(const std::string& name, const std::string& path)
{
    sf::Font font;

    if (!font.loadFromFile(path))
    {
        std::cerr << "Could not load font " << name << " from path " << path << std::endl;
        exit(1);
    }

    m_fonts[name] = font;
}

void Assets::loadFromFile(const std::string& path)
{
    std::fstream fin;

    fin.open(path);

    std::string assetType;

    while (fin >> assetType)
    {
        if (assetType == "Texture")
        {
            std::string name, assetPath;

            fin >> name >> assetPath;

            addTexture(name, assetPath);
        }
        else if (assetType == "Sound")
        {
            std::string name, assetPath;

            fin >> name >> assetPath;

            addSound(name, assetPath);
        }
        else if (assetType == "Font")
        {
            std::string name, assetPath;

            fin >> name >> assetPath;

            addFont(name, assetPath);
        }
    }

    fin.close();
}

const sf::Texture& Assets::getTexture(const std::string& name) const
{
    const auto& it = m_textures.find(name);

    if (it != m_textures.end())
    {
        return *it->second;
    }
    else
    {
        std::cerr << "Could not retrieve texture: " << name << std::endl;
        exit(1);
    }
}

const sf::SoundBuffer& Assets::getSoundBuffer(const std::string& name) const
{
    const auto& it = m_soundBuffers.find(name);

    if (it != m_soundBuffers.end())
    {
        return it->second;
    }
    else
    {
        std::cerr << "Could not retrieve sound buffer: " << name << std::endl;
        exit(1);
    }
}

const sf::Sound& Assets::getSound(const std::string& name) const
{
    const auto& it = m_sounds.find(name);

    if (it != m_sounds.end())
    {
        return it->second;
    }
    else
    {
        std::cerr << "Could not retrieve sound: " << name << std::endl;
        exit(1);
    }
}

const sf::Font& Assets::getFont(const std::string& name) const
{
    const auto& it = m_fonts.find(name);

    if (it != m_fonts.end())
    {
        return it->second;
    }
    else
    {
        std::cerr << "Could not retrieve font: " << name << std::endl;
        exit(1);
    }
}
