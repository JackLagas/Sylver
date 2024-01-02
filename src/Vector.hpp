#ifndef VECTOR_H
#define VECTOR_H
struct Vec3{ 
    float x, y, z;
    Vec3();
    Vec3(float _x, double _y, double _z): x(_x), y(_y), z(_z){}
    Vec3 Normal();
};

struct Vec2{ 
    float x, y;
    Vec2();
    Vec2(float _x, double _y): x(_x), y(_y){}
    Vec3 ToVec3();
    Vec2 Normal();
};

struct Vec4{
    float w, x, y, z;
    Vec4(): w(0), x(0), y(0), z(0){}
    Vec4(float _w, float _x, float _y, float _z): w(_w), x(_x), y(_y), z(_z){}
    Vec4 Normal();
};

Vec2 operator+(Vec2& a, Vec2& b);
Vec2 operator-(Vec2& a, Vec2& b);
float operator*(Vec2& a, Vec2& b);
Vec3 operator%(Vec2& a, Vec2& b);
Vec2 operator*(float& scalar, Vec2& a);

Vec3 operator+(Vec3& a, Vec3& b);
Vec3 operator-(Vec3& a, Vec3& b);
float operator*(Vec3& a, Vec3& b);
Vec3 operator%(Vec3& a, Vec3& b);

Vec4 operator+(Vec4& a, Vec4& b);
Vec4 operator-(Vec4& a, Vec4& b);
float operator*(Vec4& a, Vec4& b);
Vec4 operator%(Vec4& a, Vec4& b);
#endif
