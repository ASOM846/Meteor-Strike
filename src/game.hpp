#ifndef GAME_HPP
#define GAME_HPP

#include <raylib.h>
#include <vector>
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

private:
    Spaceship spaceship;
    std::vector<Asteroid> asteroids;
    void InitializeAsteroids(int count);
    int lives = 3;

    float asteroidSpawnTimer;
    float asteroidSpawnInterval;
    void UpdateAsteroidSpawn();
    void CheckCollisions();
    void GameOver();
    void Reset();
    void DisplayLives();

    int* gameMode;
    bool useShipGraphics = true;
    bool useAsteroidGraphics = true;
    bool useSounds = true;

    void LoadTextures();
    void UnloadTextures();

    Texture2D shipTexture;
    Texture2D laserTexture;
    Sound laserSound;
    Texture2D backgroundTexture;
    Sound checkEngineSound;
    void PlayCheckEngineSound();
    void StopCheckEngineSound();
};

#endif // GAME_HPP
