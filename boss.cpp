#include "boss.hpp"

Boss::Boss(Texture2D texture)
    : texture(texture), x(1920 / 2), y(1080 / 2), speedX(300.0f), speedY(300.0f), width(100), height(100)
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
    DrawTexture(texture, static_cast<int>(x), static_cast<int>(y), WHITE);
}

Rectangle Boss::GetRect() const
{
    return Rectangle{ x, y, static_cast<float>(width), static_cast<float>(height) };
}

// ...existing code...
void Boss::Reset()
{
    x = 1920 / 2;
    y = 1080 / 2;
    speedX = 300.0f;
    speedY = 300.0f;
}
// ...existing code...
