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

MapWidget::MapWidget(int x_, int y_, int w, int h, const char* str) : x(x_), y(y_), width(w), height(h)
{
    if(!texture.loadFromFile(str))
        exit(WRONG_PICTURE);
    spr.setTexture(texture);
}

void MapWidget::Draw(sf::RenderWindow& window)
{
    spr.setPosition(x, y);
    float WidgetWidth = (float) width / (float) texture.getSize().x;
    float WidgetHeight = (float) height / (float) texture.getSize().y;
    spr.setScale(WidgetWidth, WidgetHeight);
    window.draw(spr);
}
