#include "spaceship.hpp"

Spaceship::Spaceship()
    : width(50), height(50), useSounds(true)
{
    x = 1920 / 2;
    y = 1080 - 200;
}

Spaceship::Spaceship(Texture2D shipTexture, Texture2D laserTexture, Sound laserSound, Texture2D shieldTexture, Sound shieldUpSound, Sound shieldDownSound)
    : width(50), height(50), texture(shipTexture), laserTexture(laserTexture), laserSound(laserSound), shieldTexture(shieldTexture), shieldUpSound(shieldUpSound), shieldDownSound(shieldDownSound), useSounds(true)
{
    x = 1920 / 2;
    y = 1080 - 200;
}

Spaceship::~Spaceship()
{
    // Nie zwalniamy tekstur i d?wi?k�w, poniewa? s� one zwalniane w klasie Game
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
        shieldCooldown = 10.0;
        if (useSounds) {
            PlaySound(shieldDownSound);
        }
    }

    if (!shieldActive && shieldCooldown > 0) {
        shieldCooldown -= GetFrameTime();
    }
}

void Spaceship::Draw()
{
    DrawTexture(texture, x - 25, y, WHITE);

    if (shieldActive) {
        DrawTexture(shieldTexture, x - 50, y - 30, WHITE);
    }

    for (auto& laser : lasers)
    {
        laser.Draw();
    }

    // Draw the shield cooldown bar
    if (!shieldActive && shieldCooldown > 0) {
        DrawShieldCooldownBar();
    }
}

void Spaceship::FireLaser()
{
    double currentTime = GetTime();
    if (currentTime - lastFireTime >= 0.5)
    {
        lasers.emplace_back(x + width / 2, y, laserTexture);
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
    if (useSounds) {
        PlaySound(shieldUpSound);
    }
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

void Spaceship::SetShieldPurchased(bool purchased) {
    shieldPurchased = purchased;
}


double Spaceship::GetShieldCooldown() const {
    return shieldCooldown;
}

void Spaceship::DrawShieldCooldownBar()
{
    float barWidth = 100.0f;
    float barHeight = 10.0f;
    float barX = x - (barWidth / 2) + 25;
    float barY = y + height + 30;

    float cooldownPercentage = (10.0 - shieldCooldown) / 10.0 * barWidth;

    DrawRectangle(barX, barY, barWidth, barHeight, GRAY);
    DrawRectangle(barX, barY, cooldownPercentage, barHeight, BLUE);
}
