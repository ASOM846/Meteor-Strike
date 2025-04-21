#include "game.hpp"
#include "spaceship.hpp"
#include "asteroid.hpp"
#include "laser.hpp"
#include <raylib.h>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <cmath>

Game::Game(int* gameMode) : gameMode(gameMode), asteroidSpawnTimer(0.0f), asteroidSpawnInterval(static_cast<float>(GetRandomValue(0.5f, 1.5f)))
{
    InitAudioDevice();
    LoadTextures();
    spaceship = Spaceship(shipTexture, laserTexture, ultTexture, laserSound, shieldTexture, shieldUpSound, shieldDownSound);
    InitializeAsteroids(5);
    backgroundTexture = LoadTexture("graphics/bcg.jpg");
    std::srand(static_cast<unsigned int>(std::time(nullptr)));
    GenerateObjective();
    LoadStuffFromFile(); // Load game state including current level
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
    if (IsKeyPressed(KEY_E)) {
        spaceship.FireUltLaser();
    }
    if (IsKeyPressed(KEY_Q) && spaceship.IsShieldPurchased() && GetShieldCooldown() <= 0) { // Klawisz do aktywacji tarczy
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
        money += 100; // Nagroda za uko?czenie celu
        GenerateObjective(); // Generowanie nowego celu
    }
    if (levelTime >= levelDuration) {
        *gameMode = 4; // Przej?cie do ekranu uko?czenia poziomu
        if (currentLevel == levelsCompleted) {
            levelsCompleted++;
        }
    }

    UpdateUI();
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

    DisplayLevelProgress();

    if (paused) {
        int screenWidth = 1920;
        int screenHeight = 1080;

        const char* pausedText = "PAUSED";
        const char* titleText = "SPACE SHOOTER";

        int pausedTextWidth = MeasureText(pausedText, 60);
        int titleTextWidth = MeasureText(titleText, 60);

        DrawText(pausedText, screenWidth / 2 - pausedTextWidth / 2, screenHeight / 2 - 30, 60, YELLOW);
        DisplayHighScore();
        DisplayMoney();
    }

    DrawUI();
}

void Game::DrawUI()
{
    int screenWidth = 1920;
    int screenHeight = 1080;
    float padding = 15.0f; // Staˆy odst©p

    // Pasek zdrowia
    float healthBarWidth = (screenWidth / 2.0f) - (1.5f * padding);
    float healthBarHeight = 30.0f;
    float healthBarX = padding;
    float healthBarY = screenHeight - healthBarHeight - padding;

    float healthPercentage = static_cast<float>(lives) / static_cast<float>(maxLives);
    float healthBarCurrentWidth = healthBarWidth * healthPercentage;

    DrawRectangle(healthBarX, healthBarY, healthBarWidth, healthBarHeight, GRAY);
    DrawRectangle(healthBarX, healthBarY, healthBarCurrentWidth, healthBarHeight, RED);

    // Pasek tarczy (ˆadowanie lub rozˆadowanie)
    float shieldBarWidth = (screenWidth / 4.0f) - (1.5f * padding);
    float shieldBarHeight = healthBarHeight;
    float shieldBarX = healthBarX + healthBarWidth + padding;
    float shieldBarY = healthBarY;

    float shieldPercentage;
    if (spaceship.IsShieldActive()) {
        // Discharging: Remaining shield time
        float remainingTime = spaceship.GetShieldEndTime() - GetTime();
        shieldPercentage = remainingTime / spaceship.GetShieldDuration();
    }
    else {
        // Charging: Cooldown time
        shieldPercentage = (10.0f - spaceship.GetShieldCooldown()) / 10.0f;
    }

    // Ensure the percentage is within bounds
    if (shieldPercentage < 0.0f) shieldPercentage = 0.0f;
    if (shieldPercentage > 1.0f) shieldPercentage = 1.0f;

    DrawRectangle(shieldBarX, shieldBarY, shieldBarWidth, shieldBarHeight, GRAY);
    DrawRectangle(shieldBarX, shieldBarY, shieldBarWidth * shieldPercentage, shieldBarHeight, spaceship.IsShieldActive() ? BLUE : BLUE);

    // Pasek ult¢w
    float ultBarWidth = shieldBarWidth;
    float ultBarHeight = healthBarHeight;
    float ultBarX = shieldBarX + shieldBarWidth + padding;
    float ultBarY = healthBarY;

    float ultPercentage = static_cast<float>(spaceship.GetUlts()) / static_cast<float>(spaceship.GetMaxUlts());
    float ultBarCurrentWidth = ultBarWidth * ultPercentage;

    DrawRectangle(ultBarX, ultBarY, ultBarWidth, ultBarHeight, GRAY);
    DrawRectangle(ultBarX, ultBarY, ultBarCurrentWidth, ultBarHeight, GREEN);
}


void Game::UpdateUI()
{
    // Mo?esz doda? tutaj logik? aktualizacji UI, je?li jest to potrzebne
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
        asteroidSpawnInterval = static_cast<float>(GetRandomValue(0.5f, 1.5f));
    }
}

