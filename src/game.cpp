#include "game.hpp"
#include "spaceship.hpp"
#include "asteroid.hpp"
#include "laser.hpp"
#include <raylib.h>
#include <fstream>
#include <cstdlib>
#include <ctime>

Game::Game(int* gameMode) : gameMode(gameMode), asteroidSpawnTimer(0.0f), asteroidSpawnInterval(GetRandomValue(0.5f, 1.5f))
{
    InitAudioDevice();
    LoadTextures();
    spaceship = Spaceship(shipTexture, laserTexture, laserSound, shieldTexture, shieldUpSound, shieldDownSound);
    InitializeAsteroids(5);
    backgroundTexture = LoadTexture("graphics/bcg.jpg");
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    GenerateObjective();
    LoadStuffFromFile(); // Load game state including current level
    spaceship.SetShieldPurchased(shieldPurchased); // Ustawiamy flag�
}

Game::~Game()
{
    UnloadTextures();
    UnloadTexture(backgroundTexture);
    CloseAudioDevice();
}

void Game::Update()
{
    if (paused) return;

    spaceship.Update();
    if (IsKeyPressed(KEY_SPACE)) {
        spaceship.FireLaser();
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

    // Aktualizacja czasu poziomu
    levelTime += GetFrameTime();
    if (CheckObjectiveCompleted()) {
        *gameMode = 4; // Przej�cie do ekranu uko�czenia poziomu
        if (currentLevel == levelsCompleted) {
            levelsCompleted++;
        }
    }
}

void Game::Draw()
{
    DrawTexture(backgroundTexture, 0, 0, WHITE);

    spaceship.Draw();
    for (auto& asteroid : asteroids)
    {
        asteroid.Draw();
    }
    DisplayLives();
    DisplayScore();
    DisplayObjective();

    // Display the level time
    DrawText(TextFormat("Time: %.1f", levelTime), 10, 90, 30, YELLOW);

    if (paused) {
        int screenWidth = GetScreenWidth();
        int screenHeight = GetScreenHeight();

        const char* pausedText = "PAUSED";
        const char* titleText = "SPACE SHOOTER";

        int pausedTextWidth = MeasureText(pausedText, 60);
        int titleTextWidth = MeasureText(titleText, 60);

        DrawText(pausedText, screenWidth / 2 - pausedTextWidth / 2, screenHeight / 2 - 30, 60, YELLOW);
        DrawText(titleText, screenWidth / 2 - titleTextWidth / 2, screenHeight / 4, 60, YELLOW);

        DisplayHighScore();
        DisplayMoney();
    }
}

void Game::InitializeAsteroids(int count)
{
    for (int i = 0; i < count; ++i)
    {
        asteroids.emplace_back(currentLevel); // Przekazujemy poziom do konstruktora Asteroid
    }
}

void Game::UpdateAsteroidSpawn()
{
    asteroidSpawnTimer += GetFrameTime();

    if (asteroidSpawnTimer >= asteroidSpawnInterval)
    {
        asteroids.emplace_back(currentLevel); // Przekazujemy poziom do konstruktora Asteroid
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
                    if (currentObjective == "Collect 15 health packs") {
                        objectiveProgress++;
                    }
                }
                else {
                    score++;
                    money += 10;
                    if (currentObjective == "Destroy 20 brown asteroids" &&
                        (asteroid.texture.id == Asteroid::brownBigTexture1.id ||
                            asteroid.texture.id == Asteroid::brownBigTexture2.id ||
                            asteroid.texture.id == Asteroid::brownBigTexture3.id ||
                            asteroid.texture.id == Asteroid::brownBigTexture4.id ||
                            asteroid.texture.id == Asteroid::brownMedTexture1.id ||
                            asteroid.texture.id == Asteroid::brownMedTexture2.id ||
                            asteroid.texture.id == Asteroid::brownSmallTexture1.id ||
                            asteroid.texture.id == Asteroid::brownSmallTexture2.id ||
                            asteroid.texture.id == Asteroid::brownTinyTexture1.id ||
                            asteroid.texture.id == Asteroid::brownTinyTexture2.id)) {
                        objectiveProgress++;
                    }
                }
                asteroid.Hit(); // Zamiast deaktywowa� asteroid�, obs�ugujemy trafienie
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
                if (currentObjective == "Collect 15 health packs") {
                    objectiveProgress++;
                }
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
    paused = false;
    asteroids.clear();
    InitializeAsteroids(5);
    levelTime = 0.0f;
    GenerateObjective();
}

void Game::PauseGame()
{
    paused = true;
}

void Game::ResumeGame()
{
    paused = false;
}

bool Game::IsPaused() const
{
    return paused;
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

void Game::DisplayObjective() {
    DrawText(TextFormat("Objective: %s (%i/%i)", currentObjective.c_str(), objectiveProgress, objectiveCount), 10, 130, 30, YELLOW);
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
    shieldTexture = LoadTexture("graphics/shield.png");
    laserTexture = LoadTexture("graphics/laser.png");
    laserSound = LoadSound("sounds/laser.ogg");
    shieldUpSound = LoadSound("sounds/shieldUp.ogg");
    shieldDownSound = LoadSound("sounds/shieldDown.ogg");
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
    Asteroid::brownBigTexture1 = LoadTexture("graphics/meteorBrown_big1.png");
    Asteroid::brownBigTexture2 = LoadTexture("graphics/meteorBrown_big2.png");
    Asteroid::brownBigTexture3 = LoadTexture("graphics/meteorBrown_big3.png");
    Asteroid::brownBigTexture4 = LoadTexture("graphics/meteorBrown_big4.png");
    Asteroid::brownMedTexture1 = LoadTexture("graphics/meteorBrown_med1.png");
    Asteroid::brownMedTexture2 = LoadTexture("graphics/meteorBrown_med2.png");
    Asteroid::brownSmallTexture1 = LoadTexture("graphics/meteorBrown_small1.png");
    Asteroid::brownSmallTexture2 = LoadTexture("graphics/meteorBrown_small2.png");
    Asteroid::brownTinyTexture1 = LoadTexture("graphics/meteorBrown_tiny1.png");
    Asteroid::brownTinyTexture2 = LoadTexture("graphics/meteorBrown_tiny2.png");
}

void Game::UnloadTextures()
{
    UnloadTexture(shipTexture);
    UnloadTexture(shieldTexture);
    UnloadTexture(laserTexture);
    UnloadSound(laserSound);
    UnloadSound(shieldUpSound);
    UnloadSound(shieldDownSound);
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
    UnloadTexture(Asteroid::brownBigTexture1);
    UnloadTexture(Asteroid::brownBigTexture2);
    UnloadTexture(Asteroid::brownBigTexture3);
    UnloadTexture(Asteroid::brownBigTexture4);
    UnloadTexture(Asteroid::brownMedTexture1);
    UnloadTexture(Asteroid::brownMedTexture2);
    UnloadTexture(Asteroid::brownSmallTexture1);
    UnloadTexture(Asteroid::brownSmallTexture2);
    UnloadTexture(Asteroid::brownTinyTexture1);
    UnloadTexture(Asteroid::brownTinyTexture2);
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
        file << currentLevel << std::endl; // Zapisz aktualny poziom
        file << levelsCompleted << std::endl; // Zapisz levelsCompleted
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
        file >> currentLevel;
        file.close();

        for (int i = 1; i < speedLevel; ++i) {
            spaceship.IncreaseSpeed();
        }
        for (int i = 1; i < shieldLevel; ++i) {
            spaceship.IncreaseShieldTime();
        }
    }
}

void Game::SavePurchasesToFile()
{
    std::ofstream file("purchases.txt");
    if (file.is_open()) {
        file << shieldPurchased << std::endl;
        file.close();
    }
}

void Game::LoadPurchasesFromFile()
{
    std::ifstream file("purchases.txt");
    if (file.is_open()) {
        file >> shieldPurchased;
        file.close();
    }
}

void Game::PurchaseShield()
{
    if (!shieldPurchased && money >= 500) {
        money -= 500;
        shieldPurchased = true;
        spaceship.SetShieldPurchased(shieldPurchased); // Ustawiamy flag� w Spaceship
        SavePurchasesToFile();
    }
}

void Game::UpgradeShieldTime()
{
    if (money >= 300 && spaceship.GetShieldLevel() < 5) { // Zwi�ksz koszt ulepszenia czasu tarczy
        money -= 300;
        spaceship.IncreaseShieldTime();
        SaveStuffToFile();
    }
}

void Game::UpgradeSpeed()
{
    if (money >= 200 && GetSpeedLevel() < 5) { // Zwi�ksz koszt ulepszenia pr�dko�ci
        money -= 200;
        spaceship.IncreaseSpeed();
        SaveStuffToFile();
    }
}

void Game::DowngradeSpeed()
{
    if (spaceship.GetSpeed() > 7) {
        money += 100; // Zwi�ksz zwrot pieni�dzy za obni�enie pr�dko�ci
        spaceship.DecreaseSpeed();
        SaveStuffToFile();
    }
}

bool Game::IsShieldPurchased() const {
    return shieldPurchased;
}

void Game::SetLevel(int level) {
    currentLevel = level;
    levelDuration = 60.0f * level; // Przyk�adowo, czas trwania poziomu ro�nie z ka�dym poziomem
    levelTime = 0.0f; // Resetujemy czas poziomu
    Reset(); // Resetujemy stan gry
}

int Game::GetLevel() const {
    return currentLevel;
}

bool Game::IsLevelCompleted() const {
    return CheckObjectiveCompleted();
}

void Game::GenerateObjective() {
    int objectiveType = GetRandomValue(0, 1);
    if (objectiveType == 0) {
        currentObjective = "Collect 15 health packs";
        objectiveCount = 2;
    }
    else if (objectiveType == 1 && currentLevel >= 2) {
           currentObjective = "Destroy 20 brown asteroids";
           objectiveCount = 2;
       }
    objectiveProgress = 0;
}

bool Game::CheckObjectiveCompleted() const {
    return objectiveProgress >= objectiveCount;
}
