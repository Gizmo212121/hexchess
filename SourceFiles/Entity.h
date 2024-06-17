#pragma once

#include "Components.h"

#include <tuple>

class EntityManager;

typedef std::tuple<
    CTransform,
    CPiece,
    CState,
    CSprite,
    CPlayer
> ComponentTuple;

class Entity
{
private:
    friend class EntityManager;

    bool m_active = true;
    std::string m_tag = "default";
    size_t m_id = 0;
    ComponentTuple m_components;

    // Private constructor so Entitmanager does the spawning
    Entity(const size_t& id, const std::string& tag)
        : m_tag(tag), m_id(id)
    {
    }
    
public:

    void destroy() { m_active = false ; }
    size_t id() const { return m_id ; }
    bool isActive() const { return m_active ; }
    const std::string& tag() const { return m_tag ; }

    template <typename T>
    bool hasComponent() const
    {
        return getComponent<T>().has;
    }

    template <typename T, typename... TArgs>
    T& addComponent(TArgs&&... mArgs)
    {
        auto& component = getComponent<T>();
        component = T(std::forward<TArgs>(mArgs)...);
        component.has = true;
        return component;
    }

    template <typename T>
    T& getComponent()
    {
        return std::get<T>(m_components);
    }

    template <typename T>
    const T& getComponent() const
    {
        return std::get<T>(m_components);
    }

    template <typename T>
    void removeComponent()
    {
        getComponent<T>() = T();
    }
};
