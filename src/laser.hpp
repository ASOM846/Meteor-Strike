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

private:
    float x;
    float y;
    float speed;
    Texture2D texture;
};

#endif // LASER_HPP
