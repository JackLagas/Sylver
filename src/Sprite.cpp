#include "Sprite.hpp"
#include "Logger.hpp"


SylverSprite::SylverSprite(const char* filename){

    texture = LoadTexture(filename);
    if(texture.id <= 0){
        WARN("Texture {} failed to load", filename);
    }
    textureSourcePosition = Vec4(0.0f, 0.0f, texture.width, texture.height);
    position = Vec2(0.0f, 0.0f);
    size = Vec2(texture.width, texture.height);
    origin = Vec2(0.0f, 0.0f);
    scale = Vec2(1.0f, 1.0f);
    rotation = 0.0f;
}

SylverSprite::SylverSprite(){

    
}

void SylverSprite::Draw(){

    DrawTexturePro(texture,
        Rectangle{textureSourcePosition.w, textureSourcePosition.x, textureSourcePosition.y, textureSourcePosition.z},
        Rectangle{position.x, position.y, scale.x * size.x, scale.y * size.y},
        Vector2{origin.x * size.x * scale.x, origin.y * size.y * scale.y},
        rotation,
        WHITE
        );

    

}


// Setters
void SylverSprite::SetPosition(Vec2 _position){
    position = _position;
}
void SylverSprite::SetPosition(float x, float y){
    SetPosition(Vec2(x, y));
}
void SylverSprite::SetRotation(float theta){
    rotation = theta;
}
void SylverSprite::SetScale(Vec2 _scale){
    scale = _scale;
}
void SylverSprite::SetScale(float x, float y){
    SetScale(Vec2(x, y));
}
void SylverSprite::SetOrigin(Vec2 _origin){
    origin = _origin;
}
void SylverSprite::SetOrigin(float x, float y){
    SetOrigin(Vec2(x, y));
}


//Getters
Vec2 SylverSprite::GetPosition(){
    return position;
}
Vec2 SylverSprite::GetScale(){
    return scale;
}
float SylverSprite::GetRotation(){
    return rotation;
}
Vec2 SylverSprite::GetOrigin(){
    return origin;
}
Vec2 SylverSprite::GetSize(){
    return Vec2(scale.x * size.x, scale.y * size.y);
}



