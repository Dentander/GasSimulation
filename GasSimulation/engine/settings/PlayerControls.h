#pragma once
#include <SFML/Graphics.hpp>
#include "../patterns/Singleton.h"


class PlayerControls : public Singleton<PlayerControls> {
public:
    float mouseSensitivityX = 0.003;
    float mouseSensitivityY = 0.003;
    float movementSpeed = 1;

    sf::Keyboard::Key forward[2] = { sf::Keyboard::Key::W, sf::Keyboard::Key::W };
    sf::Keyboard::Key backward[2] = { sf::Keyboard::Key::S, sf::Keyboard::Key::S };
    sf::Keyboard::Key left[2] = { sf::Keyboard::Key::A, sf::Keyboard::Key::A };
    sf::Keyboard::Key right[2] = { sf::Keyboard::Key::D, sf::Keyboard::Key::D };
    sf::Keyboard::Key up[2] = { sf::Keyboard::Key::Space, sf::Keyboard::Key::Space };
    sf::Keyboard::Key down[2] = { sf::Keyboard::Key::C, sf::Keyboard::Key::C };
    sf::Keyboard::Key run[2] = { sf::Keyboard::Key::LShift, sf::Keyboard::Key::RShift };
};

