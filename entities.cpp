#include "vector.h"
#include "entities.h"
#include <cmath>
#include <SFML/Graphics.hpp>


Collidable::Collidable(Vector n, Vector c, float d, float L) {
    this->n = n;
    this->c = c;
    this->d = d;
    this->L = L;
}

bool Collidable::contains(Vector &pos, float extent) {
    Vector s1 = Vector(-n.x, -n.y); // Dot product positive if penetrating in direction -n
    Vector s2 = Vector(-n.y, n.x);  // Gains in absolute value parallel to collsion surface
    Vector q = pos;

    float penetration_depth = s1.dot(q-c);
    float parallel_distance = std::fabs(s2.dot(q-c));

    return penetration_depth >= -extent and parallel_distance <= L/2;
}

void Collidable::collide(Vector &pos, Vector &vel, float extent) {
    if (this->contains(pos, extent)) {
        vel = vel-n*n.dot(vel)*2;
        pos = pos+vel;
    }
}


PlayerRect::PlayerRect(const float x, const float y, float direction): pos{x,y},
                                                                       coll(Vector{direction*1,0}, Vector{0,0}, 12, length),
                                                                       visual{sf::Vector2f(12.f,length)} {
    auto rect = this->visual;
    this->visual.setPosition(x, y);
    this->visual.setOrigin(rect.getSize().x/2, rect.getSize().y/2);
}

void PlayerRect::move() {
    coll.c = pos;
    pos = pos+velocity;
    visual.setPosition(pos.x, pos.y);
}



DottedLine::DottedLine(const float x, const float ystart, const float yend, const float interval) {
    for (float y = ystart; y < yend; y += interval) {
        sf::RectangleShape rect{sf::Vector2f(20.f, 20.f)};
        rect.setPosition(x, y);
        this->visuals.push_back(rect);
    }
}


Ball::Ball(float radius) {
    position.x = 500;
    position.y = 300;
    velocity.x = 0.04;
    velocity.y = 0.02;

    model.setRadius(radius);
    model.setOrigin(12,12);
}

void Ball::move() {
    position = position + velocity;
    position.y = position.y + velocity.y;
    this->model.setPosition(position.x, position.y);
}
