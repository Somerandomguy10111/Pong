#include <chrono>
#include <iostream>
#include <bits/this_thread_sleep.h>
#include <SFML/Graphics.hpp>
#include "vector.h"
#include <cmath>


class Collidable {
public:
    Vector n{0,0};
    Vector c{0,0};
    float d,L;

    Collidable(Vector n, Vector c, float d, float L) {
        this->n = n;
        this->c = c;
        this->d = d;
        this->L = L;
    }

    bool contains(Vector& pos, float extent = 0) {
        Vector s1 = Vector(-n.x, -n.y); // Dot product positive if penetrating in direction -n
        Vector s2 = Vector(-n.y, n.x);  // Gains in absolute value parallel to collsion surface
        Vector q = pos;

        float penetration_depth = s1.dot(q-c);
        float parallel_distance = std::fabs(s2.dot(q-c));

        return penetration_depth >= -extent and parallel_distance <= L/2;
    }

    void collide(Vector& pos, Vector& vel, float extent = 0) {
        if (this->contains(pos, extent)) {
            vel = vel-n*n.dot(vel)*2;
            pos = pos+vel;
        }
    }

};

class PlayerRect {
public:
    float length{100.f};
    Vector pos;
    Vector velocity{0,0};
    Collidable coll;
    sf::RectangleShape visual;

    PlayerRect(const float x,const float y, float direction = 1.f)
        : pos{x,y},
          coll(Vector{direction*1,0}, Vector{0,0}, 12, length),
          visual{sf::Vector2f(12.f,length)}
    {
        auto rect = this->visual;
        this->visual.setPosition(x, y);
        this->visual.setOrigin(rect.getSize().x/2, rect.getSize().y/2);
    }

    void move() {
        coll.c = pos;
        pos = pos+velocity;
        visual.setPosition(pos.x, pos.y);
    }
};

class DottedLine {
public:
    std::vector<sf::RectangleShape> visuals;

    DottedLine(const float x, const float ystart, const float yend, const float interval) {
        for (float y = ystart; y < yend; y += interval) {
            sf::RectangleShape rect{sf::Vector2f(20.f, 20.f)};
            rect.setPosition(x, y);
            this->visuals.push_back(rect);
        }
    }
};


class Ball {
public:
    Vector position{0,0};
    Vector velocity{0,0};
    sf::CircleShape model{0};

    Ball(float radius = 12) {
        position.x = 500;
        position.y = 300;
        velocity.x = 0.04;
        velocity.y = 0;

        model.setRadius(radius);
        model.setOrigin(12,12);

    }

    void move() {
        position = position + velocity;
        position.y = position.y + velocity.y;
        this->model.setPosition(position.x, position.y);
    }
};

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