#include <iostream>
#include <raylib.h>
#include "game.hpp"
#include "button.hpp"
#include "menu.hpp"

using namespace std;

void PlayRaylibLogoAnimation();

int main() {
    Color Dark_Green = Color{ 20, 160, 133, 255 };
    Color Yellow = Color{ 255, 255, 0, 255 };

    int gameMode = 0;

    const int renderWidth = 1920;
    const int renderHeight = 1080;
    int screenWidth = 1280;
    int screenHeight = 720;

    cout << "Hello World" << endl;

    InitWindow(screenWidth, screenHeight, "ASTEROID SHOOTER");

    SetWindowSize(screenWidth, screenHeight);
    SetWindowPosition((GetMonitorWidth(0) - screenWidth) / 2, (GetMonitorHeight(0) - screenHeight) / 2);
    SetTargetFPS(60);

    RenderTexture2D target = LoadRenderTexture(renderWidth, renderHeight);

    Game game(&gameMode);
    game.LoadStuffFromFile();

    // Start menu buttons
    Button startButton(screenWidth / 2 - 250, screenHeight / 2 - 100, 500, 70, "Start Game");
    Button levelsButton(screenWidth / 2 - 150, screenHeight / 2 - 10, 300, 50, "Levels");
    Button upgradesButton(screenWidth / 2 - 150, screenHeight / 2 + 50, 300, 50, "Upgrades");
    Button settingsButton(screenWidth / 2 - 150, screenHeight / 2 + 110, 300, 50, "Settings");
    Button exitButton(screenWidth / 2 - 150, screenHeight / 2 + 170, 300, 50, "Exit");

    // Settings menu buttons
    Button mainMenu(screenWidth / 2 - 150, screenHeight / 2 - 50, 300, 50, "Main Menu");
    Button toggleSounds(screenWidth / 2 - 150, screenHeight / 2 + 10, 300, 50, "Sounds");
    Button changeResolution(screenWidth / 2 - 150, screenHeight / 2 + 70, 300, 50, "Change Resolution");
    Button toggleFullscreen(screenWidth / 2 - 150, screenHeight / 2 + 130, 300, 50, "Toggle Fullscreen");

    // Upgrades menu buttons
    Button upgradeSpeed(screenWidth / 2 - 450, screenHeight / 2 - 100, 300, 50, "Upgrade Speed");
    Button downgradeSpeed(screenWidth / 2 - 450, screenHeight / 2 - 40, 300, 50, "Downgrade Speed");
    Button purchaseShield(screenWidth / 2 - 150, screenHeight / 2 - 100, 300, 50, "Purchase Shield");
    Button upgradeShieldTime(screenWidth / 2 - 150, screenHeight / 2 - 40, 300, 50, "Upgrade Shield Time");
    Button backToMenu(screenWidth / 2 - 250, screenHeight / 2 - 100, 400, 70, "Back to Menu");

    // Levels menu buttons
    Button level1Button(screenWidth / 2 - 150, screenHeight / 2 - 100, 300, 50, "Level 1");
    Button level2Button(screenWidth / 2 - 150, screenHeight / 2 - 40, 300, 50, "Level 2");
    Button level3Button(screenWidth / 2 - 150, screenHeight / 2 + 20, 300, 50, "Level 3");
    Button backToMainMenu(screenWidth / 2 - 150, screenHeight / 2 + 80, 300, 50, "Back to Main Menu");

    // Pause menu button
    Button returnToMainMenu(screenWidth / 2 - 150, screenHeight / 2 + 50, 300, 50, "Return to Main Menu");

    bool useSounds = true;
    bool isFullscreen = false;

    // Zaˆaduj tekstur© tˆa
    Texture2D backgroundTexture = LoadTexture("graphics/bcg.jpg");

    // Wywoˆaj animacj© logo raylib
    //PlayRaylibLogoAnimation();

    while (WindowShouldClose() == false) {
        // Aktualizuj pozycje przycisk¢w na podstawie aktualnej szeroko˜ci i wysoko˜ci okna
        startButton.SetPosition(screenWidth / 2 - startButton.GetWidth() / 2, screenHeight / 2 - 100);
        levelsButton.SetPosition(screenWidth / 2 - levelsButton.GetWidth() / 2, screenHeight / 2 - 10);
        upgradesButton.SetPosition(screenWidth / 2 - upgradesButton.GetWidth() / 2, screenHeight / 2 + 50);
        settingsButton.SetPosition(screenWidth / 2 - settingsButton.GetWidth() / 2, screenHeight / 2 + 110);
        exitButton.SetPosition(screenWidth / 2 - exitButton.GetWidth() / 2, screenHeight / 2 + 170);

        mainMenu.SetPosition(screenWidth / 2 - mainMenu.GetWidth() / 2, screenHeight / 2 - 50);
        toggleSounds.SetPosition(screenWidth / 2 - toggleSounds.GetWidth() / 2, screenHeight / 2 + 10);
        changeResolution.SetPosition(screenWidth / 2 - changeResolution.GetWidth() / 2, screenHeight / 2 + 70);
        toggleFullscreen.SetPosition(screenWidth / 2 - toggleFullscreen.GetWidth() / 2, screenHeight / 2 + 130);

        upgradeSpeed.SetPosition(screenWidth / 2 - 600, screenHeight / 2 - 100);
        downgradeSpeed.SetPosition(screenWidth / 2 - 600, screenHeight / 2 - 40);
        purchaseShield.SetPosition(screenWidth / 2 - 150, screenHeight / 2 - 100);
        upgradeShieldTime.SetPosition(screenWidth / 2 - 150, screenHeight / 2 - 40);
        backToMenu.SetPosition(screenWidth / 2 - backToMenu.GetWidth() / 2, screenHeight / 2 + 250);

        level1Button.SetPosition(screenWidth / 2 - level1Button.GetWidth() / 2, screenHeight / 2 - 100);
        level2Button.SetPosition(screenWidth / 2 - level2Button.GetWidth() / 2, screenHeight / 2 - 40);
        level3Button.SetPosition(screenWidth / 2 - level3Button.GetWidth() / 2, screenHeight / 2 + 20);
        backToMainMenu.SetPosition(screenWidth / 2 - backToMainMenu.GetWidth() / 2, screenHeight / 2 + 80);

        returnToMainMenu.SetPosition(screenWidth / 2 - returnToMainMenu.GetWidth() / 2, screenHeight / 2 + 50);

        if (IsKeyPressed(KEY_Q)) {
            if (gameMode == 2) {
                if (game.IsPaused()) {
                    game.ResumeGame();
                }
                else {
                    game.PauseGame();
                }
            }
            else if (gameMode == 1 || gameMode == 3 || gameMode == 5) {
                gameMode = 0; // Switch to menu mode
            }
        }

        switch (gameMode) {
        case 0: // Menu mode
            BeginDrawing();
            ClearBackground(Dark_Green);

            // Rysowanie tˆa
            DrawTexture(backgroundTexture, 0, 0, WHITE);

            // Rysowanie tytuˆu
            DrawText("SPACE SHOOTER", screenWidth / 2 - MeasureText("SPACE SHOOTER", 60) / 2, screenHeight / 4, 60, YELLOW);

            startButton.Update();
            levelsButton.Update();
            upgradesButton.Update();
            settingsButton.Update();
            exitButton.Update();

            startButton.Draw();
            levelsButton.Draw();
            upgradesButton.Draw();
            settingsButton.Draw();
            exitButton.Draw();

            DrawText(TextFormat("High Score: %i", game.GetHighScore()), screenWidth / 2 - MeasureText(TextFormat("High Score: %i", game.GetHighScore()), 30) / 2, screenHeight / 2 + 260, 30, YELLOW);
            DrawText(TextFormat("Money: %i", game.GetMoney()), screenWidth / 2 - MeasureText(TextFormat("Money: %i", game.GetMoney()), 30) / 2, screenHeight / 2 + 300, 30, YELLOW);

            if (startButton.IsClicked()) {
                gameMode = 2; // Switch to game mode
            }
            else if (levelsButton.IsClicked()) {
                gameMode = 5; // Switch to levels mode
            }
            else if (upgradesButton.IsClicked()) {
                gameMode = 3; // Switch to upgrades mode
            }
            else if (settingsButton.IsClicked()) {
                gameMode = 1; // Switch to settings mode
            }
            else if (exitButton.IsClicked()) {
                CloseWindow();
                return 0; // Exit the game
            }

            EndDrawing();
            break;

        case 1: // Settings mode
            BeginDrawing();
            ClearBackground(Dark_Green);

            // Rysowanie tˆa
            DrawTexture(backgroundTexture, 0, 0, WHITE);

            // Rysowanie tytuˆu
            DrawText("SETTINGS", screenWidth / 2 - MeasureText("SETTINGS", 60) / 2, screenHeight / 4, 60, YELLOW);

            mainMenu.Update();
            toggleSounds.Update();
            changeResolution.Update();
            toggleFullscreen.Update();

            mainMenu.Draw();
            toggleSounds.Draw();
            changeResolution.Draw();
            toggleFullscreen.Draw();

            if (mainMenu.IsClicked()) {
                gameMode = 0; // Switch to menu mode
            }
            if (toggleSounds.IsClicked()) {
                toggleSounds.Toggle();
                useSounds = !useSounds;
                game.SetUseSounds(useSounds);
            }
            if (changeResolution.IsClicked()) {
                if (screenWidth == 1280) {
                    screenWidth = 1920;
                    screenHeight = 1080;
                }
                else {
                    screenWidth = 1280;
                    screenHeight = 720;
                }
                SetWindowSize(screenWidth, screenHeight);
                SetWindowPosition((GetMonitorWidth(0) - screenWidth) / 2, (GetMonitorHeight(0) - screenHeight) / 2);
            }
            if (toggleFullscreen.IsClicked()) {
                isFullscreen = !isFullscreen;
                if (isFullscreen) {
                    ToggleFullscreen();
                }
                else {
                    SetWindowSize(screenWidth, screenHeight);
                    SetWindowPosition((GetMonitorWidth(0) - screenWidth) / 2, (GetMonitorHeight(0) - screenHeight) / 2);
                }
            }

            EndDrawing();
            break;

        case 2: // Game mode
            game.Update();

            BeginTextureMode(target);
            ClearBackground(Dark_Green);
            game.Draw();
            EndTextureMode();

            BeginDrawing();
            ClearBackground(Dark_Green);

            // Rysowanie tˆa
            DrawTexture(backgroundTexture, 0, 0, WHITE);

            DrawTexturePro(target.texture,
                Rectangle{ 0, 0, (float)target.texture.width, (float)-target.texture.height },
                Rectangle{ 0, 0, (float)screenWidth, (float)screenHeight },
                Vector2{ 0, 0 },
                0.0f,
                WHITE);

            if (game.IsPaused()) {
                returnToMainMenu.Update();
                returnToMainMenu.Draw();
                if (returnToMainMenu.IsClicked()) {
                    game.Reset();
                    gameMode = 0; // Switch to menu mode
                }
            }

            EndDrawing();
            break;

        case 3: // Upgrades mode
            BeginDrawing();
            ClearBackground(Dark_Green);

            // Rysowanie tˆa
            DrawTexture(backgroundTexture, 0, 0, WHITE);

            // Rysowanie tytuˆu
            DrawText("UPGRADES", screenWidth / 2 - MeasureText("UPGRADES", 60) / 2, screenHeight / 4, 60, YELLOW);
            DrawText(TextFormat("Money: %i", game.GetMoney()), screenWidth / 2 - MeasureText(TextFormat("Money: %i", game.GetMoney()), 30) / 2, screenHeight / 2 + 170, 30, YELLOW);

            upgradeSpeed.Update();
            downgradeSpeed.Update();
            purchaseShield.Update();
            upgradeShieldTime.Update();
            backToMenu.Update();

            upgradeSpeed.Draw();
            downgradeSpeed.Draw();
            purchaseShield.Draw();
            upgradeShieldTime.Draw();
            backToMenu.Draw();

            // Wy˜wietlanie informacji o poziomach i pieni¥dzach w jednej linii
            DrawText(TextFormat("Speed Level: %i", game.GetSpeedLevel()), 10, 10, 30, YELLOW);
            DrawText(TextFormat("Shield Level: %i", game.GetShieldLevel()), 250, 10, 30, YELLOW);

            if (game.IsShieldPurchased()) {
                DrawText("Purchased", purchaseShield.GetX() + purchaseShield.GetWidth() + 10, purchaseShield.GetY() + 10, 20, YELLOW);
            }
            else {
                DrawText("Cost: 500", purchaseShield.GetX() + purchaseShield.GetWidth() + 10, purchaseShield.GetY() + 10, 20, YELLOW);
            }

            // Wy˜wietlanie koszt¢w obok przycisk¢w
            DrawText("Cost: 200", upgradeSpeed.GetX() + upgradeSpeed.GetWidth() + 10, upgradeSpeed.GetY() + 10, 20, YELLOW);
            DrawText("Refund: 100", downgradeSpeed.GetX() + downgradeSpeed.GetWidth() + 10, downgradeSpeed.GetY() + 10, 20, YELLOW);
            DrawText("Cost: 300", upgradeShieldTime.GetX() + upgradeShieldTime.GetWidth() + 10, upgradeShieldTime.GetY() + 10, 20, YELLOW);

            if (upgradeSpeed.IsClicked()) {
                game.UpgradeSpeed();
            }
            if (downgradeSpeed.IsClicked()) {
                game.DowngradeSpeed();
            }
            if (purchaseShield.IsClicked()) {
                game.PurchaseShield();
            }
            if (upgradeShieldTime.IsClicked()) {
                game.UpgradeShieldTime();
            }
            if (backToMenu.IsClicked()) {
                gameMode = 0; // Switch to menu mode
            }

            EndDrawing();
            break;

        case 4: // Level completed mode
            BeginDrawing();
            ClearBackground(Dark_Green);

            // Rysowanie tˆa
            DrawTexture(backgroundTexture, 0, 0, WHITE);

            // Rysowanie tytuˆu
            DrawText("LEVEL COMPLETED", screenWidth / 2 - MeasureText("LEVEL COMPLETED", 60) / 2, screenHeight / 4, 60, YELLOW);

            DrawText("Press ENTER to continue", screenWidth / 2 - MeasureText("Press ENTER to continue", 30) / 2, screenHeight / 2, 30, YELLOW);

            if (IsKeyPressed(KEY_ENTER)) {
                game.SetLevel(game.GetLevel() + 1);
                gameMode = 2; // Switch to game mode
            }

            EndDrawing();
            break;

        case 5: // Levels mode
            BeginDrawing();
            ClearBackground(Dark_Green);

            // Rysowanie tˆa
            DrawTexture(backgroundTexture, 0, 0, WHITE);

            // Rysowanie tytuˆu
            DrawText("LEVELS", screenWidth / 2 - MeasureText("LEVELS", 60) / 2, screenHeight / 4, 60, YELLOW);

            level1Button.Update();
            level2Button.Update();
            level3Button.Update();
            backToMainMenu.Update();

            level1Button.Draw();
            level2Button.Draw();
            level3Button.Draw();
            backToMainMenu.Draw();

            if (level1Button.IsClicked()) {
                game.SetLevel(1);
                gameMode = 2; // Switch to game mode
            }
            if (level2Button.IsClicked()) {
                game.SetLevel(2);
                gameMode = 2; // Switch to game mode
            }
            if (level3Button.IsClicked()) {
                game.SetLevel(3);
                gameMode = 2; // Switch to game mode
            }
            if (backToMainMenu.IsClicked()) {
                gameMode = 0; // Switch to menu mode
            }

            EndDrawing();
            break;
        }
    }

    UnloadTexture(backgroundTexture);
    UnloadRenderTexture(target);
    CloseWindow();
    return 0;
}
