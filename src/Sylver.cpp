#include <Sylver.hpp>
#include <map>

#include "Sprite.hpp"
#include "Logger.hpp"


std::map<int, SylverSprite> spriteMap;

static std::vector<int> sprites;

int HighestValue(std::vector<int> vec){


    if(vec.size() <= 0)
        return 1;
    

    int highest = vec[0];
    for (int i = 0; i < vec.size(); i++){
        if( vec[i] > highest)
            highest = vec[i];
    }
    return highest;
}


int Sylver::CreateSprite(const char* filePath){

    int id = HighestValue(sprites);
    INFO("Sprite with texture: {} created with id: {}", filePath, id);

    spriteMap[id] = SylverSprite(filePath);
    sprites.push_back(id);


    return id;
}
int Sylver::CreateSprite(const char* filePath, float x, float y){
    int id = CreateSprite(filePath);
    SetSpritePosition(id, x, y);
    return id;
}

int Sylver::CreateSprite(const char* filePath, float x, float y, float scale){
    int id = CreateSprite(filePath);
    SetSpritePosition(id, x, y);
    SetSpriteScale(id, scale);
    return id;
}
int Sylver::CreateSprite(const char* filePath, float x, float y, float scale, Vec2 origin){
    int id = CreateSprite(filePath);
    SetSpritePosition(id, x, y);
    SetSpriteScale(id, scale);
    SetSpriteOrigin(id, origin.x, origin.y);
    return id;
}

void Sylver::DrawSprites(){

    for(int i = 0; i < sprites.size(); i++){
        if (spriteMap.count(sprites[i] != 0)){
            spriteMap[sprites[i]].Draw();
        }
    }

}

void Sylver::SetSpritePosition(int id, float x, float y){
    spriteMap[id].SetPosition(x, y);
}
void Sylver::SetSpriteRotation(int id, float theta){
    spriteMap[id].SetRotation(theta);
}
void Sylver::SetSpriteScale(int id, float x, float y){
    spriteMap[id].SetScale(x, y);
}
void Sylver::SetSpriteScale(int id, float scale){
    spriteMap[id].SetScale(scale, scale);
}
void Sylver::SetSpriteOrigin(int id, float x, float y){
    spriteMap[id].SetOrigin(x, y);
}

Vec2 Sylver::GetSpritePosition(int id){
    return spriteMap[id].GetPosition();
}
Vec2 Sylver::GetSpriteScale(int id){
    return spriteMap[id].GetScale();
}
float Sylver::GetSpriteRotation(int id){
    return spriteMap[id].GetRotation();
}
Vec2 Sylver::GetSpriteOrigin(int id){
    return spriteMap[id].GetOrigin();
}
Vec2 Sylver::GetSpriteSize(int id){
    return spriteMap[id].GetSize();
}



