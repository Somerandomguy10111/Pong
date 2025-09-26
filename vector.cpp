#include "vector.h"

Vector::Vector(const float x, const float y) {
    this->x = x;
    this->y = y;
}

Vector Vector::operator+(const Vector& other) const {
    return Vector{this->x+other.x, this->y+other.y};
}

Vector Vector::operator-(const Vector& other) const {
    return Vector{this->x-other.x, this->y-other.y};
}

Vector Vector::operator*(float scalar) const {
    return Vector{this->x*scalar, this->y*scalar};
}