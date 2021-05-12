#include "Application.h"

Application::Application()
{
    window = new sf::RenderWindow(sf::VideoMode(1900, 900), "Sea Battle");
    game = new MapWidget(0, 0, 1900, 900, "images/table.jpg");
    button_color = Color(126, 0, 0);
    main_color = Color(126, 181, 221);
    random = new Button("images/random.png", 1450, 750, button_color);
    lose = new Button("images/lose.png", 800, 300, button_color);
    win = new Button("images/win.png", 800, 300, button_color);
}

Application::~Application()
{
    delete window;
    delete game;
    delete random;
    delete man;
}

void Application::Run()
{
	// create music
    Music pirate_music;
	if (!pirate_music.openFromFile("music/pirate.wav"))
		exit(WRONG_MUSIC);
	pirate_music.setLoop(true);
	pirate_music.play();

	// create pictures
    Texture cell;
    if(!cell.loadFromFile("images/cell.png"))
        exit(WRONG_PICTURE);
    Sprite s(cell);

    Texture sh;
    if(!sh.loadFromFile("images/ships.jpg"))
        exit(WRONG_PICTURE);
    Sprite ships(sh);

    int mode = menu.run(*window); // return value = game mode
    if(mode == SUPER_FAN)
    {
        LSIZE = 22;
        SIZE = 21;
    }
    man = new Player;
	
	//main loop
    while(window->isOpen())
    {
		// get coordinates of mouse
        Vector2i pos = Mouse::getPosition(*window);
	    
		// and adapt them to board's size
        int x_my = (pos.x - x_0) / len;
        int y_my = (pos.y - y_0) / len;
        if(mode == SUPER_FAN)
        {
            x_my = (pos.x - x_0_fan) / len;
            y_my = (pos.y - y_0_fan) / len;
        }

        sf::Event event;
        while(window->pollEvent(event))
        {
			// if window is closed
            if(event.type == sf::Event::Closed)
                window->close();
			
			// if cell is pressed
            if(event.type == Event::MouseButtonPressed)
                if(event.key.code == Mouse::Left)
                    if(x_my > 0 && y_my > 0 && x_my < SIZE && y_my < SIZE)
                    {
                        if(man->Myboard->Get(x_my, y_my) == SHIP)
                            man->Myboard->Set(x_my, y_my, 0);
                        else
                            man->Myboard->Set(x_my, y_my, SHIP);
                    }
			
			// if user wants to make ship coordinates by his own
            if(event.type == Event::KeyPressed && event.key.code == Keyboard::Enter)
                if(man->check_ship_number() == 10)
                {
                    man->Enemyboard->random_ships();
                    StartGame(s, mode);
                }
        }

        random->SetColor(button_color);
        window->clear(main_color);

		// random making ship coordinates
        if(IntRect(1450, 750, 450, 160).contains(Mouse::getPosition(*window)))
        {
            random->SetColor(Color::Blue);
            if(Mouse::isButtonPressed(Mouse::Left))
            {
                man->random_position();
                StartGame(s, mode);
            }
        }
		
		// draw a window and some pictures
        game->Draw(*window);
        DrawTable(s, mode);
        ships.setPosition(1200, y_2);
        window->draw(ships);
        random->Draw(*window);
        window->display();
    }
}

void Application::StartGame(Sprite s, int mode)
{
	// variable status is a current status of a game
	// if status > 0 then it is user's turn
	// else computer's
    int status = 10;

    while(window->isOpen())
    {
        if(status > 0)
        {
			// get coordinates of mouse
            Vector2i pos = Mouse::getPosition(*window);
            int x_my = (pos.x - x_0) / len;
            int y_my = (pos.y - y_0) / len;
            if(mode == SUPER_FAN)
            {
                x_my = (pos.x - x_0_fan) / len;
                y_my = (pos.y - y_0_fan) / len;
            }
            coords XY_my = {x_my, y_my};

            Event event;
            while(window->pollEvent(event))
            {
				// if window is closed
                if(event.type == Event::Closed)
                    window->close();
				
				// if a cell is pressed
                if(event.type == Event::MouseButtonPressed && status != PLAYER_WIN)
                    if(event.key.code == Mouse::Left)
                        if(x_my > 0 && y_my > 0 && x_my < SIZE && y_my < SIZE)
                        {
							// change status to know whose turn is now
                            if(mode == SUPER_FAN)
                                status = man->extra_turn(XY_my);
                            else
                                status = man->turn(XY_my);
                        }
            }
			
			// draw a window and pictures
            game->Draw(*window);
            DrawBoards(s, mode);
			
			// if user win - draw it
            if(status == PLAYER_WIN)
                win->Draw(*window);
            window->display();
        }
        else // status < 0
        {
            if(status != PLAYER_LOSE)
            {
				// change status to know whose turn is now
                if(mode == SUPER_FAN)
                    status = man->extra_enemy_turn();
                else
                    status = man->enemy_turn();
            }

            Event event;
            while(window->pollEvent(event))
                if(event.type == Event::Closed)
                    window->close();

			// draw a window and pictures
            game->Draw(*window);
            DrawBoards(s, mode);
			
			// if computer win - draw it
            if(status == PLAYER_LOSE)
                lose->Draw(*window);
            window->display();
        }
    }
}

