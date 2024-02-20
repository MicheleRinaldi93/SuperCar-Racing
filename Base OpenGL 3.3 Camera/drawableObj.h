#pragma once

#include "shader_s.h"
#include "model.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class DrawableObj {
public:

	Model* model;

	DrawableObj(string modelDirectory) {
		setIdentityTrasf();
		setModel(modelDirectory);
	}

	DrawableObj() {

	}

	void setShaders(Shader* shaderS) {
		shader = shaderS;
	}

	void setModel(string modelDir) {
		model = new Model(modelDir);
	}

	void traslate(glm::vec3 newPosition) {
		trasfMatrix = glm::translate(trasfMatrix, newPosition);
	}

	void scale(glm::vec3 scale) {
		trasfMatrix = glm::scale(trasfMatrix, scale);
	}

	void rotate(float angleInDegrees, glm::vec3 axis) {
		trasfMatrix = glm::rotate(trasfMatrix, glm::radians(angleInDegrees), glm::normalize(axis));
	}

	void setIdentityTrasf() {
		trasfMatrix = mat4(1.0);
	}

	void Draw() {
		shader->use();
		//shader->setMat4("model", trasfMatrix);
		model->Draw(shader);
		setIdentityTrasf();
	}

	Shader* getShader() {
		return shader;
	}

	Model* getModel() {
		return model;
	}

protected:
	Shader* shader;
	mat4 trasfMatrix;

private:


};
