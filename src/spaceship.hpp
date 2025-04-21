#ifndef SPACESHIP_HPP
#define SPACESHIP_HPP

#include <raylib.h>
#include <vector>
#include "laser.hpp"

class Spaceship {
public:
    Spaceship();
    Spaceship(Texture2D shipTexture, Texture2D laserTexture, Texture2D ultTexture,
        Sound laserSound, Texture2D shieldTexture, Sound shieldUpSound, Sound shieldDownSound);

    ~Spaceship();

    void Update();
    void Draw();
    void FireLaser();
    void FireUltLaser();
    std::vector<Laser>& GetLasers();
    Rectangle GetRect() const;
    void SetUseSounds(bool useSounds);

    void IncreaseLaserCount();
    int GetLaserCount() const;
    void SetLaserCount(int count);

    void IncreaseSpeed();
    void DecreaseSpeed();
    int GetSpeed() const;

    int GetX() const;
    int GetY() const;
    void ResetPosition();

    bool IsShieldPurchased() const;
    void ActivateShield();
    bool IsShieldActive() const;
    void IncreaseShieldTime();
    int GetShieldLevel() const;
    double GetShieldCooldown() const;
    void SetShieldLevel(int level);
    void SetShieldPurchased(bool purchased);

    int GetUlts() const;
    void SetUlts(int level);
    void IncreaseUlts();
    void DecreaseUlts();

    int GetMaxUlts() const;
    void SetMaxUlts(int level);

    // Expose shield duration and end time
    double GetShieldDuration() const { return shieldDuration; }
    double GetShieldEndTime() const { return shieldEndTime; }

private:
    int x;
    int y;
    int width;
    int height;
    int speed = 6;
    int laserCount = 1;
    int ults = 1;
    int maxUlts = 3;
    std::vector<Laser> lasers;
    double lastFireTime = 0.0;
    Texture2D texture;
    Texture2D laserTexture;
    Texture2D ultTexture;
    Texture2D shieldTexture;
    Sound laserSound;
    Sound shieldUpSound;
    Sound shieldDownSound;
    bool useSounds;

    bool shieldActive = false;
    double shieldEndTime = 0.0;
    double shieldDuration = 5.0;
    double shieldCooldown = 10.0;
    bool shieldPurchased = false;

    void Move();
    void CheckIfOffScreen();
    void DrawShieldCooldownBar();
};

#endif // SPACESHIP_HPP
