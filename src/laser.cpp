#include "laser.hpp"

Laser::Laser(float startX, float startY) : x(startX), y(startY), speed(10.0f), active(true)
{
}

Laser::Laser(float startX, float startY, Texture2D texture) : x(startX), y(startY), speed(10.0f), active(true), texture(texture)
{
}

Laser::~Laser()
{
    // Nie zwalniamy tekstury, poniewa¾ jest ona zwalniana w klasie Game
}

void Laser::Update()
{
    y -= speed; // Przesuä laser w g¢r©

    // Dezaktywuj laser, je¾eli wyjdzie poza ekran
    if (y < 0) {
        active = false;
    }
}

void Laser::Draw()
{
    if (active) {
        DrawTexture(texture, x - texture.width / 2, y - texture.height / 2, WHITE); // Rysowanie lasera z tekstur¥
    }
}

Rectangle Laser::GetRect() const
{
    return Rectangle{ x - 2, y - 10, 4, 20 }; // Zwr¢† prostok¥t reprezentuj¥cy hitboks lasera
}

LaserUlt::LaserUlt(float startX, float startY) : x(startX), y(startY), speed(15.0f), active(true)
{
}

LaserUlt::LaserUlt(float startX, float startY, Texture2D texture) : x(startX), y(startY), speed(15.0f), active(true), texture(texture)
{
}

LaserUlt::~LaserUlt()
{
    // Nie zwalniamy tekstury, poniewa¾ jest ona zwalniana w klasie Game
}

void LaserUlt::Update()
{
    y -= speed; // Przesuä laser w g¢r©

    // Dezaktywuj laser, je¾eli wyjdzie poza ekran
    if (y < 0) {
        active = false;
    }
}

void LaserUlt::Draw()
{
    if (active) {
        DrawTexture(texture, x - texture.width / 2, y - texture.height / 2, RED); // Rysowanie lasera z tekstur¥
    }
}

Rectangle LaserUlt::GetRect() const
{
    return Rectangle{ x - 4, y - 20, 8, 40 }; // Zwr¢† prostok¥t reprezentuj¥cy hitboks lasera
}
