// SebF2DEngine.cpp : définit le point d'entrée de l'application.
//

#include "SebF2DEngine.h"
#include <SFML/Graphics.hpp>
#include "Engine/GameEngine.h"

int main()
{
    //sf::RenderWindow window(sf::VideoMode({ 800, 600 }), "SFML works!");
    //sf::CircleShape shape(100.f);
    //shape.setFillColor(sf::Color::Green);

    GameEngine gameEngine;

    gameEngine.Run();

    //gameEngine.Init();

    /*while (window.isOpen())
    {
        //gameEngine.MainLoop();
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear();
        window.draw(shape);
        window.display();
    }*/
}
