#include "Engine.h"


float random(float min = 0, float max = 500) {
    min *= 10;
    max *= 10;
    return float(rand() % int((max - min + 1) + min)) / 10;
}


Engine::Engine() {
    // ======== WINDOW ========
    sf::ContextSettings settings;
    settings.depthBits = 24;
    settings.stencilBits = 8;
    settings.antialiasingLevel = 0;
    settings.majorVersion = 0;
    settings.minorVersion = 0;


    window.create(sf::VideoMode(getWidth(), getHeight()), "3DEngine", 7u, settings);
    //window.setVerticalSyncEnabled(true);
    window.setMouseCursorVisible(false);
    camera.setPosition(vec3f(0, 0, -20));

    float k = 500;
    for (int i = 0; i < 100000; ++i) {

        map.addSphere(vec3f(random() - 20, random() - 20, random() - 20), 2, 1, vec3f(random() / k, random() / k, random() / k));
    }
    /*for (int i = 0; i < 25; ++i) {
        map.addBox(vec3f(0, float(-2 -3*i), 0), vec3f(2, 1, 2), 5);
    }*/

    /*for (int i = 0; i < 20000; ++i) {

        map.addSphere(vec3f(0, float(-2 - 3 * i), 0), 2, 1, vec3f(random() / k, random() / k, random() / k));
    }*/
    map.addSphere(vec3f(3, 0, 0), 2, 1, vec3f(random() / k, random() / k, random() / k));
    map.addBox(vec3f(0, 40, 0), vec3f(50, 10, 50), 5);
    //map.addBox(vec3f(0, -40, 0), vec3f(50, 10, 50), 5);
    //map.addBox(vec3f(-40, 0, 0), vec3f(10, 50, 50), 5);
    //map.addBox(vec3f(40, 0, 0), vec3f(10, 50, 50), 5);
    //map.addBox(vec3f(0, 0, 40), vec3f(50, 50, 10), 5);
    //map.addBox(vec3f(0, 0, -40), vec3f(50, 50, 10), 5);



    // ======== SHADERS ========
    shader.build(getSize());
    shader.setUniform("u_sphere_pos", vec3f(1));
    visualization.init(getSize());
}

void Engine::updateAndDrawOpenGL() {
    visualization.changeSize(getSize());
    visualization.draw(camera, map);
}

void Engine::eventsUpdate() {
    sf::Event event{};
    while (window.pollEvent(event)) {
        // ========= CLOSE ========
        if (event.type == sf::Event::Closed) { exit(); }

        // ========= KEYBOARD ========
        if (event.type == sf::Event::KeyPressed) {
            // ======== CLOSE ========
            if (event.key.code == sf::Keyboard::Escape) { exit(); }

            // ======== GAME STATE ========
            if (event.key.code == sf::Keyboard::E) {
                gameState = gameInactive;
                window.setMouseCursorVisible(true);
            }
        }

        // ======== GAME STATE ========
        if (event.type == sf::Event::GainedFocus) {
            gameState = gameActive;
            window.setMouseCursorVisible(false);
            wasFocused = false;
        }

        // ======== RESIZE ========
        if (event.type == sf::Event::Resized) {
            width = window.getSize().x;
            height = window.getSize().y;           
        }
    }
}

void Engine::update() {
    // ======== EVENTS ========
    eventsUpdate();

    if (gameState == gameActive) {
        // ======== INPUT ========
        input.update(window, wasFocused);
        camera.update();

        // ======== MAP ========
        //map.update(window);

        // ======== UNIFORMS ========
        camera.setUniforms(shader);
        map.setUniforms(shader);

        // ======== FOCUSED ========
        wasFocused = true;
    }
}

void Engine::draw() {
    // ======== SHADER ========
    //shader.draw(window);
}

void Engine::run() {
    sf::Clock clock;
    sf::Time lastTime;
    while (window.isOpen()) {      
        // ======== UPDATE ========
        updateAndDrawOpenGL();
        window.pushGLStates();
        update();

        // ======== DEBUG ========
        sf::Time currentTime = clock.getElapsedTime();
        float fps = 1.f / (currentTime.asSeconds() - lastTime.asSeconds());
        lastTime = currentTime;
        window.setTitle(std::to_string(fps));

        // ======== DRAW ========
        draw();

        // ======== DISPLAY AND DELAY ========
        window.display();
        //window.clear();
        window.popGLStates();
    }
}

void Engine::exit() {
    window.close();
}

int Engine::getWidth() const { return width; }

int Engine::getHeight() const { return height; }

vec2f Engine::getSize() const { return vec2f((float)getWidth(), (float)getHeight()); }
