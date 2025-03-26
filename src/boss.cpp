#include "boss.hpp"

Boss::Boss(Texture2D texture)
    : texture(texture), x(1920 / 2), y(1080 / 2), speedX(300), speedY(300), width(100), height(100)
{
}

void Boss::Update()
{
    x += speedX * GetFrameTime();
    y += speedY * GetFrameTime();

    if (x <= 0 || x + width >= 1920) speedX = -speedX;
    if (y <= 0 || y + height >= 1080) speedY = -speedY;
}

void Boss::Draw()
{
    DrawTexture(texture, x, y, WHITE);
}

Rectangle Boss::GetRect() const
{
    return Rectangle{ x, y, (float)width, (float)height };
}

void Boss::Reset()
{
    x = 1920 / 2;
    y = 1080 / 2;
    speedX = 300;
    speedY = 300;
}
