#include <chrono>
#include <iostream>
#include <bits/this_thread_sleep.h>
#include <SFML/Graphics.hpp>


class PlayerRect {
public:
    float x,y;
    sf::RectangleShape visuals;

    PlayerRect(const float x,const float y) {
        this->x = x;
        this->y = y;
        this->visuals = sf::RectangleShape{sf::Vector2f(25.f, 150.f)};
        this->visuals.setPosition(x,y);
    }

    void move(const float deltaX,const float deltaY) {
        this->x += deltaX;
        this->y += deltaY;
        this->visuals.setPosition(this->x, this->y);
    }
};



class Vector {
public:
    float x,y;

    Vector(const float x, const float y) {
        this->x = x;
        this->y = y;
    }
};

class DottedLine {
public:
    std::vector<sf::RectangleShape> visuals;

    DottedLine(const float x, const float ystart, const float yend, const float interval) {
        for (float y = ystart; y < yend; y += interval) {
            sf::RectangleShape rect{sf::Vector2f(25.f, 25.f)};
            rect.setPosition(x, y);
            this->visuals.push_back(rect);
        }
    }
};

int main() {
    constexpr int FPS = 60;
    const auto frameDuration = std::chrono::milliseconds(1000 / FPS);
    sf::RenderWindow window(sf::VideoMode(1080,600), "Window");

    PlayerRect p1{100,0};
    PlayerRect p2{1080-100, 200};
    DottedLine line{1080/2, 0, 600, 50};
    Vector velocity{0,0};

    while (window.isOpen()) {
        auto frameStart = std::chrono::steady_clock::now();

        // Handle event
        sf::Event event{};
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            if (event.type == sf::Event::KeyPressed) {
                std::cout << "Registered Key press!" << "\n";
                if (event.key.code == sf::Keyboard::Up)    { velocity.y = -1; }
                if (event.key.code == sf::Keyboard::Down)  { velocity.y = 1; }
            }

            if (event.type == sf::Event::KeyReleased) {
                std::cout << "Registered Key release!" << "\n";
                if (event.key.code == sf::Keyboard::Up)    { velocity.y = 0; }
                if (event.key.code == sf::Keyboard::Down)  { velocity.y = 0; }
            }
        }

        // 2 | Update model
        p1.move(0.2*velocity.x, 0.2*velocity.y);

        // 3 | Render frame
        window.clear(sf::Color::Transparent);
        window.draw(p1.visuals);
        window.draw(p2.visuals);
        int N_DOTS = line.visuals.size();
        for (int j = 0; j < N_DOTS; j++) {
            window.draw(line.visuals[j]);
        }

        window.display();

        // Wait for next frame
        auto frameEnd = std::chrono::steady_clock::now();
        auto computationTime = frameEnd - frameStart;
        if (computationTime > frameDuration) {
            auto wait_time = frameDuration - computationTime;
            std::this_thread::sleep_for(wait_time);
        }

    }

}