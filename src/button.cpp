#include "button.hpp"

Color Green = Color{ 25, 25, 112, 255 }; // Midnight Blue
Color HoverColor = Color{ 72, 61, 139, 255 }; // Dark Slate Blue
Color PressedColor = Color{ 47, 79, 79, 255 }; // Dark Slate Gray

// Implementacja konstruktora domy˜lnego
Button::Button() : x(0), y(0), width(100), height(50), label("Button"), color(Green), isToggled(false) {
}

Button::Button(float x, float y, float width, float height, const std::string& label)
    : x(x), y(y), width(width), height(height), label(label), color(Green), isToggled(false) {
}

void Button::Draw() const {
    // Rysowanie prostok¥ta (tˆo przycisku)
    DrawRectangle(x, y, width, height, color);

    // Obliczanie szeroko˜ci tekstu
    int textWidth = MeasureText(label.c_str(), 20);
    // Obliczanie pozycji tekstu, aby byˆ wycentrowany
    int textX = x + (width - textWidth) / 2;
    int textY = y + (height - 20) / 2;

    // Rysowanie tekstu etykiety na ˜rodku przycisku
    DrawText(label.c_str(), textX, textY, 20, WHITE);
}

bool Button::IsClicked() const {
    // Sprawdzanie, czy pozycja myszy znajduje si© w obszarze przycisku i czy przycisk myszy zostaˆ klikni©ty
    return CheckCollisionPointRec(GetMousePosition(), { x, y, width, height }) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON);
}

bool Button::IsHovered() const {
    // Sprawdzanie, czy pozycja myszy znajduje si© w obszarze przycisku
    return CheckCollisionPointRec(GetMousePosition(), { x, y, width, height });
}

void Button::SetLabel(const std::string& newLabel) {
    // Aktualizacja etykiety przycisku nowym tekstem
    label = newLabel;
}

void Button::Update() {
    // Zmiana koloru przycisku, gdy kursor myszy znajduje si© nad przyciskiem
    if (CheckCollisionPointRec(GetMousePosition(), { x, y, width, height })) {
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            color = PressedColor;
        }
        else {
            color = HoverColor;
        }
    }
    else {
        color = Green;
    }
}

void Button::Toggle() {
    // Usuni©cie poprzedniego stanu "ON" lub "OFF"
    size_t pos = label.find(" : OFF");
    if (pos != std::string::npos) {
        label = label.substr(0, pos);
    }
    pos = label.find(" : ON");
    if (pos != std::string::npos) {
        label = label.substr(0, pos);
    }

    // Przeˆ¥czenie stanu
    isToggled = !isToggled;
    if (isToggled) {
        SetLabel(label + " : OFF");
    }
    else {
        SetLabel(label + " : ON");
    }
}

void Button::SetPosition(float newX, float newY) {
    x = newX;
    y = newY;
}

float Button::GetWidth() const {
    return width;
}

float Button::GetHeight() const {
    return height;
}
