#include <chrono>
#include <iostream>
#include <bits/this_thread_sleep.h>
#include <SFML/Graphics.hpp>
#include "vector.h"
#include "entities.h"

int main() {
    constexpr int FPS = 60;
    float XMAX = 1080;
    float YMAX = 600;
    const auto frameDuration = std::chrono::milliseconds(1000 / FPS);
    sf::RenderWindow window(sf::VideoMode(XMAX, YMAX), "Window");

    PlayerRect p1{100,0};
    PlayerRect p2{XMAX-100, 200, -1};
    DottedLine line{XMAX/2.f, 0, YMAX, 50};

    Collidable topWall(Vector{0,1}, Vector{0,0}, 500, 4000);
    Collidable bottomWall{Vector{0,-1}, Vector{0, YMAX}, 500, 4000};

    Ball ball;

    while (window.isOpen()) {
        auto frameStart = std::chrono::steady_clock::now();

        // 1 | Handle events
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                float topSpeed = 0.1f;
                if (event.key.code == sf::Keyboard::W)    p1.velocity.y = -topSpeed;
                if (event.key.code == sf::Keyboard::S)    p1.velocity.y = topSpeed;
                if (event.key.code == sf::Keyboard::Up)   p2.velocity.y = -topSpeed;
                if (event.key.code == sf::Keyboard::Down) p2.velocity.y = topSpeed;
            }

            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::W || event.key.code == sf::Keyboard::S)
                    p1.velocity.y = 0;
                if (event.key.code == sf::Keyboard::Up || event.key.code == sf::Keyboard::Down)
                    p2.velocity.y = 0;
            }
        }

        // 2 | Update model
        p1.move();
        p2.move();
        ball.move();

        p1.coll.collide(ball.position, ball.velocity, ball.model.getRadius());
        p2.coll.collide(ball.position, ball.velocity, ball.model.getRadius());
        topWall.collide(ball.position, ball.velocity, ball.model.getRadius());
        bottomWall.collide(ball.position, ball.velocity, ball.model.getRadius());

        // 3 | Render frame
        window.clear(sf::Color::Transparent);
        window.draw(p1.visual);
        window.draw(p2.visual);
        window.draw(ball.model);
        std::size_t N_DOTS = line.visuals.size();
        for (int j = 0; j < N_DOTS; j++) {
            window.draw(line.visuals[j]);
        }

        // Wait for next frame
        auto frameEnd = std::chrono::steady_clock::now();
        auto computationTime = frameEnd - frameStart;

        if (computationTime > frameDuration) {
            auto wait_time = frameDuration - computationTime;
            std::this_thread::sleep_for(wait_time);
        }
        window.display();


    }

}