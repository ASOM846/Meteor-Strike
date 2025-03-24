#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include <raylib.h>

class Asteroid {
public:
    Asteroid();
    ~Asteroid();

    void Update();
    void Draw(bool useGraphics);
    Rectangle GetRect() const;
    bool IsActive() const;
    void Deactivate();
    bool IsHpUp() const; // Dodaj metod? do sprawdzania, czy asteroida to hpUp

    static void LoadTextures();
    static void UnloadTextures();

    static Texture2D bigTexture1;
    static Texture2D bigTexture2;
    static Texture2D bigTexture3;
    static Texture2D bigTexture4;
    static Texture2D medTexture1;
    static Texture2D medTexture2;
    static Texture2D smallTexture1;
    static Texture2D smallTexture2;
    static Texture2D tinyTexture1;
    static Texture2D tinyTexture2;
    static Texture2D hpUpTexture; // Dodaj tekstur? hpUp

private:
    float x, y;
    float width, height;
    float speed;
    bool active;
    bool isHpUp; // Dodaj zmienn¤ do przechowywania stanu hpUp
    Texture2D texture;
};

#endif // ASTEROID_HPP
