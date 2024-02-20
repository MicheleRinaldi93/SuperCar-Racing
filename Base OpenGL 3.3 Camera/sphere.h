#pragma once
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include "glm/glm.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader_s.h"

using namespace glm;
using namespace std;

const float PI = 3.14159;

class Sphere {
public:
	Sphere() {
		unsigned int texture;
		vec3 traslate;
		float radius;
		float latitude;
		float longitude;
	}

	Sphere(unsigned int text, float r, float lat, float longi) {
		texture = text;
		radius = r;
		latitude = lat;
		longitude = longi;
	}

	virtual vector<float> initSphere()const;
	virtual vector<unsigned int> indici()const;
	unsigned int generateBuffer(unsigned int sphereVAO, unsigned int sphereEBO, unsigned int sphereVBO, vector<float> vettori, vector<unsigned int> indici)const;
	virtual void drawSphere(Shader* myShader, vec3 position, unsigned int sphereVAO, int indici)const;
	virtual void drawSkyBox(Shader* myShader, unsigned int sphereVAO, int indici)const;

	unsigned int texture;
	vec3 traslate;
	float radius;
	float latitude;
	float longitude;
};

vector<float> Sphere::initSphere()const {

	float r = radius;
	int longi = longitude;
	int lat = latitude;
	vector<float> list;
	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / r;			// vertex normal
	float s, t;
	float longiStep = 2 * PI / longi;
	float latStep = PI / lat;
	float longiAngle, latAngle;
	for (float i = 0; i <= lat; i++) {
		latAngle = PI / 2 - i * latStep;        // starting from pi/2 to -pi/2
		xy = r * cos(latAngle);				// r * cos(u)
		z = r * sin(latAngle);
		for (float j = 0; j <= longi; j++) {

			longiAngle = j * longiStep;           // starting from 0 to 2pi

			x = xy * cos(longiAngle);             // r * cos(u) * cos(v)
			y = xy * sin(longiAngle);             // r * cos(u) * sin(v)

			list.push_back(x);
			list.push_back(y);
			list.push_back(z);

			// normalized vertex normal (nx, ny, nz)
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;

			list.push_back(nx);
			list.push_back(ny);
			list.push_back(nz);

			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / longi;
			t = (float)i / lat;

			list.push_back(s);
			list.push_back(t);
		}
	}
	return list;
}

vector<unsigned int> Sphere::indici()const {
	vector<unsigned int> indices;
	float sectorCount = longitude;
	float stackCount = latitude;
	unsigned int k1, k2;
	for (int i = 0; i < stackCount; ++i)
	{
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
		{
			// 2 triangles per sector excluding 1st and last stacks
			if (i != 0)
			{
				indices.push_back(k1);
				indices.push_back(k2);
				indices.push_back(k1 + 1);
				//addIndices(k1, k2, k1 + 1);   // k1---k2---k1+1
			}

			if (i != (stackCount - 1))
			{
				indices.push_back(k1 + 1);
				indices.push_back(k2);
				indices.push_back(k2 + 1);
				//addIndices(k1 + 1, k2, k2 + 1); // k1+1---k2---k2+1
			}
		}
	}
	return indices;
}

unsigned int Sphere::generateBuffer(unsigned int sphereVAO, unsigned int sphereEBO, unsigned int sphereVBO, vector<float> vettori, vector<unsigned int> indici)const {
	glGenVertexArrays(1, &sphereVAO);
	glGenBuffers(1, &sphereVBO);
	glGenBuffers(1, &sphereEBO);

	glBindVertexArray(sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, vettori.size() * 4, vettori.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indici.size() * 4, indici.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//Normal coord
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	//text coord
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return sphereVAO;
}

void Sphere::drawSphere(Shader* myShader, vec3 position, unsigned int VAO, int ind)const {
	mat4 model = mat4(1.0);
	model=translate(model,vec3(position.x, position.y, position.z));
	model = scale(model,vec3(radius,radius,radius));
	myShader->setMat4("model", model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, ind, GL_UNSIGNED_INT, 0);
}

void Sphere::drawSkyBox(Shader* myShader, unsigned int VAO, int ind)const {
	mat4 model = mat4(1.0);
	model = translate(model, vec3(traslate.x, traslate.y, traslate.z));
	model = rotate(model, 90.0f, vec3(0.0, 1.0, 0.0));
	myShader->setMat4("model", model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, ind, GL_UNSIGNED_INT, 0);
}