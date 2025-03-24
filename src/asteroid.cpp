#include "asteroid.hpp"

Texture2D Asteroid::bigTexture1;
Texture2D Asteroid::bigTexture2;
Texture2D Asteroid::bigTexture3;
Texture2D Asteroid::bigTexture4;
Texture2D Asteroid::medTexture1;
Texture2D Asteroid::medTexture2;
Texture2D Asteroid::smallTexture1;
Texture2D Asteroid::smallTexture2;
Texture2D Asteroid::tinyTexture1;
Texture2D Asteroid::tinyTexture2;
Texture2D Asteroid::hpUpTexture; // Dodaj tekstur© hpUp

Asteroid::Asteroid()
    : x(GetRandomValue(0, 1920)),
    y(-GetRandomValue(100, 200)),
    width(GetRandomValue(30, 100)),
    height(width),
    speed(5.0f),
    active(true),
    isHpUp(false) // Inicjalizuj isHpUp jako false
{
    if (GetRandomValue(0, 100) < 10) { // 10% szans na spawnowanie hpUp
        isHpUp = true;
        width = 34;
        height = 34;
        texture = hpUpTexture;
    }
    else {
        if (width > 80) {
            int textureIndex = GetRandomValue(1, 4);
            switch (textureIndex) {
            case 1: texture = bigTexture1; break;
            case 2: texture = bigTexture2; break;
            case 3: texture = bigTexture3; break;
            case 4: texture = bigTexture4; break;
            }
        }
        else if (width > 60) {
            int textureIndex = GetRandomValue(1, 2);
            switch (textureIndex) {
            case 1: texture = medTexture1; break;
            case 2: texture = medTexture2; break;
            }
        }
        else if (width > 40) {
            int textureIndex = GetRandomValue(1, 2);
            switch (textureIndex) {
            case 1: texture = smallTexture1; break;
            case 2: texture = smallTexture2; break;
            }
        }
        else {
            int textureIndex = GetRandomValue(1, 2);
            switch (textureIndex) {
            case 1: texture = tinyTexture1; break;
            case 2: texture = tinyTexture2; break;
            }
        }
    }
}

Asteroid::~Asteroid()
{
}

void Asteroid::Update()
{
    if (!active) return;

    y += speed;

    if (y > 1080) {
        y = -GetRandomValue(100, 200);
        x = GetRandomValue(0, 1920);
        width = GetRandomValue(30, 100);
        height = width;
    }
}

void Asteroid::Draw(bool useGraphics)
{
    if (active) {
        if (useGraphics) {
            DrawTexture(texture, x, y, WHITE);
        }
        else {
            if (isHpUp) {
                DrawRectangle(x, y, width, height, RED); // Rysuj czerwon¥ skrzynk© dla hpUp
            }
            else {
                DrawRectangle(x, y, width, height, GRAY);
            }
        }
    }
}

Rectangle Asteroid::GetRect() const
{
    return Rectangle{ x, y, width, height };
}

bool Asteroid::IsActive() const
{
    return active;
}

void Asteroid::Deactivate()
{
    active = false;
}

bool Asteroid::IsHpUp() const
{
    return isHpUp;
}

void Asteroid::LoadTextures()
{
    bigTexture1 = LoadTexture("graphics/meteorGrey_big1.png");
    bigTexture2 = LoadTexture("graphics/meteorGrey_big2.png");
    bigTexture3 = LoadTexture("graphics/meteorGrey_big3.png");
    bigTexture4 = LoadTexture("graphics/meteorGrey_big4.png");
    medTexture1 = LoadTexture("graphics/meteorGrey_med1.png");
    medTexture2 = LoadTexture("graphics/meteorGrey_med2.png");
    smallTexture1 = LoadTexture("graphics/meteorGrey_small1.png");
    smallTexture2 = LoadTexture("graphics/meteorGrey_small2.png");
    tinyTexture1 = LoadTexture("graphics/meteorGrey_tiny1.png");
    tinyTexture2 = LoadTexture("graphics/meteorGrey_tiny2.png");
    hpUpTexture = LoadTexture("graphics/hpUp.png"); // Zaˆaduj tekstur© hpUp
}

void Asteroid::UnloadTextures()
{
    UnloadTexture(bigTexture1);
    UnloadTexture(bigTexture2);
    UnloadTexture(bigTexture3);
    UnloadTexture(bigTexture4);
    UnloadTexture(medTexture1);
    UnloadTexture(medTexture2);
    UnloadTexture(smallTexture1);
    UnloadTexture(smallTexture2);
    UnloadTexture(tinyTexture1);
    UnloadTexture(tinyTexture2);
    UnloadTexture(hpUpTexture); // Zwolnij tekstur© hpUp
}