void Application::DrawBoards(Sprite s, int mode)
{
    // draw upper left corners
    s.setTextureRect(IntRect(len, 0, len, len));
    s.setPosition(x_0, y_0);
    if(mode == SUPER_FAN)
        s.setPosition(x_0_fan, y_0_fan);
    window->draw(s);
    s.setPosition(x_2, y_2);
    if(mode == SUPER_FAN)
        s.setPosition(x_2_fan, y_2_fan);
    window->draw(s);

	// draw borders
    for(int i = 1; i < SIZE; i++)
    {
        s.setTextureRect(IntRect((i + 3) * len, 0, len, len));
        s.setPosition(i*len + x_0, y_0);
        if(mode == SUPER_FAN)
            s.setPosition(i*len + x_0_fan, y_0_fan);
        window->draw(s);
        s.setPosition(i*len + x_2, y_2);
        if(mode == SUPER_FAN)
            s.setPosition(i*len + x_2_fan, y_2_fan);
        window->draw(s);
        s.setPosition(x_0, i*len + y_0);
        if(mode == SUPER_FAN)
            s.setPosition(x_0_fan, i*len + y_0_fan);
        window->draw(s);
        s.setPosition(x_2, i*len + y_2);
        if(mode == SUPER_FAN)
            s.setPosition(x_2_fan, i*len + y_2_fan);
        window->draw(s);
    }

	// draw main part of boards:
	// board to that user shoot
    for(int i = 1; i < LSIZE; i++)
        for(int j = 1; j < LSIZE; j++)
        {
            if(man->Hitboard->Get(i, j) == POISON)
                continue;
            else if(man->Hitboard->Get(i, j) == HIT_SHIP)
                s.setTextureRect(IntRect(2 * len, 0, len, len));
            else if(man->Hitboard->Get(i, j) == HIT_MISS)
                s.setTextureRect(IntRect(3 * len, 0, len, len));
            else
                s.setTextureRect(IntRect(1 * len, 0, len, len));
            s.setPosition(i*len + x_0, j*len + y_0);
            if(mode == SUPER_FAN)
                s.setPosition(i*len + x_0_fan, j*len + y_0_fan);
            window->draw(s);
        }
	
	// board to that computer shoot
    for(int i = 1; i < LSIZE; i++)
        for(int j = 1; j < LSIZE; j++)
        {
            if(man->HitEnemyboard->Get(i, j) == POISON)
                continue;
            else if(man->HitEnemyboard->Get(i, j) == HIT_SHIP)
                s.setTextureRect(IntRect(2 * len, 0, len, len));
            else if(man->HitEnemyboard->Get(i, j) == HIT_MISS)
                s.setTextureRect(IntRect(3 * len, 0, len, len));
            else
                s.setTextureRect(IntRect(1 * len, 0, len, len));
            s.setPosition(i*len + x_2, j*len + y_2);
            if(mode == SUPER_FAN)
                s.setPosition(i*len + x_2_fan, j*len + y_2_fan);
            window->draw(s);
        }

}
void Application::DrawTable(Sprite s, int mode)
{
    // draw upper left corner
    s.setTextureRect(IntRect(len, 0, len, len));
    s.setPosition(x_0, y_0);
    if(mode == SUPER_FAN)
        s.setPosition(x_0_fan, y_0_fan);
    window->draw(s);

	// draw borders 
    for(int i = 1; i < SIZE; i++)
    {
        s.setTextureRect(IntRect((i + 3) * len, 0, len, len));
        s.setPosition(i*len + x_0, y_0);
        if(mode == SUPER_FAN)
            s.setPosition(i*len + x_0_fan, y_0_fan);
        window->draw(s);
        s.setPosition(x_0, i*len + y_0);
        if(mode == SUPER_FAN)
            s.setPosition(x_0_fan, i*len + y_0_fan);
        window->draw(s);
    }
	
	// draw a board to that ships are being written
    for(int i = 0; i < SIZE; i++)
        for(int j = 0; j < SIZE; j++)
        {
            if(man->Myboard->Get(i, j) == POISON)
                continue;
            else if(man->Myboard->Get(i, j) == HIT_SHIP)
                s.setTextureRect(IntRect(2 * len, 0, len, len));
            else if(man->Myboard->Get(i, j) == HIT_MISS)
                s.setTextureRect(IntRect(3 * len, 0, len, len));
            else
                s.setTextureRect(IntRect(1 * len, 0, len, len));
            s.setPosition(i*len + x_0, j*len + y_0);
            if(mode == SUPER_FAN)
                s.setPosition(i*len + x_0_fan, j*len + y_0_fan);
            window->draw(s);
        }
}

