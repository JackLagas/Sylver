#include "Vector.hpp"
#include <cmath>

//Vec2


Vec2::Vec2(){
    x = 0;
    y = 0;
}
Vec2 Vec2::Normal(){
    float scale = 1 / sqrt(x * x + y * y);

    return Vec2(scale * x, scale * y);
}
Vec3 Vec2::ToVec3(){
    return Vec3(x, y, 0);
}
Vec2 operator+(Vec2& a, Vec2& b){
    return Vec2(a.x + b.x, a.y + b.y);
}
Vec2 operator-(Vec2& a, Vec2& b){
    return Vec2(a.x - b.x, a.y - b.y);
}
float operator*(Vec2& a, Vec2& b){
    return a.x * b.x + a.y * b.y;
}
Vec3 operator%(Vec2& a, Vec2& b){
    return Vec3(0, 0, a.x * b.y - a.y - b.x);
}
Vec2 operator*(float& scalar, Vec2& a){
    return Vec2(scalar * a.x, scalar * a.y);
}


//Vec3

Vec3::Vec3(){
    x = 0;
    y = 0;
    z = 0;
}
Vec3 Vec3::Normal(){
    float scale = 1 / sqrt(x * x + y * y + z * z);

    return Vec3(scale * x, scale * y, scale * z);
}
Vec3 operator+(Vec3& a, Vec3& b){
    return Vec3(a.x + b.x, a.y + b.y, a.z + b.z);
}
Vec3 operator-(Vec3& a, Vec3& b){
    return Vec3(a.x - b.x, a.y - b.y, a.z - b.z);
}
float operator*(Vec3& a, Vec3& b){
    return a.x * b.x + a.y * b.y + a.z * b.z;
}
Vec3 operator%(Vec3& a, Vec3& b){
    return Vec3(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y - b.x);
}

//Vec4
Vec4 Vec4::Normal(){
    float scale = 1 / sqrt(w*w + x*x + y*y + z*z);

    return Vec4( scale * w, scale * x, scale * y, scale * z);
}
Vec4 operator+(Vec4& a, Vec4& b){
    return Vec4(a.w + b.w, a.x + b.x, a.y + b.y, a.z + b.z);
}
Vec4 operator-(Vec4& a, Vec4& b){
    return Vec4(a.w - b.w, a.x - b.x, a.y - b.y, a.z - b.z);
}
float operator*(Vec4& a, Vec4& b){
    return a.w * b.w + a.x * b.x + a.y * b.y + a.z * b.z;
}


