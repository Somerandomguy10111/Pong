//
// Created by daniel on 25/09/2025.
//

#ifndef PONG_VECTOR_H
#define PONG_VECTOR_H

class Vector {
public:
    float x,y;

    Vector(float x, float y);

    Vector operator+(Vector& other) const;

    Vector operator-(Vector& other) const;

    Vector operator*(float scalar) const;
};


#endif //PONG_VECTOR_H
