#pragma once

#include <iostream>
#include <cstdlib>

class Vec3
{
private:
    float m_x = 0;
    float m_y = 0;
    float m_z = 0;
public:
    Vec3()
        : m_x(0), m_y(0), m_z(0) {}
    Vec3(const float x, const float y, const float z)
        : m_x(x), m_y(y), m_z(z) {}

    float getX() const { return m_x ; }
    float getY() const { return m_y ; }
    float getZ() const { return m_z ; }

    void setX(const float x) { m_x = x ; }
    void setY(const float y) { m_y = y ; }
    void setZ(const float z) { m_z = z ; }

    bool operator == (const Vec3& rhs) const;
    bool operator != (const Vec3& rhs) const;

    Vec3 operator + (const Vec3& rhs) const;
    Vec3 operator - (const Vec3& rhs) const;
    Vec3 operator / (const float val) const;
    Vec3 operator * (const float val) const;

    void operator += (const Vec3& rhs);
    void operator -= (const Vec3& rhs);
    void operator *= (const float val);
    void operator /= (const float val);

    // float dist(const Vec3& rhs) const;
    
   friend std::ostream& operator << (std::ostream& os, const Vec3& vec);
};
