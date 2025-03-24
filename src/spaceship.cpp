#include "spaceship.hpp"

Spaceship::Spaceship()
    : width(50), height(50), useSounds(true)
{
    x = 1920 / 2; // Ustaw x na ?rodek ekranu
    y = 1080 - 200; // Ustaw y 50 pikseli nad doln¤ kraw?dzi¤
}

Spaceship::Spaceship(Texture2D shipTexture, Texture2D laserTexture, Sound laserSound)
    : width(50), height(50), texture(shipTexture), laserTexture(laserTexture), laserSound(laserSound), useSounds(true)
{
    x = 1920 / 2; // Ustaw x na ?rodek ekranu
    y = 1080 - 200; // Ustaw y 50 pikseli nad doln¤ kraw?dzi¤
}

Spaceship::~Spaceship()
{
    // Nie zwalniamy tekstur i d®wi?kôw, poniewa– s¤ one zwalniane w klasie Game
}

void Spaceship::Update()
{
    Move();
    CheckIfOffScreen();
    for (auto& laser : lasers)
    {
        laser.Update();
    }
}

void Spaceship::Draw(bool useGraphics)
{
    if (useGraphics) {
        DrawTexture(texture, x - 25, y, WHITE); // Rysuj statek kosmiczny z tekstur¥
    }
    else {
        // Rysuj statek kosmiczny jako prostok¥t z tr¢jk¥tem na g¢rze
        DrawRectangle(x - width / 2, y, width, height, BLUE); // Rysuj prostok¥t
        DrawTriangle(
            Vector2{ (float)x, (float)(y - height / 2) }, // Wierzchoˆek tr¢jk¥ta
            Vector2{ (float)(x - width / 2), (float)y },  // Lewy dolny r¢g tr¢jk¥ta
            Vector2{ (float)(x + width / 2), (float)y },  // Prawy dolny r¢g tr¢jk¥ta
            BLUE
        ); // Rysuj tr¢jk¥t na g¢rze
    }
    for (auto& laser : lasers)
    {
        laser.Draw(useGraphics); // Przeka¾ parametr useGraphics do metody Draw lasera
    }
}

void Spaceship::FireLaser(bool useGraphics, bool useSounds)
{
    double currentTime = GetTime();
    if (currentTime - lastFireTime >= 0.5) // Ograniczenie strza?ôw do jednego na 0.5 sekundy
    {
        if (useGraphics) {
            lasers.emplace_back(x + width / 2, y, laserTexture); // Przesu„ strza? troch? w lewo, gdy grafiki s¤ w?¤czone
        }
        else {
            lasers.emplace_back(x + width / 2 - 25, y); // Strzelaj ze ?rodka, gdy grafiki s¤ wy?¤czone
        }
        if (this->useSounds) { // U–yj zmiennej cz?onkowskiej
            PlaySound(laserSound); // Odtwôrz d®wi?k lasera, gdy d®wi?ki s¤ w?¤czone
        }
        lastFireTime = currentTime;
    }
}

std::vector<Laser>& Spaceship::GetLasers()
{
    return lasers; // Zwrô? referencj? do wektora laserôw
}

void Spaceship::Move()
{
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) x += 7;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) x -= 7;
}

void Spaceship::CheckIfOffScreen()
{
    if (x + width > 1920) x = 0; // Teleportuj na lew¤ kraw?d®
    if (x < 0) x = 1920 - width; // Teleportuj na praw¤ kraw?d®
}

Rectangle Spaceship::GetRect() const
{
    return Rectangle{ (float)x, (float)y, (float)width, (float)height }; // Zwrô? prostok¤t reprezentuj¤cy hitboks statku
}

void Spaceship::SetUseSounds(bool useSounds) {
    this->useSounds = useSounds;
}
