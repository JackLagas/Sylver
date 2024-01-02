#include <string>
#include <vector>

#include "../src/Vector.hpp"



class Sylver{
    private:


    public:

        static int screenWidth;
        static int screenHeight;
        static std::string appTitle;





        static int CreateSprite(const char* filePath);
        static int CreateSprite(const char* filePath, float x, float y);
        static int CreateSprite(const char* filePath, float x, float y, float scale);
        static int CreateSprite(const char* filePath, float x, float y, float scale, Vec2 origin);
        static void DrawSprites();

        static void SetSpritePosition(int id, float x, float y);
        static void SetSpriteRotation(int id, float theta);
        static void SetSpriteScale(int id, float x, float y);
        static void SetSpriteScale(int id, float scale);
        static void SetSpriteOrigin(int id, float x, float y);

        static Vec2 GetSpritePosition(int id);
        static Vec2 GetSpriteScale(int id);
        static float GetSpriteRotation(int id);
        static Vec2 GetSpriteOrigin(int id);
        static Vec2 GetSpriteSize(int id);


        static void Update();

        static void Init();




};



