#ifndef MENU_HPP
#define MENU_HPP

#include <raylib.h>
#include "game.hpp"
#include "button.hpp"

class Menu {
public:
    Menu(int screenWidth, int screenHeight, Game* game);
    void Update(int& gameMode);
    void Draw(int gameMode);

private:
    Game* game;
    int screenWidth;
    int screenHeight;

    // Start menu buttons
    Button startButton;
    Button upgradesButton;
    Button settingsButton;
    Button exitButton;

    // Settings menu buttons
    Button mainMenu;
    Button toggleShipGraphics;
    Button toggleAsteroidGraphics;
    Button toggleSounds;
    Button changeResolution;
    Button toggleFullscreen;

    // Upgrades menu buttons
    Button upgradeSpeed;
    Button downgradeSpeed;
    Button purchaseShield;
    Button upgradeShieldTime;
    Button backToMenu;

    bool useShipGraphics;
    bool useAsteroidGraphics;
    bool useSounds;
    bool isFullscreen;
};

#endif // MENU_HPP
