#pragma once

#include "Scene.h"
#include "ChessEngine.h"

#include <cmath>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

class ScenePlay : public Scene
{

private:
    // DEBUG STUFF
    bool profileUpdateFunction = false;

    ChessEngine m_chessEngine;

    std::array<sf::Sprite, GRID_HEX_COUNT> m_pieceSpriteBoard;
    float pieceSpriteScale = 1.0 / 14;
    std::array<sf::Sprite, GRID_HEX_COUNT> m_hexSpriteBoard;
    float hexSpriteScale = 1.0 / 2.99;
    // PLAYER VARIABLES
    // Something to store a held piece, also:
    sf::Sprite* m_selectedPieceSprite = nullptr;
    size_t m_selectedPieceIndex = 0;
    bool m_currentlyClicking = false;
    bool m_currentlyDragging = false;

    bool m_renderTextures = true;
    bool m_renderGridValues = false;

    float m_hexTextureSize = 121.0 / 3;

    sf::Text m_gridText;

public:

    // PUBLIC MEMBER VARIABLES

private:

    void init();
    void initializeHexSpriteBoard();
    void initializePieceSpriteBoard();

    void update() override;
    void sDoAction(const Action& action) override;
    void sMouseInput();
    void sRender() override;

    void onEnd() override;


    void movePiece(size_t startIndex, size_t targetIndex);


    sf::Vector2f axialToPixel(const sf::Vector2f& vec) const;
    sf::Vector2f pixelToAxial(const sf::Vector2f& vec) const;
    sf::Vector2f indexToAxial(const size_t index) const;
    size_t axialToIndex(const sf::Vector2f& axial) const;
    bool onBoard(const sf::Vector2f& axialPos) const;

    sf::Vector2f mousePositionRelativeToWindowCenter();


public:

    ScenePlay(GameEngine* gameEngine = nullptr);

};
