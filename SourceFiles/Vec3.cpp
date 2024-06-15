#include "Vec3.h"

bool Vec3::operator == (const Vec3& rhs) const
{
    if (rhs.m_x == m_x && rhs.m_y == m_y && rhs.m_z == m_z)
    {
        return true;
    }

    return false;
}

bool Vec3::operator != (const Vec3& rhs) const
{
    if (rhs.m_x == m_x && rhs.m_y == m_y && rhs.m_z == m_z)
    {
        return false;
    }

    return true;
}

Vec3 Vec3::operator + (const Vec3& rhs) const
{
    return Vec3(rhs.m_x + m_x, rhs.m_y + m_y, rhs.m_z + m_z);
}

Vec3 Vec3::operator - (const Vec3& rhs) const
{
    return Vec3(m_x - rhs.m_x, m_y - rhs.m_y, m_z - rhs.m_z);
}

Vec3 Vec3::operator / (const float val) const
{
    if (val == 0)
    {
        std::cout << "Error: div by 0!\n";
        exit(1);
    }
    else
    {
        return Vec3(m_x / val, m_y / val, m_z / val);
    }
}

Vec3 Vec3::operator * (const float val) const
{
    return Vec3(m_x * val, m_y * val, m_z * val);
}

void Vec3::operator += (const Vec3& rhs)
{
    m_x += rhs.m_x;
    m_y += rhs.m_y;
    m_z += rhs.m_z;
}

void Vec3::operator -= (const Vec3& rhs)
{
    m_x -= rhs.m_x;
    m_y -= rhs.m_y;
    m_z -= rhs.m_z;
}

void Vec3::operator *= (const float val)
{
    m_x *= val;
    m_y *= val;
    m_z *= val;
}

void Vec3::operator /= (const float val)
{
    if (val == 0)
    {
        std::cout << "Error: div by 0!\n";
        exit(1);
    }
    else
    {
        m_x /= val;
        m_y /= val;
        m_z /= val;
    }
}

std::ostream& operator << (std::ostream& os, const Vec3& vec)
{
    os << '(' << vec.m_x << ", " << vec.m_y << ", " << vec.m_z << ')';
    return os;
}
