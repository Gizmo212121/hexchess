#pragma once

#include "Scene.h"
#include "EntityManager.h"
#include <memory>

class ScenePlay : public Scene
{

private:

    struct Vec2Hash
    {
        std::size_t operator()(const Vec2& vec) const
        {
            std::hash<int> intHash;
            int hq = intHash(static_cast<int>(vec.getX()));
            int hr = intHash(static_cast<int>(vec.getY()));
            return hq ^ (hr + 0x9e3779b9 + (hq << 6) + (hq >> 2));
        }
    };

    Vec2 m_bishopDiagonalPositions[4] = { Vec2(1, 1), Vec2(-1, 2), Vec2(-1, -1), Vec2(1, -2) };
    Vec2 m_queenDiagonalPositions[2] = { Vec2(-2, 1), Vec2(2, -1) };
    Vec2 m_adjacentPositions[6] = { Vec2(0, 1), Vec2(1, 0), Vec2(1, -1), Vec2(0, -1), Vec2(-1, 0), Vec2(-1, 1) };
    Vec2 m_pawnTakePositions[2] = { Vec2(-1, 1), Vec2(1, 0) };
    Vec2 m_pawnMovePositions[2] = { Vec2(0, 1), Vec2(0, 2) };
    Vec2 m_knightMovePositions[12] = { Vec2(1, 2), Vec2(2, 1), Vec2(3, -1), Vec2(3, -2), Vec2(2, -3), Vec2(1, -3), Vec2(-1, -2), Vec2(-2, -1), Vec2(-3, 1), Vec2(-3, 2), Vec2(-2, 3), Vec2(-1, 3) };

protected:

    bool m_drawTextures = true;

    bool m_drawGrid = false;
    sf::Text m_gridText;

    std::unordered_map<Vec2, std::shared_ptr<Entity>, Vec2Hash> m_grid;

    std::shared_ptr<Entity> m_player;

    void init();
    void initializeAxialCoordinateGrid();
    void initializeTiles();
    void initializePieces();

    float m_size = 121.0 / 3;
    Vec2 axialToPixel(const Vec2& vec);
    Vec2 pixelToAxial(const Vec2& vec);
    Vec2 mousePositionRelativeToWindowCenter();

    bool onBoard(const Vec2& vec);
    Vec2 getAxialFromGridPiece(const std::shared_ptr<Entity>& piece);

    bool nextTurn = true;
    void movePiece(const Vec2& targetPosition, const std::shared_ptr<Entity>& piece);

    void sUpdateMoveSets();

    void calculateBidirectionalMoveSet(const Vec2& pos, CPiece& piece, Vec2 moveDirections[], int moveDirectionsSize, bool loop);
    void calculatePawnMoveSet(const Vec2& pos, CPiece& cPiece);
    void calculateBishopMoveSet(const Vec2& pos, CPiece& cPiece);
    void calculateRookMoveSet(const Vec2& pos, CPiece& cPiece);
    void calculateLastQueenMoveSet(const Vec2& pos, CPiece& cPiece);
    void calculateKnightMoveSet(const Vec2& pos, CPiece& cPiece);
    void calculateKingMoveSet(const Vec2& pos, CPiece& cPiece);


    void onEnd() override;


public:

    ScenePlay(GameEngine* gameEngine);

    bool whiteToMove = true;

    void update() override;
    void sDoAction(const Action& action) override;
    void sMouseInput();

    void sRender() override;
};
