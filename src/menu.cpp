#include "menu.hpp"

Menu::Menu(int screenWidth, int screenHeight, Game* game)
    : screenWidth(screenWidth), screenHeight(screenHeight), game(game),
    startButton(100, 100, 200, 50, "Start"),
    upgradesButton(100, 200, 200, 50, "Upgrades"),
    settingsButton(100, 300, 200, 50, "Settings"),
    exitButton(100, 400, 200, 50, "Exit"),
    mainMenu(100, 100, 200, 50, "Main Menu"),
    toggleShipGraphics(100, 200, 200, 50, "Toggle Ship Graphics"),
    toggleAsteroidGraphics(100, 300, 200, 50, "Toggle Asteroid Graphics"),
    toggleSounds(100, 400, 200, 50, "Toggle Sounds"),
    changeResolution(100, 500, 200, 50, "Change Resolution"),
    toggleFullscreen(100, 600, 200, 50, "Toggle Fullscreen"),
    upgradeSpeed(100, 100, 200, 50, "Upgrade Speed"),
    downgradeSpeed(100, 200, 200, 50, "Downgrade Speed"),
    purchaseShield(100, 300, 200, 50, "Purchase Shield"),
    upgradeShieldTime(100, 400, 200, 50, "Upgrade Shield Time"),
    backToMenu(100, 500, 200, 50, "Back to Menu"),
    useShipGraphics(true), useAsteroidGraphics(true), useSounds(true), isFullscreen(false) {
}

void Menu::Update(int& gameMode) {
    if (gameMode == 0) { // Main menu
        startButton.Update();
        upgradesButton.Update();
        settingsButton.Update();
        exitButton.Update();

        if (startButton.IsClicked()) {
            gameMode = 1; // Start game
        }
        else if (upgradesButton.IsClicked()) {
            gameMode = 2; // Upgrades menu
        }
        else if (settingsButton.IsClicked()) {
            gameMode = 3; // Settings menu
        }
        else if (exitButton.IsClicked()) {
            CloseWindow(); // Exit game
        }
    }
    else if (gameMode == 2) { // Upgrades menu
        upgradeSpeed.Update();
        downgradeSpeed.Update();
        purchaseShield.Update();
        upgradeShieldTime.Update();
        backToMenu.Update();

        if (upgradeSpeed.IsClicked()) {
            game->UpgradeSpeed();
        }
        else if (downgradeSpeed.IsClicked()) {
            game->DowngradeSpeed();
        }
        else if (purchaseShield.IsClicked()) {
            game->PurchaseShield();
        }
        else if (upgradeShieldTime.IsClicked()) {
            game->UpgradeShieldTime();
        }
        else if (backToMenu.IsClicked()) {
            gameMode = 0; // Back to main menu
        }
    }
}

void Menu::Draw(int gameMode) {
    if (gameMode == 0) { // Main menu
        startButton.Draw();
        upgradesButton.Draw();
        settingsButton.Draw();
        exitButton.Draw();
    }
    else if (gameMode == 2) { // Upgrades menu
        upgradeSpeed.Draw();
        downgradeSpeed.Draw();
        purchaseShield.Draw();
        upgradeShieldTime.Draw();
        backToMenu.Draw();
    }
}
