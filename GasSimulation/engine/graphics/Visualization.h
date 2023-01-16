#pragma once
#include <GL\glew.h>
#include <GL\glu.h>
#include <GL\glut.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>
#include <SFML/Graphics.hpp>

#include "../math/vec3.h"
#include "Camera.h"
#include "Map.h"
#include "VisualizationSpheres.h"


class Visualization { 
private:
	VisualizationSpheres vs;

public:
	Visualization(vec2f size);
	Visualization() = default;

	void init(vec2f size);

	void changeSize(int w, int h);
	void changeSize(vec2f size);
	
	void clearScreen();
	void draw(Camera& user, Map& map);
};

