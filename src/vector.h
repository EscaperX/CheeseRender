#ifndef CHEESE_VECTOR_H
#define CHEESE_VECTOR_H

#include <assert.h>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <optional>

class Vector3f
{
public:
    float x, y, z;
    Vector3f() : x(0), y(0), z(0) {}
    Vector3f(float xx) : x(xx), y(xx), z(xx) {}
    Vector3f(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
    Vector3f operator*(const float &r) const { return Vector3f(x * r, y * r, z * r); }
    Vector3f operator/(const float &r) const { return Vector3f(x / r, y / r, z / r); }

    float norm() { return std::sqrt(x * x + y * y + z * z); }
    Vector3f normalized() const
    {
        float n = std::sqrt(x * x + y * y + z * z);
        return Vector3f(x / n, y / n, z / n);
    }

    Vector3f operator*(const Vector3f &v) const { return Vector3f(x * v.x, y * v.y, z * v.z); }
    Vector3f operator-(const Vector3f &v) const { return Vector3f(x - v.x, y - v.y, z - v.z); }
    Vector3f operator+(const Vector3f &v) const { return Vector3f(x + v.x, y + v.y, z + v.z); }
    Vector3f operator-() const { return Vector3f(-x, -y, -z); }
    Vector3f &operator+=(const Vector3f &v)
    {
        x += v.x, y += v.y, z += v.z;
        return *this;
    }
    friend Vector3f operator*(const float &r, const Vector3f &v)
    {
        return Vector3f(v.x * r, v.y * r, v.z * r);
    }
    friend std::ostream &operator<<(std::ostream &os, const Vector3f &v)
    {
        return os << v.x << ", " << v.y << ", " << v.z;
    }
    float operator[](int index) const;
    float &operator[](int index);

    static Vector3f Min(const Vector3f &p1, const Vector3f &p2)
    {
        return Vector3f(std::min(p1.x, p2.x), std::min(p1.y, p2.y),
                        std::min(p1.z, p2.z));
    }

    static Vector3f Max(const Vector3f &p1, const Vector3f &p2)
    {
        return Vector3f(std::max(p1.x, p2.x), std::max(p1.y, p2.y),
                        std::max(p1.z, p2.z));
    }
};
inline float Vector3f::operator[](int index) const
{
    // assert(index < 3);
    return (&x)[index];
}
inline float &Vector3f::operator[](int index)
{
    // assert(index < 3);
    return (&x)[index];
}

class Vector2f
{
public:
    Vector2f() : x(0), y(0) {}
    Vector2f(float xx) : x(xx), y(xx) {}
    Vector2f(float xx, float yy) : x(xx), y(yy) {}
    Vector2f operator*(const float &r) const { return Vector2f(x * r, y * r); }
    Vector2f operator+(const Vector2f &v) const { return Vector2f(x + v.x, y + v.y); }
    float x, y;
};

inline Vector3f lerp(const Vector3f &a, const Vector3f &b, const float &t)
{
    return a * (1 - t) + b * t;
}

inline Vector3f normalize(const Vector3f &v)
{
    float mag2 = v.x * v.x + v.y * v.y + v.z * v.z;
    if (mag2 > 0)
    {
        float invMag = 1 / sqrtf(mag2);
        return Vector3f(v.x * invMag, v.y * invMag, v.z * invMag);
    }

    return v;
}

inline float dotProduct(const Vector3f &a, const Vector3f &b)
{
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

inline Vector3f crossProduct(const Vector3f &a, const Vector3f &b)
{
    return Vector3f(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x);
}

class Vector2i
{
public:
    Vector2i() : x(0), y(0) {}
    Vector2i(int xx) : x(xx), y(xx) {}
    Vector2i(int xx, int yy) : x(xx), y(yy) {}
    Vector2i operator*(const int &r) const { return Vector2i(x * r, y * r); }
    Vector2i operator+(const Vector2i &v) const { return Vector2i(x + v.x, y + v.y); }
    int x, y;
};

class Vector3i
{
public:
    Vector3i() : x(0), y(0), z(0) {}
    Vector3i(int xx) : x(xx), y(xx), z(xx) {}
    Vector3i(int xx, int yy, int zz) : x(xx), y(yy), z(zz) {}
    Vector3i operator*(const int &r) const { return Vector3i(x * r, y * r, z * r); }
    Vector3i operator+(const Vector3i &v) const { return Vector3i(x + v.x, y + v.y, z + v.z); }
    int operator[](int index) const;
    int &operator[](int index);
    int x, y, z;
};
inline int Vector3i::operator[](int index) const
{
    // assert(index < 3);
    return (&x)[index];
}
inline int &Vector3i::operator[](int index)
{
    // assert(index < 3);
    return (&x)[index];
}
inline Vector2f interp_texcoord(float u, float v, float w, const Vector2f &tex0, const Vector2f &tex1, const Vector2f &tex2)
{
    Vector2f st = tex1 * u + tex2 * v + tex0 * w;
    return st;
}
inline Vector3f interp_normal(float u, float v, float w, const Vector3f& tex0, const Vector3f& tex1, const Vector3f& tex2)
{
    Vector3f st = tex1 * u + tex2 * v + tex0 * w;
    return st.normalized();
}

#endif
