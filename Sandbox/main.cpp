#include <Sylver.hpp>
int Sylver::screenHeight = 1080;
int Sylver::screenWidth = 1920;
std::string Sylver::appTitle = "My Game";

int monkey;

void Sylver::Init(){
    
    monkey = Sylver::CreateSprite("assets/monkey.png",
        Sylver::screenWidth / 2.0f, Sylver::screenHeight / 2.0f,
        0.3f, Vec2(0.5f, 0.5f)
    );
    

}


void Sylver::Update(){

    Sylver::SetSpriteRotation(monkey, Sylver::GetSpriteRotation(monkey) + 1);
    

}

