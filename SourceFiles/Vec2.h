#pragma once

#include <iostream>
#include <cstdlib>

class Vec2
{
private:
    float m_x = 0;
    float m_y = 0;
public:
    Vec2()
        : m_x(0), m_y(0) {}
    Vec2(const float x, const float y)
        : m_x(x), m_y(y) {}

    float getX() const { return m_x ; }
    float getY() const { return m_y ; }

    void setX(const float x) { m_x = x ; }
    void setY(const float y) { m_y = y ; }

    bool operator == (const Vec2& rhs) const;
    bool operator != (const Vec2& rhs) const;

    Vec2 operator + (const Vec2& rhs) const;
    Vec2 operator - (const Vec2& rhs) const;
    Vec2 operator / (const float val) const;
    Vec2 operator * (const float val) const;

    void operator += (const Vec2& rhs);
    void operator -= (const Vec2& rhs);
    void operator *= (const float val);
    void operator /= (const float val);

    // float dist(const Vec2& rhs) const;
    
   friend std::ostream& operator << (std::ostream& os, const Vec2& vec);
};

