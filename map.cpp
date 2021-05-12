#include "map.h"
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

