#ifndef SPRITE_H
#define SPRITE_H

#include <raylib.h>
#include "Vector.hpp"



class SylverSprite{

    private:

        Texture2D texture;
        Vec4 textureSourcePosition;
        Vec2 position;
        Vec2 size;
        Vec2 origin;
        Vec2 scale;
        float rotation;


    public:

        SylverSprite();
        SylverSprite(const char* filename);
        
        void SetPosition(Vec2 _position);
        void SetPosition(float x, float y);
        void SetRotation(float theta);
        void SetScale(Vec2 _scale);
        void SetScale(float x, float y);
        void SetOrigin(Vec2 _origin);
        void SetOrigin(float x, float y);

        Vec2 GetPosition();
        Vec2 GetScale();
        float GetRotation();
        Vec2 GetOrigin();
        Vec2 GetSize();


        void Draw();




};
#endif
