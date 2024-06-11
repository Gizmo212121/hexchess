#include "EntityManager.h"
#include <memory>

EntityManager::EntityManager()
{

}

void EntityManager::removeDeadEntities(EntityVec& vec)
{
    vec.erase(
        std::remove_if(vec.begin(), vec.end(), 
        [](const std::shared_ptr<Entity>& entity) { return !entity->isActive() ; }), 
        vec.end());
}

void EntityManager::update()
{
    removeDeadEntities(m_entities);

    for (auto& [tag, entities] : m_entityMap)
    {
        removeDeadEntities(entities);
    }

    for (std::shared_ptr<Entity> entity : m_entitiesToAdd)
    {
        m_entities.push_back(entity);

        m_entityMap[entity->tag()].push_back(entity);
    }

    m_entitiesToAdd.clear();
}

std::shared_ptr<Entity> EntityManager::addEntity(const std::string& tag)
{
    std::cout << "Adding entity of tag: " << tag << "\n";
    std::shared_ptr<Entity> entity(new Entity(m_totalEntities++, tag));

    m_entitiesToAdd.push_back(entity);

    std::cout << "Entity active?: " << entity->isActive() << std::endl;

    return entity;
}

const EntityVec& EntityManager::getEntities()
{
    return m_entities;
}

const EntityVec& EntityManager::getEntities(const std::string& tag)
{
    return m_entityMap[tag];
}
