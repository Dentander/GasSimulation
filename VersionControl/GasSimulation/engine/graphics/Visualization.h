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



class Visualization { 
private:
	GLuint prog_hdlr;
	GLint location_attribute_0,
		location_viewport,
		location_resolution,
		location_player_pos,
		location_player_rot;
	int NATOMS = 1000;
	int SCREEN_WIDTH = 1024;
	int SCREEN_HEIGHT = 1024;
	float camera[3] = { .6,0,1 };
	float light0_position[4] = { 1,1,1,0 };
	std::vector<std::vector<float> > atoms;
	float cur_camera[3] = { 0,0,0 };
	float angle = 0.72;

	bool readAndCompileShader(const char* filename, GLuint& hdlr, GLenum shaderType);
	void setShaders(GLuint& prog_hdlr, const char* vsfile, const char* fsfile);


public:
	Visualization(vec2f size);
	Visualization() = default;
	void init(vec2f size);

	void changeSize(int w, int h);
	void changeSize(vec2f size);
	void draw();
	void draw(Camera& user, Map& map);
};

