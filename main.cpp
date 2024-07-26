#include <iostream>
#include <SFML/Graphics.hpp>
#include "verletCircle.hpp"
#include "solver.hpp"
#include "renderer.hpp"
#include "dot.hpp"
#include "FPS.hpp"
#include <ctime>
#include <cstdlib>

//Simple Particle Simulation made in C++ with SFML - Summer 2024 Project - John Le


using namespace sf;
using namespace std;

static sf::Color getRainbow(float t)
{
    const float r = sin(t);
    const float g = sin(t + 0.33f * 2.0f * Math::PI);
    const float b = sin(t + 0.66f * 2.0f * Math::PI);
    return { static_cast<uint8_t>(255.0f * r * r),
            static_cast<uint8_t>(255.0f * g * g),
            static_cast<uint8_t>(255.0f * b * b) };
}

int main()
{
    //Initialize window and set framerate.
    const int WINDOW_WIDTH = 800;
    const int WINDOW_HEIGHT = 600;

    ContextSettings settings;
    settings.antialiasingLevel = 1;

    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Particle Simulation", Style::Default, settings);
    window.setFramerateLimit(60);

    //Initialize text elements
    Font font;
    font.loadFromFile("C:\\Users\\USER\\Documents\\PROJECTS\\Particle Simulation\\fonts\\Poppins-Regular.ttf"); // ADD IN USER DIRECTORY TO FONT

    FPS fpsClass;
    Text fps;
    fps.setFont(font);
    fps.setPosition(Vector2f(5, 5));
    fps.setCharacterSize(12);

    Text timeElapsed;
    timeElapsed.setFont(font);
    timeElapsed.setPosition(Vector2f(5, 17.5));
    timeElapsed.setCharacterSize(12);

    Text objectCount;
    objectCount.setFont(font);
    objectCount.setPosition(Vector2f(5, 30));
    objectCount.setCharacterSize(12);

    //Initialize physics variables
    Clock clock;
    Clock timeClock;
    Clock spawnDelay;
    Solver solver;
    Renderer renderer{ window };
    srand(static_cast <unsigned> (time(0)));

    //Set solver

    solver.setConstraint({ (WINDOW_WIDTH) * 0.5f, (WINDOW_HEIGHT) * 0.5f }, 300.0f);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed || Keyboard::isKeyPressed(Keyboard::Escape)) {
                window.close();
            }
        }

        if (Keyboard::isKeyPressed(Keyboard::A) && spawnDelay.getElapsedTime().asSeconds() >= 0.01f) {
            spawnDelay.restart();
            Vector2i mousePosition = Mouse::getPosition(window);
            float r = 5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (15 - 10)));
            auto& obj = solver.addObject((Vector2f(float(mousePosition.x), float(mousePosition.y))), r);
            obj.color = getRainbow(timeClock.getElapsedTime().asSeconds());
        }

        if (Mouse::isButtonPressed(sf::Mouse::Left) && spawnDelay.getElapsedTime().asSeconds() >= 0.3f) {
            spawnDelay.restart();
            Vector2i mousePosition = Mouse::getPosition(window);
            float r = 5 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (15 - 10)));
            auto& obj = solver.addObject((Vector2f(float(mousePosition.x), float(mousePosition.y))), r);
            obj.color = getRainbow(timeClock.getElapsedTime().asSeconds());
        }

        float timePassed = timeClock.getElapsedTime().asSeconds();
        timeElapsed.setString("Seconds Elapsed: " + to_string(int(timePassed)));

        fpsClass.update();
        fps.setString("FPS: " + to_string(fpsClass.getFPS()));

        objectCount.setString("Object Count: " + to_string(solver.getObjectCount()));

        //Update physics and render circles

        solver.update(clock.restart().asSeconds());
        window.clear(Color::Black);
        renderer.render(solver);
        window.draw(fps);
        window.draw(timeElapsed);
        window.draw(objectCount);
        window.display();
    }

    return 0;
}