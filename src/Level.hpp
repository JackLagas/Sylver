#ifndef LEVEL_H
#define LEVEL_H


#include <vector>
#include <string>


class Level{

    private:

        std::vector<int> levelMap;
        int lvlWidth;
        std::string tileMap;
        int tileSize;



    public:



        Level();
        Level(const char* filePath);

        void LoadFromFile(const char* filePath);
    

    


};
#endif
