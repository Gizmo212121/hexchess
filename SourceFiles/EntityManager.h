#pragma once

#include "Entity.h"
#include <memory>

typedef std::vector<std::shared_ptr<Entity>> EntityVec;

class EntityManager
{
private:
    EntityVec m_entities;
    EntityVec m_entitiesToAdd;
    std::map<std::string, EntityVec> m_entityMap;
    size_t m_totalEntities = 0;

    void removeDeadEntities(EntityVec& vec);

public:

    EntityManager();

    void update();

    std::shared_ptr<Entity> addEntity(const std::string& tag);

    const EntityVec& getEntities();
    const EntityVec& getEntities(const std::string& tag);

    size_t totalEntities() const { return m_totalEntities ; }
};
