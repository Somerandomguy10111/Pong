//
// Created by daniel on 25/09/2025.
//

#ifndef PONG_VECTOR_H
#define PONG_VECTOR_H

class Vector {
public:
    float x,y;

    Vector(float x, float y);

    Vector operator+(const Vector& other) const;

    Vector operator-(const Vector& other) const;

    Vector operator*(float scalar) const;

    float norm();

    float dot(const Vector& other);
};


#endif //PONG_VECTOR_H
