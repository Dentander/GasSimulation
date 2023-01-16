#include "Visualization.h"


Visualization::Visualization(vec2f size) {
    vs.changeSize(size);
    //vb.changeSize(size);
}

void Visualization::init(vec2f size) {
    vs.init(size);
    //vb.init(size);
}

void Visualization::changeSize(int w, int h) {
    vs.changeSize(w, h);
   // vb.changeSize(w, h);
}

void Visualization::changeSize(vec2f size) {
    vs.changeSize(size);
    //vb.changeSize(size);
}

void Visualization::clearScreen() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(1, 1, 1, 1);
}

void Visualization::draw(Camera& user, Map& map) {
    //vb.draw(user, map);
    vs.draw(user, map);
}
