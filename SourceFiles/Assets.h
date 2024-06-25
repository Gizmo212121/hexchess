#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <map>
#include <memory>
#include <iostream>
#include <fstream>

typedef std::unordered_map<std::string, std::shared_ptr<sf::Texture>> TextureMap;
typedef std::unordered_map<std::string, sf::Sound> SoundMap;
typedef std::unordered_map<std::string, sf::SoundBuffer> SoundBufferMap;
typedef std::unordered_map<std::string, sf::Font> FontMap;


class Assets
{
private:

    TextureMap m_textures;
    mutable SoundMap m_sounds;
    SoundBufferMap m_soundBuffers;
    FontMap m_fonts;

public:

    Assets();
    Assets(const TextureMap& textureMap, const SoundMap& soundMap, const SoundBufferMap& soundBufferMap, const FontMap& fontMap);

    void addTexture(const std::string& name, const std::string& path);
    void addSound(const std::string& name, const std::string& path);
    void addFont(const std::string& name, const std::string& path);

    void loadFromFile(const std::string& path);

    const sf::Texture& getTexure(const std::string& name) const;
    const sf::SoundBuffer& getSoundBuffer(const std::string& name) const;
    const sf::Sound& getSound(const std::string& name) const;
    const sf::Font& getFont(const std::string& name) const;

};
