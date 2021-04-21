#pragma once
#include"errors.h"
#include<iostream>

using namespace sf;

class Menu
{
    private:
        Texture string_start_texture,
                string_settings_texture,
                string_exit_texture,
                settings_texture;
        MapWidget *background,
                  *settings;
        bool status;
        void SettingsWindow(RenderWindow& window, MapWidget* settings);

    public:
        Menu();
        void run(RenderWindow& window);
        ~Menu();
};

Menu::Menu()
{
    if(!string_start_texture.loadFromFile("images/string_start.png"))
        exit(WRONG_PICTURE);
    if(!string_settings_texture.loadFromFile("images/string_settings.png"))
        exit(WRONG_PICTURE);
    if(!string_exit_texture.loadFromFile("images/string_exit.png"))
        exit(WRONG_PICTURE);
    background = new MapWidget(0, 0, 1900, 900, "images/background.png");
    settings = new MapWidget(0, 0, 1900, 900, "images/settings.png");
    status = true;
}

void Menu::run(RenderWindow& window)
{
    Sprite string_start(string_start_texture),
           string_settings(string_settings_texture),
           string_exit(string_exit_texture);

    string_start.setPosition(30, 10);
	string_settings.setPosition(30, 70);
	string_exit.setPosition(30, 130);
	while(window.isOpen() && status)
    {
        Event event;
        while(window.pollEvent(event))
            if(event.type == Event::Closed)
                window.close();

        int num = 0;
        string_start.setColor(Color(126, 0, 0));
        string_settings.setColor(Color(126, 0, 0));
        string_exit.setColor(Color(126, 0, 0));
        num = 0;
        window.clear(Color(126, 181, 221));

        if(IntRect(30, 10, 180, 40).contains(Mouse::getPosition(window)))
        {
            string_start.setColor(Color::Blue);
            num = 1;
        }
        if(IntRect(30, 70, 200, 40).contains(Mouse::getPosition(window)))
        {
            string_settings.setColor(Color::Blue);
            num = 2;
        }
        if(IntRect(30, 130, 100, 40).contains(Mouse::getPosition(window)))
        {
            string_exit.setColor(Color::Blue);
            num = 3;
        }

        if(Mouse::isButtonPressed(Mouse::Left))
        {
            if(num == 1)
                status = false;
            if(num == 2)
                SettingsWindow(window, settings);
            if(num == 3)
            {
                window.close();
                status = false;
            }
        }
        background->Draw(&window);
        window.draw(string_start);
        window.draw(string_settings);
        window.draw(string_exit);

        window.display();
    }
}

Menu::~Menu()
{
    delete background;
    delete settings;
}

void Menu::SettingsWindow(RenderWindow& window, MapWidget* settings)
{ // add a button
    while(window.isOpen())
    {
        Event event;
        while(window.pollEvent(event))
            if(event.type == Event::Closed)
                window.close();
            else if(event.type == Event::KeyPressed &&
                    event.key.code == Keyboard::Escape)
                return;
        settings->Draw(&window);
        window.display();
    }
}
