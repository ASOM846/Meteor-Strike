#ifndef SPACESHIP_HPP
#define SPACESHIP_HPP

#include <raylib.h>
#include <vector>
#include "laser.hpp"

class Spaceship {
public:
    Spaceship();
    Spaceship(Texture2D shipTexture, Texture2D laserTexture, Sound laserSound); // Dodaj nowy konstruktor
    ~Spaceship();

    void Update();
    void Draw(bool useGraphics); // Dodaj parametr useGraphics
    void FireLaser(bool useGraphics, bool useSounds); // Dodaj parametry useGraphics i useSounds
    std::vector<Laser>& GetLasers();
    Rectangle GetRect() const;
    void SetUseSounds(bool useSounds); // Dodaj metod? do ustawiania u–ycia d®wi?kôw

private:
    int x;
    int y;
    int width;
    int height;
    std::vector<Laser> lasers;
    double lastFireTime = 0.0;
    Texture2D texture; // Dodaj zmienn¤ do przechowywania tekstury statku
    Texture2D laserTexture; // Dodaj zmienn¤ do przechowywania tekstury lasera
    Sound laserSound; // Dodaj zmienn¤ do przechowywania d®wi?ku lasera
    bool useSounds; // Dodaj zmienn¤ do przechowywania stanu d®wi?kôw

    void Move();
    void CheckIfOffScreen();
};

#endif // SPACESHIP_HPP
