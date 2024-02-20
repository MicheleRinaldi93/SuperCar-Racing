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

const float M_PI = 3.14159;

const float skyboxVertices[] = {
	// positions
	-1.0f, 1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, -1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, -1.0f, 1.0f,
	-1.0f, -1.0f, 1.0f,

	-1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, -1.0f,
	1.0f, 1.0f, 1.0f,
	1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, -1.0f,
	1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, 1.0f,
	1.0f, -1.0f, 1.0f
};

class skyBox {
public:
	skyBox() {
		unsigned int texture;
		vec3 traslate;
		float radius;
		float latitude;
		float longitude;
	}

	skyBox(unsigned int text, vec3 tras,  float r, float lat, float longi) {
		texture = text;
		traslate = tras;
		radius = r;
		latitude = lat;
		longitude = longi;
	}

	virtual vector<float> init()const;
	virtual vector<unsigned int> indici()const;
	unsigned int generateBuffer(unsigned int sphereVAO, unsigned int sphereEBO, unsigned int sphereVBO, vector<float> vettori, vector<unsigned int> indici)const;
	unsigned int generateCubeBuffer(unsigned int skyboxVAO, unsigned int skyboxVBO)const;
	virtual void draw(Shader* myShader, unsigned int sphereVAO, int indici)const;
	unsigned int loadCubemap(vector<std::string> faces);
	void drawCube(Shader* shader, unsigned int VAO, unsigned int texture);

	unsigned int texture;
	vec3 traslate;
	float radius;
	float latitude;
	float longitude;
};

vector<float> skyBox::init()const {

	float r = radius;
	int longi = longitude;
	int lat = latitude;
	vector<float> list;
	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / r;			// vertex normal
	float s, t;
	float longiStep = 2 * M_PI / longi;
	float latStep = M_PI / lat;
	float longiAngle, latAngle;
	for (float i = 0; i <= lat; i++) {
		latAngle = M_PI / 2 - i * latStep;        // starting from pi/2 to -pi/2
		xy = r * cos(latAngle);				// r * cos(u)
		z = r * sin(latAngle);
		for (float j = 0; j <= longi; j++) {

			longiAngle = j * longiStep;           // starting from 0 to 2pi

			x = xy * cos(longiAngle);             // r * cos(u) * cos(v)
			y = xy * sin(longiAngle);             // r * cos(u) * sin(v)

			list.push_back(x);
			list.push_back(y);
			list.push_back(z);

			// vertex tex coord (s, t) range between [0, 1]
			s = (float)j / longi;
			t = (float)i / lat;

			list.push_back(s);
			list.push_back(t);
		}
	}
	return list;
}

vector<unsigned int> skyBox::indici()const {
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

unsigned int skyBox::generateBuffer(unsigned int sphereVAO, unsigned int sphereEBO, unsigned int sphereVBO, vector<float> vettori, vector<unsigned int> indici)const {
	glGenVertexArrays(1, &sphereVAO);
	glGenBuffers(1, &sphereVBO);
	glGenBuffers(1, &sphereEBO);

	glBindVertexArray(sphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, sphereVBO);
	glBufferData(GL_ARRAY_BUFFER, vettori.size() * 4, vettori.data(), GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sphereEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indici.size() * 4, indici.data(), GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//text coord
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	return sphereVAO;
}

void skyBox::draw(Shader* shader, unsigned int VAO, int ind)const {
	mat4 model = mat4(1.0);
	model = translate(model, vec3(traslate.x, traslate.y, traslate.z));
	//model = rotate(model, 90.0f, vec3(0.0, 1.0, 0.0));
	model = rotate(model, 30.0f, vec3(0.0, 1.0, 1.0));  //Per sky1.jpg

	shader->setMat4("model", model);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, ind, GL_UNSIGNED_INT, 0);
}

//-----SkyBox Cube-------------

unsigned int skyBox::generateCubeBuffer(unsigned int skyboxVAO, unsigned int skyboxVBO)const {
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	return skyboxVAO;
}

unsigned int skyBox::loadCubemap(vector<string> faces)
{
	unsigned int textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces.size(); i++) {
		unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);
		}
		else {
			std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
			stbi_image_free(data);
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;
}

void skyBox::drawCube(Shader* shader, unsigned int VAO, unsigned int textureCube) {
	
	glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureCube);

	mat4 model = scale(model, vec3(2.0f, 2.0f, 2.0f));
	shader->setMat4("model", model);

	glDepthMask(GL_FALSE);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
	glBindVertexArray(0);
	
	/*glBindVertexArray(VAO);
	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureCube);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);*/
}