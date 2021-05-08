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

Button::Button(const char* name, int x, int y, const sf::Color& color) : x_pos(x), y_pos(y)
{
    if(!texture.loadFromFile(name))
        exit(WRONG_PICTURE);
    spr.setTexture(texture);
    spr.setPosition(x_pos, y_pos);
    spr.setColor(color);
}

void Button::SetColor(const sf::Color& c)
{
    spr.setColor(c);
}

void Button::Draw(sf::RenderWindow& window)
{
    window.draw(spr);
}
