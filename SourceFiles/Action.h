#pragma once

#include <string>
#include <iostream>

class Action
{

private:

    bool m_state;
    std::string m_name;

public:

    enum { END = 0, START = 1 };

    Action() {}

    Action(const bool state, const std::string& name)
        : m_state(state), m_name(name) {}

    bool state() const { return m_state ; }
    const std::string& name() const { return m_name ; }
};
