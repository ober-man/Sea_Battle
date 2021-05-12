#pragma once
#include"errors.h"
class MapWidget
{
    private:
        int x;
        int y;
        int width;
        int height;
        sf::Texture texture; // texture is just a picture
        sf::Sprite spr; // sprite is a figure filled with a picture

    public:
        MapWidget(int x_, int y_, int w, int h, const char* str);
        void Draw(sf::RenderWindow& window);
};
