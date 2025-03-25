#include "spaceship.hpp"

Spaceship::Spaceship()
    : width(50), height(50), useSounds(true)
{
    x = 1920 / 2;
    y = 1080 - 200;
}

Spaceship::Spaceship(Texture2D shipTexture, Texture2D laserTexture, Sound laserSound, Texture2D shieldTexture)
    : width(50), height(50), texture(shipTexture), laserTexture(laserTexture), laserSound(laserSound), shieldTexture(shieldTexture), useSounds(true)
{
    x = 1920 / 2;
    y = 1080 - 200;
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

    if (shieldActive && GetTime() > shieldEndTime) {
        shieldActive = false;
        shieldCooldown = 10.0; // Ustaw czas odnowienia tarczy po jej wy?¤czeniu
    }

    if (!shieldActive && shieldCooldown > 0) {
        shieldCooldown -= GetFrameTime();
    }
}

void Spaceship::Draw(bool useGraphics)
{
    if (useGraphics) {
        DrawTexture(texture, x - 25, y, WHITE);
    }
    else {
        DrawRectangle(x - width / 2, y, width, height, WHITE);
        DrawTriangle(
            Vector2{ (float)x, (float)(y - height / 2) },
            Vector2{ (float)(x - width / 2), (float)y },
            Vector2{ (float)(x + width / 2), (float)y },
            WHITE
        );
    }

    if (shieldActive) {
        if (useGraphics) {
            DrawTexture(shieldTexture, x - 50, y - 30, WHITE);
        }
        else {
            DrawCircle(x, y + height - 60 / 2, width, Fade(BLUE, 0.3f)); // Rysuj tarcz?
        }
    }

    for (auto& laser : lasers)
    {
        laser.Draw(useGraphics);
    }
}

void Spaceship::FireLaser(bool useGraphics, bool useSounds)
{
    double currentTime = GetTime();
    if (currentTime - lastFireTime >= 0.5)
    {
        if (useGraphics) {
            lasers.emplace_back(x + width / 2, y, laserTexture);
        }
        else {
            lasers.emplace_back(x + width / 2 - 25, y);
        }
        if (this->useSounds) {
            PlaySound(laserSound);
        }
        lastFireTime = currentTime;
    }
}

std::vector<Laser>& Spaceship::GetLasers()
{
    return lasers;
}

void Spaceship::Move()
{
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) x += speed;
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) x -= speed;
}

void Spaceship::CheckIfOffScreen()
{
    if (x + width > 1920) x = 0;
    if (x < 0) x = 1920 - width;
}

Rectangle Spaceship::GetRect() const
{
    return Rectangle{ (float)x, (float)y, (float)width, (float)height };
}

void Spaceship::SetUseSounds(bool useSounds) {
    this->useSounds = useSounds;
}

void Spaceship::IncreaseSpeed() {
    speed += 2;
}

void Spaceship::DecreaseSpeed() {
    speed -= 2;
}

int Spaceship::GetSpeed() const {
    return speed;
}

void Spaceship::ActivateShield() {
    shieldActive = true;
    shieldEndTime = GetTime() + shieldDuration;
}

bool Spaceship::IsShieldActive() const {
    return shieldActive;
}

void Spaceship::SetShieldLevel(int level) {
    shieldDuration = 5.0 + (level - 1) * 2.0;
}

void Spaceship::IncreaseShieldTime() {
    if (GetShieldLevel() < 5) {
        shieldDuration += 2.0;
    }
}

int Spaceship::GetShieldLevel() const {
    return static_cast<int>((shieldDuration - 5.0) / 2.0) + 1;
}

double Spaceship::GetShieldCooldown() const {
    return shieldCooldown;
}

