#include "game.hpp"
#include "spaceship.hpp"
#include "asteroid.hpp"
#include "laser.hpp"
#include <raylib.h>

Game::Game(int* gameMode) : gameMode(gameMode), asteroidSpawnTimer(0.0f), asteroidSpawnInterval(GetRandomValue(0.5f, 1.5f))
{
    InitAudioDevice();
    LoadTextures();
    spaceship = Spaceship(shipTexture, laserTexture, laserSound);
    InitializeAsteroids(5);
    backgroundTexture = LoadTexture("graphics/bcg.jpg");
}

Game::~Game()
{
    UnloadTextures();
    UnloadTexture(backgroundTexture);
    CloseAudioDevice();
}

void Game::Update()
{
    spaceship.Update();
    if (IsKeyPressed(KEY_SPACE)) {
        spaceship.FireLaser(useShipGraphics, useSounds);
    }
    for (auto& asteroid : asteroids)
    {
        asteroid.Update();
    }
    UpdateAsteroidSpawn();
    CheckCollisions();
}

void Game::Draw()
{
    DrawTexture(backgroundTexture, 0, 0, WHITE);

    spaceship.Draw(useShipGraphics);
    for (auto& asteroid : asteroids)
    {
        asteroid.Draw(useAsteroidGraphics);
    }
    DisplayLives();
}

void Game::InitializeAsteroids(int count)
{
    for (int i = 0; i < count; ++i)
    {
        asteroids.emplace_back();
    }
}

void Game::UpdateAsteroidSpawn()
{
    asteroidSpawnTimer += GetFrameTime();

    if (asteroidSpawnTimer >= asteroidSpawnInterval)
    {
        asteroids.emplace_back();
        asteroidSpawnTimer = 0.0f;
        asteroidSpawnInterval = GetRandomValue(0.5f, 1.5f);
    }
}

void Game::CheckCollisions()
{
    for (auto& asteroid : asteroids)
    {
        if (!asteroid.IsActive()) continue;

        for (auto& laser : spaceship.GetLasers())
        {
            if (laser.active && CheckCollisionRecs(asteroid.GetRect(), laser.GetRect()))
            {
                if (asteroid.IsHpUp()) {
                    lives++;
                }
                asteroid.Deactivate();
                laser.active = false;
                break;
            }
        }

        if (CheckCollisionRecs(asteroid.GetRect(), spaceship.GetRect()))
        {
            if (asteroid.IsHpUp()) {
                lives++;
            }
            else {
                lives--;
                if (lives == 1) {
                    PlayCheckEngineSound();
                }
                if (lives == 0) {
                    GameOver();
                }
            }
            asteroid.Deactivate();
        }
    }
}

void Game::GameOver()
{
    StopCheckEngineSound();
    *gameMode = 0;
    Reset();
}

void Game::Reset()
{
    lives = 3;
    asteroids.clear();
    InitializeAsteroids(5);
}

void Game::DisplayLives()
{
    DrawText(TextFormat("Lives: %i", lives), 10, 10, 60, WHITE);
}

void Game::LoadTextures()
{
    shipTexture = LoadTexture("graphics/playerShip.png");
    laserTexture = LoadTexture("graphics/laser.png");
    laserSound = LoadSound("sounds/laser.ogg");
    checkEngineSound = LoadSound("sounds/checkEngine.mp3");
    Asteroid::bigTexture1 = LoadTexture("graphics/meteorGrey_big1.png");
    Asteroid::bigTexture2 = LoadTexture("graphics/meteorGrey_big2.png");
    Asteroid::bigTexture3 = LoadTexture("graphics/meteorGrey_big3.png");
    Asteroid::bigTexture4 = LoadTexture("graphics/meteorGrey_big4.png");
    Asteroid::medTexture1 = LoadTexture("graphics/meteorGrey_med1.png");
    Asteroid::medTexture2 = LoadTexture("graphics/meteorGrey_med2.png");
    Asteroid::smallTexture1 = LoadTexture("graphics/meteorGrey_small1.png");
    Asteroid::smallTexture2 = LoadTexture("graphics/meteorGrey_small2.png");
    Asteroid::tinyTexture1 = LoadTexture("graphics/meteorGrey_tiny1.png");
    Asteroid::tinyTexture2 = LoadTexture("graphics/meteorGrey_tiny2.png");
    Asteroid::hpUpTexture = LoadTexture("graphics/hpUp.png"); // Zaˆaduj tekstur© hpUp
}

void Game::UnloadTextures()
{
    UnloadTexture(shipTexture);
    UnloadTexture(laserTexture);
    UnloadSound(laserSound);
    UnloadSound(checkEngineSound);
    UnloadTexture(Asteroid::bigTexture1);
    UnloadTexture(Asteroid::bigTexture2);
    UnloadTexture(Asteroid::bigTexture3);
    UnloadTexture(Asteroid::bigTexture4);
    UnloadTexture(Asteroid::medTexture1);
    UnloadTexture(Asteroid::medTexture2);
    UnloadTexture(Asteroid::smallTexture1);
    UnloadTexture(Asteroid::smallTexture2);
    UnloadTexture(Asteroid::tinyTexture1);
    UnloadTexture(Asteroid::tinyTexture2);
    UnloadTexture(Asteroid::hpUpTexture); // Zwolnij tekstur© hpUp
}

void Game::SetUseShipGraphics(bool use)
{
    useShipGraphics = use;
}

void Game::SetUseAsteroidGraphics(bool use)
{
    useAsteroidGraphics = use;
}

void Game::SetUseSounds(bool useSounds) {
    this->useSounds = useSounds;
    spaceship.SetUseSounds(useSounds);
    if (!useSounds) {
        StopCheckEngineSound();
    }
}

void Game::PlayCheckEngineSound()
{
    if (useSounds) {
        PlaySound(checkEngineSound);
    }
}

void Game::StopCheckEngineSound()
{
    StopSound(checkEngineSound);
}
