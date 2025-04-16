#ifndef GAME_HPP
#define GAME_HPP

#include <raylib.h>
#include <vector>
#include <fstream>
#include <string>
#include "spaceship.hpp"
#include "asteroid.hpp"
#include "laser.hpp"

class Game {
public:
    Game(int* gameMode);
    ~Game();
    void Update();
    void Draw();
    void SetUseSounds(bool use);

    int GetHighScore() const;
    int GetMoney() const;
    int GetSpeedLevel() const;
    int GetShieldLevel() const;
    double GetShieldCooldown() const;
    bool IsShieldPurchased() const;
    int GetLaserCount() const;
    int GetLaserUpgradeCost() const;

    void SaveStuffToFile();
    void LoadStuffFromFile();

    void UpgradeSpeed();
    void DowngradeSpeed();

    void PurchaseShield();
    void UpgradeShieldTime();
    void UpgradeLaserCount();

    void UpgradeMaxLives();
    int GetMaxLives() const;

    void SetLevel(int level);
    int GetLevel() const;
    bool IsLevelCompleted() const;

    void IncreaseLives();
    void DecreaseLives();

    void PauseGame();
    void ResumeGame();
    bool IsPaused() const;

    void Reset();

private:
    Spaceship spaceship;
    std::vector<Asteroid> asteroids;
    void InitializeAsteroids(int count);
    int lives = 3;
    int maxLives = 3;
    int highScore = 0;
    int score = 0;
    int money = 0;

    float asteroidSpawnTimer;
    float asteroidSpawnInterval;
    void UpdateAsteroidSpawn();
    void CheckCollisions();
    void GameOver();
    void DisplayLives();
    void DisplayScore();
    void DisplayHighScore();
    void DisplayMoney();
    void DisplayObjective();
    void DisplayLevelProgress();


    int* gameMode;
    bool useSounds = true;

    int currentLevel = 1;
    int levelsCompleted = 1;
    float levelTime = 0.0f;
    float levelDuration = 120.0f; // Czas trwania poziomu w sekundach

    void LoadTextures();
    void UnloadTextures();

    Texture2D shipTexture;
    Texture2D shieldTexture;
    Texture2D laserTexture;
    Texture2D ultTexture;
    Sound laserSound;
    Sound shieldUpSound;
    Sound shieldDownSound;
    Texture2D backgroundTexture;
    Sound checkEngineSound;
    void PlayCheckEngineSound();
    void StopCheckEngineSound();

    // Cele
    std::string currentObjective;
    int objectiveCount;
    int objectiveProgress;

    void GenerateObjective();
    bool CheckObjectiveCompleted() const;

    bool paused = false;

    int laserUpgradeCost = 100; // Pocz†tkowy koszt ulepszenia laser?w

    // Nowe funkcje i zmienne dla UI
    void DrawUI();
    void UpdateUI();
};

#endif // GAME_HPP
