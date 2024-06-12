#pragma once

#include <sstream>

class Action
{
private:

    std::string m_name = "default";
    std::string m_type = "default";

public:

    Action() {}

    Action(const std::string& name, const std::string& type)
        : m_name(name), m_type(type) {}

    const std::string& name() const { return m_name ; }
    const std::string& type() const { return m_type ; }
};
