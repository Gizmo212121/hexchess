#include "Physics.h"

Vec2 Physics::GetOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    if (!a->hasComponent<CBoundingBox>() && !b->hasComponent<CBoundingBox>())
    {
        return Vec2(0.0, 0.0);
    }

    CBoundingBox& aBoundingBox = a->getComponent<CBoundingBox>();
    CBoundingBox& bBoundingBox = b->getComponent<CBoundingBox>();

    CTransform& aTransform = a->getComponent<CTransform>();
    CTransform& bTransform = b->getComponent<CTransform>();

    float aBoundHalfX = aBoundingBox.halfSize.getX();
    float aBoundHalfY = aBoundingBox.halfSize.getY();
    float bBoundHalfX = bBoundingBox.halfSize.getX();
    float bBoundHalfY = bBoundingBox.halfSize.getY();

    float dx = fabs(aTransform.pos.getX() - bTransform.pos.getX());
    float dy = fabs(aTransform.pos.getY() - bTransform.pos.getY());

    float overlapX = aBoundHalfX + bBoundHalfX - dx;
    float overlapY = aBoundHalfY + bBoundHalfY - dy;

    return Vec2(overlapX, overlapY); 
}

Vec2 Physics::GetPreviousOverlap(std::shared_ptr<Entity> a, std::shared_ptr<Entity> b)
{
    if (!a->hasComponent<CBoundingBox>() && !b->hasComponent<CBoundingBox>())
    {
        return Vec2(0.0, 0.0);
    }

    CBoundingBox& aBoundingBox = a->getComponent<CBoundingBox>();
    CBoundingBox& bBoundingBox = b->getComponent<CBoundingBox>();

    CTransform& aTransform = a->getComponent<CTransform>();
    CTransform& bTransform = b->getComponent<CTransform>();

    float aBoundHalfX = aBoundingBox.halfSize.getX();
    float aBoundHalfY = aBoundingBox.halfSize.getY();
    float bBoundHalfX = bBoundingBox.halfSize.getX();
    float bBoundHalfY = bBoundingBox.halfSize.getY();

    float dx = fabs(aTransform.prevPos.getX() - bTransform.prevPos.getX());
    float dy = fabs(aTransform.prevPos.getY() - bTransform.prevPos.getY());

    float overlapX = aBoundHalfX + bBoundHalfX - dx;
    float overlapY = aBoundHalfY + bBoundHalfY - dy;

    return Vec2(overlapX, overlapY); 
}
