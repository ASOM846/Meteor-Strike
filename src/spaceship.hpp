#ifndef SPACESHIP_HPP
#define SPACESHIP_HPP

#include <raylib.h>
#include <vector>
#include "laser.hpp"

class Spaceship {
public:
    Spaceship();
    Spaceship(Texture2D shipTexture, Texture2D laserTexture, Sound laserSound, Texture2D shieldTexture, Sound shieldUpSound, Sound shieldDownSound);
    ~Spaceship();

    void Update();
    void Draw();
    void FireLaser();
    std::vector<Laser>& GetLasers();
    Rectangle GetRect() const;
    void SetUseSounds(bool useSounds);

    void IncreaseSpeed();
    void DecreaseSpeed();
    int GetSpeed() const;

    void ActivateShield();
    bool IsShieldActive() const;
    void IncreaseShieldTime();
    int GetShieldLevel() const;
    double GetShieldCooldown() const;
    void SetShieldLevel(int level);
    void SetShieldPurchased(bool purchased); // Dodajemy t© metod©

private:
    int x;
    int y;
    int width;
    int height;
    int speed = 7;
    std::vector<Laser> lasers;
    double lastFireTime = 0.0;
    Texture2D texture;
    Texture2D laserTexture;
    Texture2D shieldTexture;
    Sound laserSound;
    Sound shieldUpSound;
    Sound shieldDownSound;
    bool useSounds;

    bool shieldActive = false;
    double shieldEndTime = 0.0;
    double shieldDuration = 5.0;
    double shieldCooldown = 10.0;
    bool shieldPurchased = false; // Dodajemy t© zmienn¥

    void Move();
    void CheckIfOffScreen();
    void DrawShieldCooldownBar();
};

#endif // SPACESHIP_HPP
