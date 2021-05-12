#pragma once
class Button
{
    private:
        sf::Texture texture;
        sf::Sprite spr;
        int x_pos;
        int y_pos;

    public:
        Button(const char* name, int x, int y, const sf::Color& color);
        void SetColor(const sf::Color& c);
        void Draw(sf::RenderWindow& window);
};
