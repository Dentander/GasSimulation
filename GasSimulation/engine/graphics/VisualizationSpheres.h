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



class VisualizationSpheres {
private:
	GLuint prog_hdlr;
	GLint location_attribute_0, location_viewport;

	int SCREEN_WIDTH = 1024;
	int SCREEN_HEIGHT = 1024;
	float light0_position[4] = { 1,1,1,0 };
	float angle = 0.72;

	bool readAndCompileShader(const char* filename, GLuint& hdlr, GLenum shaderType);
	void setShaders(GLuint& prog_hdlr, const char* vsfile, const char* fsfile);


public:
	VisualizationSpheres(vec2f size);
	VisualizationSpheres() = default;
	void init(vec2f size);

	void changeSize(int w, int h);
	void changeSize(vec2f size);
	void draw(Camera& user, Map& map);

	void setUniform(std::string& name, int item);
	void setUniform(std::string& name, float item);
	void setUniform(std::string& name, vec2f item);
	void setUniform(std::string& name, vec3f item);

	void setUniform(const char name[], int item);
	void setUniform(const char name[], float item);
	void setUniform(const char name[], vec2f item);
	void setUniform(const char name[], vec3f item);
};

