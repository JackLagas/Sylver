#ifndef ENTITY_H
#define ENTITY_H
#include "Vector.hpp"
#include <raylib.h>



class Entity{

    private:

        Vec2 position;
        Vec2 size;
        //Should be normalized
        Vec2 origin;
        bool isVisible;
        bool hasTexture = false;
        Texture2D texture;
        



    public:
    
        Entity();
        Entity(Vec2 _position, Vec2 _size);
        bool SetTexture(const char* filename);
        void Draw();


};
#endif
