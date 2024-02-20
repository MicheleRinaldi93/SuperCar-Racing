#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <cstring>
#include <string>
#include <stack>
#include "shader_s.h"
#include "camera.h"
#define STB_IMAGE_IMPLEMENTATION

using namespace std;
using namespace glm;

class Geometry {
public:

	float quadrilateral_mesh[48] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, 0.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, 0.0f,  0.0f,  0.0f, 1.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, 0.0f,  0.0f,  0.0f, 1.0f,  1.0f,  1.0f,
		 0.5f,  0.5f, 0.0f,  0.0f,  0.0f, 1.0f,  1.0f,  1.0f,
		-0.5f,  0.5f, 0.0f,  0.0f,  0.0f, 1.0f,  0.0f,  1.0f,
		-0.5f, -0.5f, 0.0f,  0.0f,  0.0f, 1.0f,  0.0f,  0.0f,
	};

	float skySize = 400.0f;

	float skyboxVertices[108] = {
		// positions
		-skySize,  skySize, -skySize,
		-skySize, -skySize, -skySize,
		 skySize, -skySize, -skySize,
		 skySize, -skySize, -skySize,
		 skySize,  skySize, -skySize,
		-skySize,  skySize, -skySize,

		-skySize, -skySize,  skySize,
		-skySize, -skySize, -skySize,
		-skySize,  skySize, -skySize,
		-skySize,  skySize, -skySize,
		-skySize,  skySize,  skySize,
		-skySize, -skySize,  skySize,

		 skySize, -skySize, -skySize,
		 skySize, -skySize,  skySize,
		 skySize,  skySize,  skySize,
		 skySize,  skySize,  skySize,
		 skySize,  skySize, -skySize,
		 skySize, -skySize, -skySize,

		-skySize, -skySize,  skySize,
		-skySize,  skySize,  skySize,
		 skySize,  skySize,  skySize,
		 skySize,  skySize,  skySize,
		 skySize, -skySize,  skySize,
		-skySize, -skySize,  skySize,

		-skySize,  skySize, -skySize,
		 skySize,  skySize, -skySize,
		 skySize,  skySize,  skySize,
		 skySize,  skySize,  skySize,
		-skySize,  skySize,  skySize,
		-skySize,  skySize, -skySize,

		-skySize, -skySize, -skySize,
		-skySize, -skySize,  skySize,
		 skySize, -skySize, -skySize,
		 skySize, -skySize, -skySize,
		-skySize, -skySize,  skySize,
		 skySize, -skySize,  skySize
	};

	float sphere_mesh[22000];

	unsigned int VBOQuad, VAOQuad;
	unsigned int VBOSphere, VAOSphere;
	unsigned int skyboxVAO, skyboxVBO;

	void init();
	void initVAOQuad();
	void initVAOSphere();
	void initVAOCubeMap();

	void drawQuad(mat4 model, Shader* myShader);
	void drawSphereTexture(mat4 model, Shader* myShader, int texture);
	void drawSky(mat4 model, Shader* myShader, unsigned int cubemaptexture);
	void drawSkyTexture(mat4 model, Shader* myShader, int t1, int t2, int t3, int t4, int t5, int t6);
	void drawCubeTexture(mat4 model, Shader* myShader, int t1, int t2, int t3, int t4, int t5, int t6);
};

void Geometry::init() {
	initVAOQuad();
	initVAOSphere();
	initVAOCubeMap();
}

void Geometry::initVAOCubeMap() {
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);

	glBindVertexArray(skyboxVAO);

	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindVertexArray(0);
}

void Geometry::initVAOQuad() {
	glGenVertexArrays(1, &VBOQuad);
	glGenBuffers(1, &VAOQuad);

	glBindVertexArray(VBOQuad);

	glBindBuffer(GL_ARRAY_BUFFER, VAOQuad);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadrilateral_mesh), quadrilateral_mesh, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normale attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}

