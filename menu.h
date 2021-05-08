#pragma once
#include"errors.h"
#include"button.h"
#include<iostream>

using namespace sf;

class Menu
{
    private:
        Button *start,
               *settings,
               *exit;
        MapWidget *background,
                  *settings_map;
        Color main_color,
              button_color;
        void SettingsWindow(RenderWindow& window);

    public:
        Menu();
        void run(RenderWindow& window);
        ~Menu();
};

Menu::Menu()
{
    button_color = Color(126, 0, 0);
    main_color = Color(126, 181, 221);
    start = new Button("images/string_start.png", 30, 10, button_color);
    settings = new Button("images/string_settings.png", 30, 70, button_color);
    exit = new Button("images/string_exit.png", 30, 130, button_color);
    background = new MapWidget(0, 0, 1900, 900, "images/background.png");
    settings_map = new MapWidget(0, 0, 1900, 900, "images/settings.png");
}

void Menu::run(RenderWindow& window)
{
	while(window.isOpen())
    {
        start->SetColor(button_color);
        settings->SetColor(button_color);
        exit->SetColor(button_color);
        Event event;
        while(window.pollEvent(event))
            if(event.type == Event::Closed)
                window.close();
        window.clear(main_color);

        if(IntRect(30, 10, 180, 40).contains(Mouse::getPosition(window)))
        {
            start->SetColor(Color::Blue);
            if(Mouse::isButtonPressed(Mouse::Left))
                break;
        }
        if(IntRect(30, 70, 200, 40).contains(Mouse::getPosition(window)))
        {
            settings->SetColor(Color::Blue);
            if(Mouse::isButtonPressed(Mouse::Left))
                SettingsWindow(window);
        }
        if(IntRect(30, 130, 100, 40).contains(Mouse::getPosition(window)))
        {
            exit->SetColor(Color::Blue);
            if(Mouse::isButtonPressed(Mouse::Left))
                window.close();
        }

        background->Draw(window);
        start->Draw(window);
        settings->Draw(window);
        exit->Draw(window);
        window.display();
    }
}

Menu::~Menu()
{
    delete start;
    delete settings;
    delete exit;
    delete background;
    delete settings_map;
}

void Menu::SettingsWindow(RenderWindow& window)
{
    Button escape{"images/string_exit.png", 1630, 830, button_color};
    while(window.isOpen())
    {
        escape.SetColor(button_color);
        Event event;
        while(window.pollEvent(event))
            if(event.type == Event::Closed)
                window.close();
            else if(event.type == Event::KeyPressed &&
                    event.key.code == Keyboard::Escape)
                return;
        if(IntRect(1630, 830, 100, 40).contains(Mouse::getPosition(window)))
        {
            escape.SetColor(Color::Blue);
            if(Mouse::isButtonPressed(Mouse::Left))
                return;
        }
        settings_map->Draw(window);
        escape.Draw(window);
        window.display();
    }
}
