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
Texture2D Asteroid::hpUpTexture;
Texture2D Asteroid::brownBigTexture1;
Texture2D Asteroid::brownBigTexture2;
Texture2D Asteroid::brownBigTexture3;
Texture2D Asteroid::brownBigTexture4;
Texture2D Asteroid::brownMedTexture1;
Texture2D Asteroid::brownMedTexture2;
Texture2D Asteroid::brownSmallTexture1;
Texture2D Asteroid::brownSmallTexture2;
Texture2D Asteroid::brownTinyTexture1;
Texture2D Asteroid::brownTinyTexture2;

Asteroid::Asteroid(int level)
    : x(static_cast<float>(GetRandomValue(0, 1920))),
    y(static_cast<float>(-GetRandomValue(100, 200))),
    width(static_cast<float>(GetRandomValue(30, 100))),
    height(width),
    speed(5.0f),
    active(true),
    isHpUp(false),
    hitsRequired(1),
    currentHits(0),
    isGray(false)
{
    if (GetRandomValue(0, 100) < 10) {
        isHpUp = true;
        width = 34.0f;
        height = 34.0f;
        texture = hpUpTexture;
    }
    else {
        if (level >= 2 && GetRandomValue(0, 100) < 50) {
            hitsRequired = 2;
            if (width > 80) {
                int textureIndex = GetRandomValue(1, 4);
                switch (textureIndex) {
                case 1: texture = brownBigTexture1; break;
                case 2: texture = brownBigTexture2; break;
                case 3: texture = brownBigTexture3; break;
                case 4: texture = brownBigTexture4; break;
                }
            }
            else if (width > 60) {
                int textureIndex = GetRandomValue(1, 2);
                switch (textureIndex) {
                case 1: texture = brownMedTexture1; break;
                case 2: texture = brownMedTexture2; break;
                }
            }
            else if (width > 40) {
                int textureIndex = GetRandomValue(1, 2);
                switch (textureIndex) {
                case 1: texture = brownSmallTexture1; break;
                case 2: texture = brownSmallTexture2; break;
                }
            }
            else {
                int textureIndex = GetRandomValue(1, 2);
                switch (textureIndex) {
                case 1: texture = brownTinyTexture1; break;
                case 2: texture = brownTinyTexture2; break;
                }
            }
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
}

Asteroid::~Asteroid()
{
}

void Asteroid::Update()
{
    if (!active) return;

    y += speed;

    if (y > 1080) {
        y = static_cast<float>(-GetRandomValue(100, 200));
        x = static_cast<float>(GetRandomValue(0, 1920));
        width = static_cast<float>(GetRandomValue(30, 100));
        height = width;
    }
}

void Asteroid::Draw()
{
    if (active) {
        DrawTexture(texture, static_cast<int>(x), static_cast<int>(y), WHITE);
    }
}

Rectangle Asteroid::GetRect() const
{
    return Rectangle{ x, y, static_cast<float>(texture.width), static_cast<float>(texture.height) };
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

void Asteroid::Hit()
{
    currentHits++;
    if (currentHits >= hitsRequired) {
        Deactivate();
    }
    else if (hitsRequired == 2 && currentHits == 1) {
        ChangeToGrayTexture();
    }
}

void Asteroid::ChangeToGrayTexture()
{
    if (texture.id == brownBigTexture1.id) texture = bigTexture1;
    else if (texture.id == brownBigTexture2.id) texture = bigTexture2;
    else if (texture.id == brownBigTexture3.id) texture = bigTexture3;
    else if (texture.id == brownBigTexture4.id) texture = bigTexture4;
    else if (texture.id == brownMedTexture1.id) texture = medTexture1;
    else if (texture.id == brownMedTexture2.id) texture = medTexture2;
    else if (texture.id == brownSmallTexture1.id) texture = smallTexture1;
    else if (texture.id == brownSmallTexture2.id) texture = smallTexture2;
    else if (texture.id == brownTinyTexture1.id) texture = tinyTexture1;
    else if (texture.id == brownTinyTexture2.id) texture = tinyTexture2;
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
    hpUpTexture = LoadTexture("graphics/hpUp.png");
    brownBigTexture1 = LoadTexture("graphics/meteorBrown_big1.png");
    brownBigTexture2 = LoadTexture("graphics/meteorBrown_big2.png");
    brownBigTexture3 = LoadTexture("graphics/meteorBrown_big3.png");
    brownBigTexture4 = LoadTexture("graphics/meteorBrown_big4.png");
    brownMedTexture1 = LoadTexture("graphics/meteorBrown_med1.png");
    brownMedTexture2 = LoadTexture("graphics/meteorBrown_med2.png");
    brownSmallTexture1 = LoadTexture("graphics/meteorBrown_small1.png");
    brownSmallTexture2 = LoadTexture("graphics/meteorBrown_small2.png");
    brownTinyTexture1 = LoadTexture("graphics/meteorBrown_tiny1.png");
    brownTinyTexture2 = LoadTexture("graphics/meteorBrown_tiny2.png");
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
    UnloadTexture(hpUpTexture);
    UnloadTexture(brownBigTexture1);
    UnloadTexture(brownBigTexture2);
    UnloadTexture(brownBigTexture3);
    UnloadTexture(brownBigTexture4);
    UnloadTexture(brownMedTexture1);
    UnloadTexture(brownMedTexture2);
    UnloadTexture(brownSmallTexture1);
    UnloadTexture(brownSmallTexture2);
    UnloadTexture(brownTinyTexture1);
    UnloadTexture(brownTinyTexture2);
}
