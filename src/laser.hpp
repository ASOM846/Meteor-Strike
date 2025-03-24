#ifndef LASER_HPP
#define LASER_HPP

#include <raylib.h>

class Laser {
public:
    Laser(float startX, float startY);
    Laser(float startX, float startY, Texture2D texture); // Dodaj nowy konstruktor
    ~Laser();

    void Update();
    void Draw(bool useGraphics); // Dodaj parametr useGraphics
    Rectangle GetRect() const;

    bool active; // Flaga aktywno˜ci lasera

private:
    float x;
    float y;
    float speed;
    Texture2D texture; // Dodaj zmienn¥ do przechowywania tekstury
};

#endif // LASER_HPP

