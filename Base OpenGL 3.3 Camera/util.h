#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <iostream>
#include <string>
#include <GL/glut.h>
#include <windows.h>
#include <math.h>
#include "shader_s.h"
#include "camera.h"

using namespace std;
using namespace glm;

const float SKIJET_SIZE = 90;
const float SKIJET_STEP = 12;
const int N_OBJECTS = 8;
const int N_BONUS = 10;
const int N_OBSTACLES = 10;
const int N_WATERS = 8;
const int N_TERRAINS = 8;
const float SEA_LEVEL = 0;
const int DROPS_PER_TIME = 1;
const float OFFSET_COLLISION = 2.0;
const int N_GENERICS = 1;
const int N_PALMS = 8;
const float WATER_SIZE = 100;
const float TERRAIN_SIZE = 600.0f;
const float TERRAIN_Z_OFFSET = 0.0f;
// settings
//const unsigned int SCR_WIDTH = 1768;
//const unsigned int SCR_HEIGHT = 1024;
const float FAR_PLANE = 1500.0f;
const float MAX_ANIM = 130;
const float H_JS = -1.2;

class Util {
public:

	Util() {}
	static float RangedRand(float range_min, float range_max);
	static string getBasePath(const string& path);
	static void dontMoveBegin(Shader* myShader);
	static mat4 transformBilboard(mat4 matrixTrans);
	static float baricentro(vec3 p1, vec3 p2, vec3 p3, vec2 pos);
};

float Util::RangedRand(float range_min, float range_max)
{
	float u = (double)rand() / (RAND_MAX + 1) * (range_max - range_min) + range_min;
	return u;
}

void Util::dontMoveBegin(Shader* myShader) {
	vec3 pos = vec3(0.0, 0.0, 0.0);
	vec3 at = vec3(0.0, 0.0, -1.0);
	vec3 up = vec3(0.0, 1.0, 0.0);
	mat4 view = lookAt(pos, at, up);
	myShader->setMat4("view", view);
}

mat4 Util::transformBilboard(mat4 matrixTrans) {
	return rotate(matrixTrans, 3.1416f / 2.0f, vec3(1.0f, 0.0f, 0.0f));
}

string Util::getBasePath(const string& path)
{
	size_t pos = path.find_last_of("\\/");
	return (string::npos == pos) ? "" : path.substr(0, pos + 1);
}

float Util::baricentro(vec3 p1, vec3 p2, vec3 p3, vec2 pos) {
	float det = (p2.z - p3.z) * (p1.x - p3.x) + (p3.x - p2.x) * (p1.z - p3.z);
	float l1 = ((p2.z - p3.z) * (pos.x - p3.x) + (p3.x - p2.x) * (pos.y - p3.z)) / det;
	float l2 = ((p3.z - p1.z) * (pos.x - p3.x) + (p1.x - p3.x) * (pos.y - p3.z)) / det;
	float l3 = 1.0f - l1 - l2;
	return l1 * p1.y + l2 * p2.y + l3 * p3.y;
}
