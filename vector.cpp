class Vector {
public:
    float x,y;

    Vector(const float x, const float y) {
        this->x = x;
        this->y = y;
    }

    Vector operator+(Vector& other) const {
        return Vector{this->x+other.x, this->y+other.y};
    }

    Vector operator-(Vector& other) const {
        return Vector{this->x-other.x, this->y-other.y};
    }

    Vector operator*(float scalar) const {
        return Vector{this->x*scalar, this->y*scalar};
    }
};