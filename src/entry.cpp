#include <Sylver.hpp>
#include <raylib.h>
#include "Sprite.hpp"
#include <map>
#include "Logger.hpp"




int main(){

    
    Logger::Init("./log");

    InitWindow(Sylver::screenWidth, Sylver::screenHeight, Sylver::appTitle.c_str());
    INFO("Window initialized with dimensions {} x {} and title {}", Sylver::screenWidth, Sylver::screenHeight, Sylver::appTitle.c_str());


    SetTargetFPS(60);
    Sylver::Init();

    while(!WindowShouldClose()){
        BeginDrawing();

        ClearBackground(WHITE);



        Sylver::Update();
 
        Sylver::DrawSprites();
        

            


        EndDrawing();

    }

    CloseWindow();

    return 0;


}
