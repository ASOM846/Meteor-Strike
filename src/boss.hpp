#ifndef BOSS_HPP
#define BOSS_HPP

#include <raylib.h>

class Boss {
public:
    Boss(Texture2D texture);
    void Update();
    void Draw();
    Rectangle GetRect() const;
    void Reset();

private:
    Texture2D texture;
    float x, y;
    float speedX, speedY;
    int width, height;
};

#endif // BOSS_HPP
