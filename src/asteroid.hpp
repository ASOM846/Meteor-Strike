#ifndef ASTEROID_HPP
#define ASTEROID_HPP

#include <raylib.h>

class Asteroid {
public:
    Asteroid(int level = 1);
    ~Asteroid();
    void Update();
    void Draw();
    Rectangle GetRect() const;
    bool IsActive() const;
    void Deactivate();
    bool IsHpUp() const;
    void Hit();
    void ChangeToGrayTexture();
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
    static Texture2D hpUpTexture;
    static Texture2D brownBigTexture1;
    static Texture2D brownBigTexture2;
    static Texture2D brownBigTexture3;
    static Texture2D brownBigTexture4;
    static Texture2D brownMedTexture1;
    static Texture2D brownMedTexture2;
    static Texture2D brownSmallTexture1;
    static Texture2D brownSmallTexture2;
    static Texture2D brownTinyTexture1;
    static Texture2D brownTinyTexture2;
    float x, y;
    float width, height;
    float speed;
    bool active;
    bool isHpUp;
    bool isGray;
    int hitsRequired;
    int currentHits;
    Texture2D texture;
};

#endif // ASTEROID_HPP
