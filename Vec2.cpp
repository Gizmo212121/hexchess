#include "Vec2.h"

bool Vec2::operator == (const Vec2& rhs) const
{
    if (rhs.m_x == m_x && rhs.m_y == m_y)
    {
        return true;
    }

    return false;
}

bool Vec2::operator != (const Vec2& rhs) const
{
    if (rhs.m_x == m_x && rhs.m_y == m_y)
    {
        return false;
    }

    return true;
}

Vec2 Vec2::operator + (const Vec2& rhs) const
{
    return Vec2(rhs.m_x + m_x, rhs.m_y + m_y);
}

Vec2 Vec2::operator - (const Vec2& rhs) const
{
    return Vec2(m_x - rhs.m_x, m_y - rhs.m_y);
}

Vec2 Vec2::operator / (const float val) const
{
    if (val == 0)
    {
        std::cout << "Error: div by 0!\n";
        exit(1);
    }
    else
    {
        return Vec2(m_x / val, m_y / val);
    }
}

Vec2 Vec2::operator * (const float val) const
{
    return Vec2(m_x * val, m_y * val);
}

void Vec2::operator += (const Vec2& rhs)
{
    m_x += rhs.m_x;
    m_y += rhs.m_y;
}

void Vec2::operator -= (const Vec2& rhs)
{
    m_x -= rhs.m_x;
    m_y -= rhs.m_y;
}

void Vec2::operator *= (const float val)
{
    m_x *= val;
    m_y *= val;
}

void Vec2::operator /= (const float val)
{
    if (val == 0)
    {
        std::cout << "Error: div by 0!\n";
        exit(1);
    }
    else
    {
        m_x *= val;
        m_y *= val;
    }
}

std::ostream& operator << (std::ostream& os, const Vec2& vec)
{
    os << '(' << vec.m_x << ", " << vec.m_y << ')';
    return os;
}
