#include "game.hpp"
#include "spaceship.hpp"
#include "asteroid.hpp"
#include "laser.hpp"
#include <raylib.h>
#include <fstream>

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
    if (IsKeyPressed(KEY_S) && shieldPurchased && GetShieldCooldown() <= 0) { // Klawisz do aktywacji tarczy
        spaceship.ActivateShield();
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
    DisplayScore();
    DisplayHighScore();
    DisplayMoney();

    // Wy?wietlanie wska®nika czasu odnowienia tarczy
    if (shieldPurchased) {
        DrawText(TextFormat("Shield Cooldown: %.1f", GetShieldCooldown()), 10, 50, 30, YELLOW);
    }
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
                else {
                    score++;
                    money += 10;
                }
                asteroid.Deactivate();
                laser.active = false;
                break;
            }
        }

        if (CheckCollisionRecs(asteroid.GetRect(), spaceship.GetRect()))
        {
            if (spaceship.IsShieldActive()) {
                asteroid.Deactivate();
                continue;
            }

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
    if (score > highScore) {
        highScore = score;
        SaveStuffToFile();
    }
    *gameMode = 0;
    Reset();
}

void Game::Reset()
{
    lives = 3;
    score = 0;
    asteroids.clear();
    InitializeAsteroids(5);
}

void Game::DisplayLives()
{
    DrawText(TextFormat("Lives: %i", lives), 10, 10, 30, WHITE);
}

void Game::DisplayScore()
{
    DrawText(TextFormat("Score: %i", score), 200, 10, 30, WHITE);
}

void Game::DisplayHighScore()
{
    DrawText(TextFormat("High Score: %i", highScore), 400, 10, 30, WHITE);
}

void Game::DisplayMoney() {
    DrawText(TextFormat("Money: %i", money), 700, 10, 30, WHITE);
}

int Game::GetHighScore() const {
    return highScore;
}

int Game::GetMoney() const {
    return money;
}

int Game::GetSpeedLevel() const {
    return (spaceship.GetSpeed() - 7) / 2 + 1;
}

int Game::GetShieldLevel() const {
    return spaceship.GetShieldLevel();
}

double Game::GetShieldCooldown() const {
    return spaceship.GetShieldCooldown();
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
    Asteroid::hpUpTexture = LoadTexture("graphics/hpUp.png");
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
    UnloadTexture(Asteroid::hpUpTexture);
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

void Game::SaveStuffToFile()
{
    std::ofstream file("save.txt");
    if (file.is_open()) {
        file << highScore << std::endl;
        file << money << std::endl;
        file << GetSpeedLevel() << std::endl;
        file << GetShieldLevel() << std::endl;
        file << shieldPurchased << std::endl;
        file.close();
    }
}

void Game::LoadStuffFromFile()
{
    std::ifstream file("save.txt");
    if (file.is_open()) {
        int speedLevel, shieldLevel;
        file >> highScore;
        file >> money;
        file >> speedLevel;
        file >> shieldLevel;
        file >> shieldPurchased;
        file.close();

        // Ustaw poziomy ulepsze„
        for (int i = 1; i < speedLevel; ++i) {
            spaceship.IncreaseSpeed();
        }
        for (int i = 1; i < shieldLevel; ++i) {
            spaceship.IncreaseShieldTime();
        }
    }
}

void Game::PurchaseShield()
{
    if (money >= 500) { // Zwi?ksz koszt zakupu tarczy
        money -= 500;
        shieldPurchased = true;
    }
}

void Game::UpgradeShieldTime()
{
    if (money >= 300) { // Zwi?ksz koszt ulepszenia czasu tarczy
        money -= 300;
        spaceship.IncreaseShieldTime();
    }
}

void Game::UpgradeSpeed()
{
    if (money >= 200 && GetSpeedLevel() < 5) { // Zwi?ksz koszt ulepszenia pr?dko?ci
        money -= 200;
        spaceship.IncreaseSpeed();
    }
}

void Game::DowngradeSpeed()
{
    if (spaceship.GetSpeed() > 7) {
        money += 100; // Zwi?ksz zwrot pieni?dzy za obni–enie pr?dko?ci
        spaceship.DecreaseSpeed();
    }
}
