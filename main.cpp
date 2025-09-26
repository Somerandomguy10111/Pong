#include <chrono>
#include <iostream>
#include <bits/this_thread_sleep.h>
#include <SFML/Graphics.hpp>
#include "vector.h"


class PlayerRect {
public:
    Vector pos{0,0};
    Vector velocity{0,0};
    sf::RectangleShape visual{sf::Vector2f(12.f,100.f)};

    PlayerRect(const float x,const float y) {
        this->pos.x = x;
        this->pos.y = y;
        this->visual.setPosition(x,y);
    }

    void move() {
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


class Rectangle {
public:
    float xmin, xmax, ymin, ymax;

    Rectangle(float xmin, float xmax, float ymin, float ymax) {
        this->xmin = xmin;
        this->xmax = xmax;
        this->ymin = ymin;
        this->ymax = ymax;
    }

    bool contains(const Vector v) {
        bool xrange = xmin <= v.x && v.x <= xmax;
        bool yrange = ymin <= v.y && v.y <= ymax;
        return xrange && yrange;
    }
};


class Collidable {
public:
    Vector n{0,0};
    Vector p{0,0};
    float d,L;

    Collidable(Vector orientation, Vector p, float d, float L) {
        this->n = orientation;
        this->p = p;
        this->d = d;
        this->L = L;
    }

    bool contains(Vector& pos) {
        Vector s1 = Vector(-n.x, -n.y);
        Vector s2 = Vector(-n.y, n.x);
        Vector q = pos;

        std::cout << s2.dot(q-p) << "\n";
        bool axis_one = s1.dot(q-p) <= d and s1.dot(q-p) >= 0;
        bool axis_two = s2.dot(q-p) <= L and s2.dot(q-p) >= 0;
        return axis_one and axis_two;
    }

    void collide(Vector& pos, Vector& vel) {
        if (this->contains(pos)) {
            vel = vel-n*n.dot(vel)*2;
            pos = pos+vel;
        }
    }

};

class Ball {
public:
    Vector position{0,0};
    Vector velocity{0,0};
    sf::CircleShape visual{12};

    Ball() {
        position.x = 1080;
        position.y = 300;
        velocity.x = -0.05;
        velocity.y = 0;
    }

    void move() {
        position = position + velocity;
        position.y = position.y + velocity.y;
        this->visual.setPosition(position.x, position.y);
    }
};

int main() {
    constexpr int FPS = 60;
    float XMAX = 1080;
    float YMAX = 600;
    const auto frameDuration = std::chrono::milliseconds(1000 / FPS);
    sf::RenderWindow window(sf::VideoMode(XMAX, YMAX), "Window");

    PlayerRect p1{100,0};
    PlayerRect p2{XMAX-100, 200};
    DottedLine line{XMAX/2.f, 0, YMAX, 50};

    // Vector leftVector = Vector{-1,0};
    // Vector rightVector = Vector{1,0};
    // Vector downVector = Vector{0,1};
    // Vector upVector = Vector{0,-1};

    // Rectangle leftZone{0,100, 0, 2*YMAX};
    // Rectangle rightZone{XMAX-100,2*XMAX, 0, 2*YMAX};
    // Rectangle upperZone{0,2*XMAX,-2*YMAX,0};
    // Rectangle lowerZone{0,2*XMAX, YMAX, 2*YMAX};


    // Collidable leftWall{rightVector, leftZone};
    // Collidable topWall{downVector, upperZone};
    // Collidable bottomWall{upVector, lowerZone};
    // Collidable rightWall{leftVector, rightZone};
    Vector n = Vector{1,0};
    Vector p = Vector{XMAX/2,0};

    Collidable bounce = Collidable(n, p, 100, 1000);
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

        bounce.collide(ball.position, ball.velocity);
        // leftWall.collide(ball.position, ball.velocity);
        // rightWall.collide(ball.position, ball.velocity);
        // topWall.collide(ball.position, ball.velocity);
        // bottomWall.collide(ball.position, ball.velocity);
        // std::cout << ball.velocity.norm() << "\n";

        // 3 | Render frame
        window.clear(sf::Color::Transparent);
        window.draw(p1.visual);
        window.draw(p2.visual);
        window.draw(ball.visual);
        std::size_t N_DOTS = line.visuals.size();
        for (int j = 0; j < N_DOTS; j++) {
            window.draw(line.visuals[j]);
        }

        // Wait for next frame
        auto frameEnd = std::chrono::steady_clock::now();
        auto computationTime = frameEnd - frameStart;

        auto printableTime = std::chrono::duration_cast<std::chrono::milliseconds>(frameEnd-frameStart).count();
        // std::cout << "ComputationTime" << printableTime << "ms\n";

        if (computationTime > frameDuration) {
            auto wait_time = frameDuration - computationTime;
            std::this_thread::sleep_for(wait_time);
        }
        window.display();


    }

}