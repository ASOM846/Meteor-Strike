#include <iostream>
#include <raylib.h>
#include "game.hpp"
#include "button.hpp"

using namespace std;

void PlayRaylibLogoAnimation(); // Deklaracja funkcji

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

    Game game(&gameMode); // Przeka® wska®nik do gameMode

    //start menu buttons
    Button startButton(screenWidth / 2 - 250, screenHeight / 2 - 100, 500, 70, "Start Game");
    Button settingsButton(screenWidth / 2 - 150, screenHeight / 2 - 10, 300, 50, "Settings");
    Button exitButton(screenWidth / 2 - 150, screenHeight / 2 + 50, 300, 50, "Exit");

    //settings menu buttons
    Button mainMenu(screenWidth / 2 - 150, screenHeight / 2 - 50, 300, 50, "Main Menu");
    Button toggleShipGraphics(screenWidth / 2 - 150, screenHeight / 2 + 10, 300, 50, "Ship Graphics");
    Button toggleAsteroidGraphics(screenWidth / 2 - 150, screenHeight / 2 + 70, 300, 50, "Asteroid Graphics");
    Button toggleSounds(screenWidth / 2 - 150, screenHeight / 2 + 130, 300, 50, "Sounds");
    Button changeResolution(screenWidth / 2 - 150, screenHeight / 2 + 190, 300, 50, "Change Resolution");
    Button toggleFullscreen(screenWidth / 2 - 150, screenHeight / 2 + 250, 300, 50, "Toggle Fullscreen");

    bool useShipGraphics = true;
    bool useAsteroidGraphics = true;
    bool useSounds = true;
    bool isFullscreen = false;

    // Za?aduj tekstur? t?a
    Texture2D backgroundTexture = LoadTexture("graphics/bcg.jpg");

    // Wywo?aj animacj? logo raylib
    PlayRaylibLogoAnimation();

    while (WindowShouldClose() == false) {
        // Aktualizuj pozycje przyciskôw na podstawie aktualnej szeroko?ci i wysoko?ci okna
        startButton.SetPosition(screenWidth / 2 - startButton.GetWidth() / 2, screenHeight / 2 - 100);
        settingsButton.SetPosition(screenWidth / 2 - settingsButton.GetWidth() / 2, screenHeight / 2 - 10);
        exitButton.SetPosition(screenWidth / 2 - exitButton.GetWidth() / 2, screenHeight / 2 + 50);

        mainMenu.SetPosition(screenWidth / 2 - mainMenu.GetWidth() / 2, screenHeight / 2 - 50);
        toggleShipGraphics.SetPosition(screenWidth / 2 - toggleShipGraphics.GetWidth() / 2, screenHeight / 2 + 10);
        toggleAsteroidGraphics.SetPosition(screenWidth / 2 - toggleAsteroidGraphics.GetWidth() / 2, screenHeight / 2 + 70);
        toggleSounds.SetPosition(screenWidth / 2 - toggleSounds.GetWidth() / 2, screenHeight / 2 + 130);
        changeResolution.SetPosition(screenWidth / 2 - changeResolution.GetWidth() / 2, screenHeight / 2 + 190);
        toggleFullscreen.SetPosition(screenWidth / 2 - toggleFullscreen.GetWidth() / 2, screenHeight / 2 + 250);

        switch (gameMode) {
        case 0: // Menu mode
            BeginDrawing();
            ClearBackground(Dark_Green);

            // Rysowanie t?a
            DrawTexture(backgroundTexture, 0, 0, WHITE);

            // Rysowanie tytu?u
            DrawText("SPACE SHOOTER", screenWidth / 2 - MeasureText("SPACE SHOOTER", 60) / 2, screenHeight / 4, 60, YELLOW);

            startButton.Update();
            settingsButton.Update();
            exitButton.Update();

            startButton.Draw();
            settingsButton.Draw();
            exitButton.Draw();

            if (startButton.IsClicked()) {
                gameMode = 2; // Switch to game mode
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

            // Rysowanie t?a
            DrawTexture(backgroundTexture, 0, 0, WHITE);

            // Rysowanie tytu?u
            DrawText("SETTINGS", screenWidth / 2 - MeasureText("SETTINGS", 60) / 2, screenHeight / 4, 60, YELLOW);

            mainMenu.Update();
            toggleShipGraphics.Update();
            toggleAsteroidGraphics.Update();
            toggleSounds.Update();
            changeResolution.Update();
            toggleFullscreen.Update();

            mainMenu.Draw();
            toggleShipGraphics.Draw();
            toggleAsteroidGraphics.Draw();
            toggleSounds.Draw();
            changeResolution.Draw();
            toggleFullscreen.Draw();

            if (mainMenu.IsClicked()) {
                gameMode = 0; // Switch to menu mode
            }
            if (toggleShipGraphics.IsClicked()) {
                toggleShipGraphics.Toggle();
                useShipGraphics = !useShipGraphics;
                game.SetUseShipGraphics(useShipGraphics);
            }
            if (toggleAsteroidGraphics.IsClicked()) {
                toggleAsteroidGraphics.Toggle();
                useAsteroidGraphics = !useAsteroidGraphics;
                game.SetUseAsteroidGraphics(useAsteroidGraphics);
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

            // Rysowanie t?a
            DrawTexture(backgroundTexture, 0, 0, WHITE);

            DrawTexturePro(target.texture,
                Rectangle{ 0, 0, (float)target.texture.width, (float)-target.texture.height },
                Rectangle{ 0, 0, (float)screenWidth, (float)screenHeight },
                Vector2{ 0, 0 },
                0.0f,
                WHITE);

            EndDrawing();
            break;
        }
    }

    // Zwolnij zasoby tekstury t?a
    UnloadTexture(backgroundTexture);
    UnloadRenderTexture(target);
    CloseWindow();
    return 0;
}
