#pragma once

#include "Vec2.h"
#include "Vec3.h"
#include <SFML/Graphics.hpp>
#include "SFML/Audio.hpp"
#include <memory>

typedef std::map<std::string, std::shared_ptr<sf::Texture>> TextureMap;
typedef std::map<std::string, sf::Sound> SoundMap;
typedef std::map<std::string, sf::Font> FontMap;

class Assets
{
private:
    
    TextureMap m_textures;
    SoundMap m_sounds;
    FontMap m_fonts;

public:

    Assets() {}

    Assets(TextureMap textures, SoundMap sounds, FontMap fonts)
        : m_textures(textures), m_sounds(sounds), m_fonts(fonts) {}

    void addTexture(std::string& name, std::string& path);
    void addSound(std::string& name, std::string& path);
    void addFont(std::string& name, std::string& path);

    void loadFromFile(const std::string& path);

    const std::shared_ptr<sf::Texture> getTexture(const std::string& name) const;
    const sf::Sound& getSound(const std::string& name) const;
    const sf::Font& getFont(const std::string& name) const;
};
