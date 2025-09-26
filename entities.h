//
// Created by daniel on 26/09/2025.
//

#ifndef PONG_ENTITIES_H
#define PONG_ENTITIES_H

#include "vector.h"
#include <SFML/Graphics.hpp>


class Collidable {
public:
    Vector n{0,0};
    Vector c{0,0};
    float d,L;

    Collidable(Vector n, Vector c, float d, float L);

    bool contains(Vector& pos, float extent = 0);

    void collide(Vector& pos, Vector& vel, float extent = 0);
};


class DottedLine {
public:
    std::vector<sf::RectangleShape> visuals;

    DottedLine(const float x, const float ystart, const float yend, const float interval);
};

class PlayerRect {
public:
    float length{100.f};
    Vector pos;
    Vector velocity{0,0};
    Collidable coll;
    sf::RectangleShape visual;

    PlayerRect(const float x,const float y, float direction = 1.f);

    void move();
};

class Ball {
public:
    Vector position{0,0};
    Vector velocity{0,0};
    sf::CircleShape model{0};

    explicit Ball(float radius = 12);

    void move();
};

#endif //PONG_ENTITIES_H