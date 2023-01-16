#pragma once
#include "enums/GameState.h"
#include "../math/vec3.h"
#include "../patterns/Singleton.h"
#include "../settings/PlayerControls.h"


class Input : public Singleton<Input> {
public:
    PlayerControls& playerControls = PlayerControls::instance();
    vec2f deltaAngle = vec2f(0);
    vec2f moveDirection = vec2f(0);

    void update(sf::RenderWindow& window, bool wasFocused);
private:
    void keysUpdate();
    void mouseUpdate(sf::RenderWindow& window, bool wasFocused);
    static bool isOneKeyPressed(const sf::Keyboard::Key keys[]);
};

