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
#include <vector>
#include "camera.h"
#include "mesh.h"
#include "camera.h"
#include "util.h"
using namespace std;
using namespace glm;


float vertices[] = {
	// positions          // colors           // texture coords
	 0.5f ,  0.5f , 0.0f,   1.0f, 1.0f, 0.0f,   1.0f, 1.0f,   // top right
	 0.5f ,  -0.5f , 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f,   // bottom right
	-0.5f , -0.5f , 0.0f,   0.0f, 0.0f, 0.0f,   0.0f, 0.0f,   // bottom left
	-0.5f ,  0.5f , 0.0f,   0.0f, 1.0f, 0.0f,   0.0f, 1.0f    // top left 
};

unsigned int indices[] = {
	0, 1, 3, // first triangle
	1, 2, 3  // second triangle
};

class Billboard {
private:
	const float M_PI = 3.14159;

public:

	Billboard() {}

	unsigned int VAOQuads, VBOQuads, EBOQuads;

	void generateBuffers() {
		glGenVertexArrays(1, &VAOQuads);
		glGenBuffers(1, &VBOQuads);
		glGenBuffers(1, &EBOQuads);

		glBindVertexArray(VAOQuads);

		glBindBuffer(GL_ARRAY_BUFFER, VBOQuads);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOQuads);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		// color attribute
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

	void drawObjectCoin(vec3 center, vec3 position, vec3 size, Shader* myShader, Model* object) {
		Util::dontMoveBegin(myShader);		
		
		glActiveTexture(GL_TEXTURE0);
		mat4 projection = perspective(radians(45.0f), 1920.0f / 1080.0f, 0.1f, 1000.0f);
		myShader->setMat4("projection", projection);

		mat4 model = mat4(1.0f);
		model = translate(model, vec3(position.x, position.y - 2, position.z - 7));
		model = rotate(model, 1 * (float)glfwGetTime(), vec3(0,1,0));
		model = scale(model, size);
		myShader->setMat4("model", model);

		object->Draw(myShader);

		glBindVertexArray(VAOQuads); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

	}

	void drawFigure(Shader* myShader, int  texture, vec3 size, float sx, float sy) {
		Util::dontMoveBegin(myShader);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ZERO);

		float z_score = -0.11;

		myShader->setInt("myTexture1", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture);

		mat4 projection = perspective(radians(45.0f), 1920.0f / 1080.0f, 0.1f, 1000.0f);
		myShader->setMat4("projection", projection);

		mat4 transformation1 = mat4(1.0f);
		transformation1 = translate(transformation1, vec3(sx, sy, z_score));
		transformation1 = scale(transformation1, size);
		

		glBindVertexArray(VAOQuads); // seeing as we only have a single VAO there's no need to bind it every time, but we'll do so to keep things a bit more organized
		myShader->setMat4("model", transformation1);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glDisable(GL_BLEND);
	}
};