void Geometry::drawQuad(mat4 model, Shader* myShader) {
	glBindVertexArray(VBOQuad);
	myShader->setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Geometry::drawSky(mat4 model, Shader* myShader, unsigned int cubemaptexture) {
	glDepthMask(GL_FALSE);
	glBindVertexArray(skyboxVAO);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemaptexture);
	model = scale(model, vec3(2.0f, 2.0f, 2.0f));
	myShader->setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);
	glBindVertexArray(0);
}

void Geometry::drawSkyTexture(mat4 model, Shader* myShader, int t1, int t2, int t3, int t4, int t5, int t6) {
	// back t2
	mat4 model2 = mat4(1.0f);
	model2 = translate(model, vec3(0.0f, 0.0f, 0.5f));
	model2 = rotate(model2, 3.1416f, vec3(0.0f, 1.0f, 0.0f));

	myShader->setInt("myTexture1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t2);
	drawQuad(model2, myShader);

	// up t3
	mat4 model3 = mat4(1.0f);
	model3 = translate(model, vec3(0.0f, 0.5f, 0.0f));
	model3 = rotate(model3, float(3.1416f / 2.0f), vec3(1.0f, 0.0f, 0.0f));

	myShader->setInt("myTexture1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t3);
	drawQuad(model3, myShader);

	// sinistra
	mat4 model5 = mat4(1.0f);
	model5 = translate(model, vec3(0.5f, 0.0f, 0.0f));
	model5 = rotate(model5, -float(3.1416f / 2.0f), vec3(0.0f, 1.0f, 0.0f));

	myShader->setInt("myTexture1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t6);
	drawQuad(model5, myShader);

	// destra
	myShader->setInt("myTexture1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t5);

	mat4 model6 = mat4(1.0f);
	model6 = translate(model, vec3(-0.5f, 0.0f, 0.0f));
	model6 = rotate(model6, float(3.1416f / 2.0f), vec3(0.0f, 1.0f, 0.0f));
	drawQuad(model6, myShader);
}

void Geometry::drawCubeTexture(mat4 model, Shader* myShader, int t1, int t2, int t3, int t4, int t5, int t6) {
	// front t1
	myShader->setInt("myTexture1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t1);
	glm::mat4 model1 = glm::mat4(1.0f);
	model1 = glm::translate(model, glm::vec3(0.0f, 0.0f, -0.5f));
	model1 = glm::rotate(model1, float(-3.1416f / 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	drawQuad(model1, myShader);

	// back t2
	myShader->setInt("myTexture1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t2);
	mat4 model2 = mat4(1.0f);
	model2 = translate(model, vec3(0.0f, 0.0f, 0.5f));
	model2 = rotate(model2, 3.1416f, vec3(0.0f, 1.0f, 0.0f));

	// up t3
	myShader->setInt("myTexture1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t3);
	glm::mat4 model3 = glm::mat4(1.0f);
	model3 = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
	model3 = glm::rotate(model3, float(-3.1416f / 2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	drawQuad(model3, myShader);

	// down t4
	myShader->setInt("myTexture1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t4);
	glm::mat4 model4 = glm::mat4(1.0f);
	model4 = glm::translate(model, glm::vec3(0.0f, -0.5f, 0.0f));
	model4 = glm::rotate(model4, float(3.1416f / 2.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	drawQuad(model4, myShader);

	// right t5
	myShader->setInt("myTexture1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t5);
	glm::mat4 model5 = glm::mat4(1.0f);
	model5 = glm::translate(model, glm::vec3(-0.5f, 0.0f, 0.0f));
	model5 = glm::rotate(model5, float(-3.1416f / 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	drawQuad(model5, myShader);

	// right t6
	myShader->setInt("myTexture1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, t6);
	glm::mat4 model6 = glm::mat4(1.0f);
	model6 = glm::translate(model, glm::vec3(0.5f, 0.0f, 0.0f));
	model6 = glm::rotate(model6, float(3.1416f / 2.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	drawQuad(model6, myShader);
}

void Geometry::initVAOSphere() {
	float n = 20;
	float r = 1;

	float vertx[100][100];
	float verty[100][100];
	float vertz[100][100];

	float PI = 3.1415926f;

	float parallelo = 0;
	float meridiano = 0;

	for (int i = 0; i < n + 1; i++) {
		parallelo = (PI * (i)) / n;

		for (int j = 0; j < n; j++) {
			meridiano = (2.0 * PI * j) / n;

			float x1 = r * cos(meridiano) * sin(parallelo);
			float y1 = r * sin(meridiano) * sin(parallelo);
			float z1 = r * cos(parallelo);

			vertx[i][j] = x1;
			verty[i][j] = y1;
			vertz[i][j] = z1;
		}
	}

	float passoXt = 1 / float(n);
	float passoYt = 1 / float(n);

	int k = 0;

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			sphere_mesh[k] = vertx[i][j];
			k++;
			sphere_mesh[k] = verty[i][j];
			k++;
			sphere_mesh[k] = vertz[i][j];
			k++;

			sphere_mesh[k] = vertx[i][j];
			k++;
			sphere_mesh[k] = verty[i][j];
			k++;
			sphere_mesh[k] = vertz[i][j];
			k++;

			sphere_mesh[k] = passoXt * i;
			k++;
			sphere_mesh[k] = passoYt * j;
			k++;

			if (j != n - 1) {
				sphere_mesh[k] = vertx[i + 1][j];
				k++;
				sphere_mesh[k] = verty[i + 1][j];
				k++;
				sphere_mesh[k] = vertz[i + 1][j];
				k++;

				sphere_mesh[k] = vertx[i + 1][j];
				k++;
				sphere_mesh[k] = verty[i + 1][j];
				k++;
				sphere_mesh[k] = vertz[i + 1][j];
				k++;

				sphere_mesh[k] = (passoXt * i) + passoXt;
				k++;
				sphere_mesh[k] = passoYt * j;
				k++;

				sphere_mesh[k] = vertx[i][j + 1];
				k++;
				sphere_mesh[k] = verty[i][j + 1];
				k++;
				sphere_mesh[k] = vertz[i][j + 1];
				k++;

				sphere_mesh[k] = vertx[i][j + 1];
				k++;
				sphere_mesh[k] = verty[i][j + 1];
				k++;
				sphere_mesh[k] = vertz[i][j + 1];
				k++;

				sphere_mesh[k] = (passoXt * i);
				k++;
				sphere_mesh[k] = (passoYt * j) + passoYt;
				k++;
			}
			else if (j == n - 1) {
				sphere_mesh[k] = vertx[i + 1][j];
				k++;
				sphere_mesh[k] = verty[i + 1][j];
				k++;
				sphere_mesh[k] = vertz[i + 1][j];
				k++;
				sphere_mesh[k] = vertx[i + 1][j];
				k++;
				sphere_mesh[k] = verty[i + 1][j];
				k++;
				sphere_mesh[k] = vertz[i + 1][j];
				k++;
				sphere_mesh[k] = (passoXt * i) + passoXt;
				k++;
				sphere_mesh[k] = (passoYt * j);
				k++;

				sphere_mesh[k] = vertx[i][0];
				k++;
				sphere_mesh[k] = verty[i][0];
				k++;
				sphere_mesh[k] = vertz[i][0];
				k++;
				sphere_mesh[k] = vertx[i][0];
				k++;
				sphere_mesh[k] = verty[i][0];
				k++;
				sphere_mesh[k] = vertz[i][0];
				k++;
				sphere_mesh[k] = (passoXt * i);
				k++;
				sphere_mesh[k] = (passoYt * j) + passoYt;
				k++;
			}

			if (j != n - 1) {
				sphere_mesh[k] = vertx[i][j + 1];
				k++;
				sphere_mesh[k] = verty[i][j + 1];
				k++;
				sphere_mesh[k] = vertz[i][j + 1];
				k++;
				sphere_mesh[k] = vertx[i][j + 1];
				k++;
				sphere_mesh[k] = verty[i][j + 1];
				k++;
				sphere_mesh[k] = vertz[i][j + 1];
				k++;
				sphere_mesh[k] = (passoXt * i);
				k++;
				sphere_mesh[k] = (passoYt * j) + passoYt;
				k++;

				sphere_mesh[k] = vertx[i + 1][j];
				k++;
				sphere_mesh[k] = verty[i + 1][j];
				k++;
				sphere_mesh[k] = vertz[i + 1][j];
				k++;
				sphere_mesh[k] = vertx[i + 1][j];
				k++;
				sphere_mesh[k] = verty[i + 1][j];
				k++;
				sphere_mesh[k] = vertz[i + 1][j];
				k++;
				sphere_mesh[k] = (passoXt * i) + passoXt;
				k++;
				sphere_mesh[k] = passoYt * j;
				k++;

				sphere_mesh[k] = vertx[i + 1][j + 1];
				k++;
				sphere_mesh[k] = verty[i + 1][j + 1];
				k++;
				sphere_mesh[k] = vertz[i + 1][j + 1];
				k++;
				sphere_mesh[k] = vertx[i + 1][j + 1];
				k++;
				sphere_mesh[k] = verty[i + 1][j + 1];
				k++;
				sphere_mesh[k] = vertz[i + 1][j + 1];
				k++;
				sphere_mesh[k] = (passoXt * i) + passoXt;
				k++;
				sphere_mesh[k] = (passoYt * j) + passoYt;
				k++;
			}
			else if (j == n - 1) {
				sphere_mesh[k] = vertx[i][0];
				k++;
				sphere_mesh[k] = verty[i][0];
				k++;
				sphere_mesh[k] = vertz[i][0];
				k++;
				sphere_mesh[k] = vertx[i][0];
				k++;
				sphere_mesh[k] = verty[i][0];
				k++;
				sphere_mesh[k] = vertz[i][0];
				k++;
				sphere_mesh[k] = (passoXt * i);
				k++;
				sphere_mesh[k] = (passoYt * j) + passoYt;
				k++;

				sphere_mesh[k] = vertx[i + 1][j];
				k++;
				sphere_mesh[k] = verty[i + 1][j];
				k++;
				sphere_mesh[k] = vertz[i + 1][j];
				k++;
				sphere_mesh[k] = vertx[i + 1][j];
				k++;
				sphere_mesh[k] = verty[i + 1][j];
				k++;
				sphere_mesh[k] = vertz[i + 1][j];
				k++;
				sphere_mesh[k] = (passoXt * i) + passoXt;
				k++;
				sphere_mesh[k] = (passoYt * j);
				k++;

				sphere_mesh[k] = vertx[i + 1][0];
				k++;
				sphere_mesh[k] = verty[i + 1][0];
				k++;
				sphere_mesh[k] = vertz[i + 1][0];
				k++;
				sphere_mesh[k] = vertx[i + 1][0];
				k++;
				sphere_mesh[k] = verty[i + 1][0];
				k++;
				sphere_mesh[k] = vertz[i + 1][0];
				k++;
				sphere_mesh[k] = (passoXt * i) + passoXt;
				k++;
				sphere_mesh[k] = (passoYt * j) + passoYt;
				k++;
			}
		}
	}

	glGenVertexArrays(1, &VBOSphere);
	glGenBuffers(1, &VAOSphere);

	glBindVertexArray(VBOSphere);

	glBindBuffer(GL_ARRAY_BUFFER, VAOSphere);
	glBufferData(GL_ARRAY_BUFFER, sizeof(sphere_mesh), sphere_mesh, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normale attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// texture coord attribute
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);

	// note that this is allowed, the call to glVertexAttribPointer registered VBO as the vertex attribute's bound vertex buffer object so afterwards we can safely unbind
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// You can unbind the VAO afterwards so other VAO calls won't accidentally modify this VAO, but this rarely happens. Modifying other
	// VAOs requires a call to glBindVertexArray anyways so we generally don't unbind VAOs (nor VBOs) when it's not directly necessary.
	glBindVertexArray(0);
}

void Geometry::drawSphereTexture(mat4 model, Shader* myShader, int texture) {
	myShader->setInt("myTexture1", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);

	glBindVertexArray(VBOSphere);
	model = rotate(model, -3.1416f / 2.0f, vec3(1.0f, 0.0f, 0.0f));
	myShader->setMat4("model", model);
	glDrawArrays(GL_TRIANGLES, 0, 2400);
}
