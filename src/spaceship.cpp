#include "spaceship.hpp"

Spaceship::Spaceship()
    : width(50), height(50), useSounds(true)
{
    x = 1920 / 2;
    y = 1080 - 200;
}

Spaceship::Spaceship(Texture2D shipTexture, Texture2D laserTexture, Texture2D ultTexture, Sound laserSound, Texture2D shieldTexture, Sound shieldUpSound, Sound shieldDownSound)
    : width(50), height(50), texture(shipTexture), laserTexture(laserTexture), ultTexture(ultTexture), laserSound(laserSound), shieldTexture(shieldTexture), shieldUpSound(shieldUpSound), shieldDownSound(shieldDownSound), useSounds(true)
{
    x = 1920 / 2;
    y = 1080 - 200;
}

Spaceship::~Spaceship()
{
    // Nie zwalniamy tekstur i d?wi?k?w, poniewa? s? one zwalniane w klasie Game
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

    //// Draw the shield cooldown bar only if the shield is purchased
    //if (shieldPurchased && !shieldActive && shieldCooldown > 0) {
    //    DrawShieldCooldownBar();
    //}
}

void Spaceship::FireLaser() {
    double currentTime = GetTime();
    if (currentTime - lastFireTime >= 0.5) {
        if (laserCount == 1) {
            // Fire from the center
            lasers.emplace_back(x + width / 2, y, laserTexture);
        }
        else if (laserCount == 2) {
            // Fire from the wings
            lasers.emplace_back(x, y, laserTexture);               // Left wing
            lasers.emplace_back(x + width, y, laserTexture);       // Right wing
        }
        else if (laserCount >= 3) {
            // Fire from both center and wings
            lasers.emplace_back(x + width / 2, y, laserTexture);   // Center
            lasers.emplace_back(x, y, laserTexture);               // Left wing
            lasers.emplace_back(x + width, y, laserTexture);       // Right wing
        }

        if (this->useSounds) {
            PlaySound(laserSound);
        }
        lastFireTime = currentTime;
    }
}

void Spaceship::FireUltLaser()
{
    double currentTime = GetTime();
    if (currentTime - lastFireTime >= 0.0 and GetUlts() > 0) {
        lasers.emplace_back(x + width / 2, y, ultTexture);
		DecreaseUlts();
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

void Spaceship::Move(){
    if (IsKeyDown(KEY_RIGHT)|| IsKeyDown(KEY_D)) {
        x += speed;
    }
    if (IsKeyDown(KEY_LEFT)|| IsKeyDown(KEY_A)) {
        x -= speed;
    }
    if (IsKeyDown(KEY_UP)|| IsKeyDown(KEY_W)) {
        y -= speed;
    }
    if (IsKeyDown(KEY_DOWN)|| IsKeyDown(KEY_S)) {
        y += speed;
    }
}

void Spaceship::CheckIfOffScreen()
{
    // Swap horizontally (left and right)
    if (x + width > 1920) 
        x = 0; // Move to the left edge
    else if (x < 0) 
        x = 1920 - width; // Move to the right edge

    // Restrict vertically (top and bottom)
    if (y < 0) 
        y = 0; // Prevent moving above the top edge
    if (y + height > 1080) 
        y = 1080 - height; // Prevent moving below the bottom edge
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

bool Spaceship::IsShieldPurchased() const {
    return shieldPurchased;
}

void Spaceship::SetShieldLevel(int level) {
    shieldDuration = 5.0 + (level - 1) * 2.0;
}

void Spaceship::IncreaseShieldTime() {
    if (GetShieldLevel() < 5) {
        shieldDuration += 2.0;
    }
}

void Spaceship::IncreaseLaserCount() {
    laserCount++;
}

void Spaceship::SetLaserCount(int count) {
    laserCount = count;
}

int Spaceship::GetLaserCount() const {
    return laserCount;
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

int Spaceship::GetUlts() const
{
    return ults;
}

void Spaceship::SetUlts(int level)
{
    ults = level;
}

void Spaceship::IncreaseUlts()
{
    if (ults < maxUlts) {
        ults++;
    }
}

void Spaceship::DecreaseUlts()
{
    if (ults > 0) {
        ults--;
    }
}

int Spaceship::GetMaxUlts() const
{
    return maxUlts;
}

void Spaceship::SetMaxUlts(int level)
{
    maxUlts = level;
}

int Spaceship::GetX() const {
    return x;
}

int Spaceship::GetY() const {
    return y;
}

void Spaceship::ResetPosition() {
    x = 1920 / 2;
    y = 1080 - 200;
}
