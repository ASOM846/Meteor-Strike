#ifndef LASER_HPP
#define LASER_HPP

#include <raylib.h>

class Laser {
public:
    Laser(float startX, float startY);
    Laser(float startX, float startY, Texture2D texture);
    ~Laser();

    void Update();
    void Draw();
    Rectangle GetRect() const;

    bool active;

    float GetX() const { return x; }
    float GetY() const { return y; }
    float GetSpeed() const { return speed; }
    void SetY(float newY) { y = newY; }

private:
    float x;
    float y;
    float speed;
    Texture2D texture;
};

class LaserUlt {
public:
    LaserUlt(float startX, float startY);
    LaserUlt(float startX, float startY, Texture2D texture);
    ~LaserUlt();

    void Update();
    void Draw();
    Rectangle GetRect() const;

    bool active;

    float GetX() const { return x; }
    float GetY() const { return y; }
    float GetSpeed() const { return speed; }
    void SetY(float newY) { y = newY; }

private:
    float x;
    float y;
    float speed;
    Texture2D texture;
};

#endif // LASER_HPP
