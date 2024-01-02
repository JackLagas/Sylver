#include "Entity.hpp"



Entity::Entity(){
    position = Vec2(0, 0);
    size = Vec2(0, 0);
    origin = Vec2(0, 0);
    isVisible = true;
}
Entity::Entity(Vec2 _position, Vec2 _size){
    position = _position;
    size = _size;
    origin = Vec2(0, 0);
    isVisible = true;
}


bool Entity::SetTexture(const char* filename){

    texture = LoadTexture(filename);


    hasTexture = true;

    return true;
}

void Entity::Draw(){

    if(isVisible)
        DrawTexture(texture, (int) (position.x - origin.x * size.x), (int) (position.y - origin.y * size.y), WHITE);

    
};


