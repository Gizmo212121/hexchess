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

private:

    ChessEngine m_chessEngine;

    std::array<sf::Sprite, TOTAL_PIECE_COUNT> m_pieceSpriteBoard;
    float pieceSpriteScale = 1.0 / 14;
    std::array<sf::Sprite, GRID_CELL_COUNT> m_hexSpriteBoard;
    float hexSpriteScale = 1.0 / 2.99;

    std::array<sf::Vector2f, GRID_CELL_COUNT> m_indexToAxial;

    // PLAYER VARIABLES
    // Something to store a held piece, also:
    sf::Sprite* m_selectedPieceSprite = nullptr;
    int m_selectedPieceIndex = 0;
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
    void initializeIndexToAxialArray();
    void initializeHexSpriteBoard();
    void initializePieceSpriteBoard();

    void update() override;
    void sDoAction(const Action& action) override;
    void sMouseInput();
    void sRender() override;

    void onEnd() override;


    void movePiece(const Move& move);


    sf::Vector2f axialToPixel(const sf::Vector2f& vec) const;
    sf::Vector2f pixelToAxial(const sf::Vector2f& vec) const;
    size_t axialToIndex(const sf::Vector2f& axial) const;
    bool onBoard(const sf::Vector2f& axialPos) const;

    sf::Vector2f mousePositionRelativeToWindowCenter();


public:

    ScenePlay(GameEngine* gameEngine = nullptr);

};
