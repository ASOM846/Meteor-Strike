#ifndef BUTTON_HPP
#define BUTTON_HPP

#include <string>
#include <raylib.h>

class Button {
public:
    float x, y, width, height;
    std::string label;
    Color color;
    bool isToggled; // Nowa zmienna do przechowywania stanu przycisku

    // Konstruktor domy˜lny
    Button();

    // Konstruktor do inicjalizacji przycisku
    Button(float x, float y, float width, float height, const std::string& label);

    // Metoda do rysowania przycisku
    void Draw() const;

    // Metoda do sprawdzania, czy przycisk zostaˆ klikni©ty
    bool IsClicked() const;

    // Metoda do sprawdzania, czy kursor myszy znajduje si© nad przyciskiem
    bool IsHovered() const;

    // Metoda do aktualizacji etykiety przycisku
    void SetLabel(const std::string& newLabel);

    // Metoda do aktualizacji stanu przycisku (zmiana koloru przy najechaniu)
    void Update();

    // Metoda do przeˆ¥czania stanu przycisku
    void Toggle();

    // Metoda do ustawiania pozycji przycisku
    void SetPosition(float newX, float newY);

    // Metody do pobierania szeroko˜ci i wysoko˜ci przycisku
    float GetWidth() const;
    float GetHeight() const;
};

#endif
