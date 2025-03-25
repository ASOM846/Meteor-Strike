#ifndef GAME_HPP
#define GAME_HPP

#include <raylib.h>
#include <vector>
#include <fstream>
#include "spaceship.hpp"
#include "asteroid.hpp"
#include "laser.hpp"

class Game {
public:
    Game(int* gameMode);
    ~Game();

    void Update();
    void Draw();
    void SetUseShipGraphics(bool use);
    void SetUseAsteroidGraphics(bool use);
    void SetUseSounds(bool use);

    void DisplayHighScore();
    void DisplayMoney();

    int GetHighScore() const;
    int GetMoney() const;
    int GetSpeedLevel() const;
    int GetShieldLevel() const;
    double GetShieldCooldown() const;
    bool IsShieldPurchased() const;

    void SaveStuffToFile();
    void LoadStuffFromFile();

    void UpgradeSpeed();
    void DowngradeSpeed();

    void PurchaseShield();
    void UpgradeShieldTime();

private:
    Spaceship spaceship;
    std::vector<Asteroid> asteroids;
    void InitializeAsteroids(int count);
    int lives = 3;
    int highScore = 0;
    int score = 0;
    int money = 0;

    float asteroidSpawnTimer;
    float asteroidSpawnInterval;
    void UpdateAsteroidSpawn();
    void CheckCollisions();
    void GameOver();
    void Reset();
    void DisplayLives();
    void DisplayScore();

    int* gameMode;
    bool useShipGraphics = true;
    bool useAsteroidGraphics = true;
    bool useSounds = true;
    bool shieldPurchased = false;

    void LoadTextures();
    void UnloadTextures();

    Texture2D shipTexture;
    Texture2D shieldTexture;
    Texture2D laserTexture;
    Sound laserSound;
    Texture2D backgroundTexture;
    Sound checkEngineSound;
    void PlayCheckEngineSound();
    void StopCheckEngineSound();

    void SavePurchasesToFile();
    void LoadPurchasesFromFile();
};

#endif // GAME_HPP
