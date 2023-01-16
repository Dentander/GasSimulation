#include <GL\glew.h>
#include <GL\glu.h>
#include <GL\glut.h>
#include "engine/graphics/Engine.h"


int main() {
    std::cout << "Hello world!\n";
    Engine engine;
    engine.run();
    return EXIT_SUCCESS;
}