void Game::CheckCollisions() {
    for (auto& asteroid : asteroids) {
        if (!asteroid.IsActive()) continue;

        for (auto& laser : spaceship.GetLasers()) {
            if (laser.active && CheckCollisionRecs(asteroid.GetRect(), laser.GetRect())) {
                if (asteroid.IsHpUp()) {
                    IncreaseLives();
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
                asteroid.Hit();
                laser.active = false;
                break;
            }
        }

        if (CheckCollisionRecs(asteroid.GetRect(), spaceship.GetRect())) {
            if (spaceship.IsShieldActive()) {
                asteroid.Deactivate();
                continue;
            }

            if (asteroid.IsHpUp()) {
                IncreaseLives();
                if (currentObjective == "Collect 15 health packs") {
                    objectiveProgress++;
                }
            }
            else if (asteroid.IsUltUp()) {
                spaceship.IncreaseUlts(); // Zwi?ksz zmiennÒ ults
            }
            else {
                DecreaseLives();
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
    spaceship.ResetPosition();
    lives = 3;
    score = 0;
    paused = false;
    asteroids.clear();
    InitializeAsteroids(5);
    levelTime = 0.0f;
    GenerateObjective();
}

void Game::IncreaseLives() {
    if (lives < maxLives) {
        lives++;
    }
}

void Game::DecreaseLives()
{
	lives--;
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

void Game::DisplayLevelProgress() {
    float progressBarWidth = (1920/2);  
    float progressBarHeight = 20.0f;
    float progressBarX = (1920 - progressBarWidth) / 2;
    float progressBarY = 50.0f;

    float progressPercentage = (levelTime / levelDuration) * progressBarWidth;

    DrawRectangle(progressBarX, progressBarY, progressBarWidth, progressBarHeight, GRAY);
    DrawRectangle(progressBarX, progressBarY, progressPercentage, progressBarHeight, BLUE);
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
    ultTexture = LoadTexture("graphics/ult.png");
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
    Asteroid::ultUpTexture = LoadTexture("graphics/ultUp.png");
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
    UnloadTexture(ultTexture);
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

void Game::SaveStuffToFile() {
    std::ofstream file("save.txt");
    if (file.is_open()) {
        file << highScore << std::endl;
        file << money << std::endl;
        file << GetSpeedLevel() << std::endl;
        file << GetShieldLevel() << std::endl;
        file << currentLevel << std::endl;
        file << levelsCompleted << std::endl;
        file << spaceship.GetLaserCount() << std::endl;
        file << spaceship.IsShieldPurchased() << std::endl;
        file << maxLives << std::endl; // Dodajemy zapisywanie maxLives
        file.close();
    }
}

void Game::LoadStuffFromFile() {
    std::ifstream file("save.txt");
    if (file.is_open()) {
        int speedLevel, shieldLevel, laserCount;
        bool shieldPurchased;
        file >> highScore;
        file >> money;
        file >> speedLevel;
        file >> shieldLevel;
        file >> currentLevel;
        file >> levelsCompleted;
        file >> laserCount;
        file >> shieldPurchased;
        file >> maxLives; // Dodajemy wczytywanie maxLives
        file.close();

        for (int i = 1; i < speedLevel; ++i) {
            spaceship.IncreaseSpeed();
        }
        for (int i = 1; i < shieldLevel; ++i) {
            spaceship.IncreaseShieldTime();
        }

        spaceship.SetLaserCount(laserCount);
        spaceship.SetShieldPurchased(shieldPurchased);
    }
}

void Game::PurchaseShield()
{
    if (!spaceship.IsShieldPurchased() && money >= 500) {
        money -= 500;
        spaceship.SetShieldPurchased(true); // Ustawiamy flag? w Spaceship
        SaveStuffToFile();
    }
}

void Game::UpgradeShieldTime()
{
    if (money >= 300 && spaceship.GetShieldLevel() < 5) { // Zwi?ksz koszt ulepszenia czasu tarczy
        money -= 300;
        spaceship.IncreaseShieldTime();
        SaveStuffToFile();
    }
}

void Game::UpgradeMaxLives() {
    if (money >= 300 && maxLives < 10) { // Koszt ulepszenia i maksymalna liczba ?y?
        money -= 300;
        maxLives++;
        SaveStuffToFile();
    }
}

void Game::UpgradeLaserCount() {
    if (money >= laserUpgradeCost) {
        money -= laserUpgradeCost;
        spaceship.IncreaseLaserCount();
        laserUpgradeCost = static_cast<int>(laserUpgradeCost * log2(laserUpgradeCost + 1)); // Logarytmiczny wzrost kosztu
        SaveStuffToFile();
    }
}

void Game::UpgradeSpeed()
{
    if (money >= 200 && GetSpeedLevel() < 5) { // Zwi?ksz koszt ulepszenia pr?dko?ci
        money -= 200;
        spaceship.IncreaseSpeed();
        SaveStuffToFile();
    }
}

void Game::DowngradeSpeed()
{
    if (spaceship.GetSpeed() > 7) {
        money += 100; // Zwi?ksz zwrot pieni?dzy za obni?enie pr?dko?ci
        spaceship.DecreaseSpeed();
        SaveStuffToFile();
    }
}

bool Game::IsShieldPurchased() const {
    return spaceship.IsShieldPurchased();
}

void Game::SetLevel(int level) {
    currentLevel = level;
    levelDuration = 120.0f; // Czas trwania poziomu w sekundach
    levelTime = 0.0f; // Resetujemy czas poziomu
    Reset(); // Resetujemy stan gry
}

int Game::GetMaxLives() const {
    return maxLives;
}

int Game::GetLevel() const {
    return currentLevel;
}

int Game::GetLaserCount() const {
    return spaceship.GetLaserCount();
}

int Game::GetLaserUpgradeCost() const {
    return laserUpgradeCost;
}

bool Game::IsLevelCompleted() const {
    return CheckObjectiveCompleted();
}

void Game::GenerateObjective() {
    int objectiveType = GetRandomValue(0, 1);
    if (objectiveType == 0) {
        currentObjective = "Collect 15 health packs";
        objectiveCount = 15;
    }
    else if (objectiveType == 1 && currentLevel >= 2) {
        currentObjective = "Destroy 20 brown asteroids";
        objectiveCount = 20;
    }
    objectiveProgress = 0;
}

bool Game::CheckObjectiveCompleted() const {
    return objectiveProgress >= objectiveCount;
}

