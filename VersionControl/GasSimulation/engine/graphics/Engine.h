#pragma once
#include "Visualization.h"
#include "Map.h"
#include "Input.h"
#include "Camera.h"
#include "OutputShader.h"
#include <SFML/Graphics.hpp>


class Engine {
private:
    GameState gameState = gameActive;
    Input& input = Input::instance();
    OutputShader shader;
    Camera camera;
    Map map;

    Visualization visualization;

    bool wasFocused = false;

    void updateAndDrawOpenGL();
    void eventsUpdate();
    void update();
    void draw();

protected:
    int width = 1000, height = 600;
    sf::RenderWindow window;

public:
    Engine();

    void run();
    void exit();

    int getWidth() const;
    int getHeight() const;
    vec2f getSize() const;
};

