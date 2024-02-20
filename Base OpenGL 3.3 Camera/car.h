#pragma once
#include <iostream>
#include <stdlib.h>
#include <string>
#include <vector>
#include <queue>
#include "glm/glm.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader_s.h"

using namespace glm;
using namespace std;

enum Direction {
	CAR_FORWARD,
	CAR_BACKWARD,
	CAR_LEFT,
	CAR_RIGHT
};

class Car {
public:

	Car(){
		vec3 position;
		vec3 front;
		float yaw;

		float speed;
		float angularVelocity;
		float acceleration;
		float staticAcceleration;
		float maxSpeed;

		queue<float> historyYaw;
		int delayFrameNum=12;
		float delayYaw;

		queue<vec3> historyPosition;
		vec3 delayPosition;
	}

	Car(vec3 pos): yaw(-60.0f), delayYaw(0.0f), speed(0.0f), angularVelocity(50.0f), acceleration(8.0f), staticAcceleration(15.0f), maxSpeed(15.0f) {
		position = pos;
		while (historyPosition.size())
			historyPosition.pop();
		while (historyYaw.size())
			historyYaw.pop();
		updateFront();
	}

	virtual vec3 getPosition();
	virtual float getYaw();
	virtual float getDelayYaw();
	virtual float getYawDif();
	virtual float getMidValYaw();
	virtual vec3 getMidValPosition();
	virtual mat4 getViewMatrix(vec3 cameraPosition);
	virtual void ProcessKeyboard(Direction direction, float deltaTime);
	virtual void ProcessKeyboard_static(float detaTime);
	virtual void UpdateDelayYaw();
	virtual void UpdateDelayPosition();
	virtual void setYaw(float delay);
	virtual void setAngularVelocity(float angularVel);
	virtual void setMaxSpeed(float speedMax);
	virtual void setAcceleration(float accel);
	virtual void setFront(vec3 f);

	vec3 position;
	vec3 front;
	float yaw;

	float speed;
	float angularVelocity;
	float acceleration;
	float staticAcceleration;
	float maxSpeed;;

	queue<float> historyYaw;
	int delayFrameNum;
	float delayYaw;

	queue<vec3> historyPosition;
	vec3 delayPosition;

private:

	void updateFront()
	{
		vec3 Front;
		Front.x = -sin(radians(getDelayYaw()));
		Front.y = 0.0f;
		Front.z = -cos(radians(getDelayYaw()));
		front = normalize(Front);
	}
};

void Car::setFront(vec3 f) {
	front = f;
}

vec3 Car::getPosition(){
    return position;
}

float Car::getYaw(){
    return yaw;
}

float Car::getDelayYaw(){
    return delayYaw;
}

void Car::setYaw(float delay) {
	yaw = delay;
	//yaw = yaw;
}

void Car::setMaxSpeed(float speedMax) {
	maxSpeed = speedMax;
}

void Car::setAngularVelocity(float angularVel) {
	angularVelocity = angularVel;
}

void Car::setAcceleration(float accel) {
	acceleration = accel;
}

float Car::getYawDif(){
    return yaw - delayYaw;
}

float Car::getMidValYaw()
{
    return (delayYaw + yaw) / 2;
}

vec3 Car::getMidValPosition()
{
    return (delayPosition + position)/2.0f;
}

mat4 Car::getViewMatrix(vec3 cameraPosition)
{
    return lookAt(cameraPosition, position, vec3(0.0f, 1.0f, 0.0f));
}

void Car::ProcessKeyboard(Direction direction, float deltaTime)
{
	if (direction == CAR_FORWARD) {
		if (speed < maxSpeed)
			speed += acceleration * deltaTime;
		position += front * speed * deltaTime;
	}
	if (direction == CAR_BACKWARD) {
		if (speed > (-1)* maxSpeed)
			speed -= acceleration * deltaTime;
		if (speed > 0) {
			speed -= acceleration * deltaTime;
		}
		position += front * speed * deltaTime;
	}
	if (direction == CAR_LEFT)
		yaw += angularVelocity * deltaTime;
	if (direction == CAR_RIGHT)
		yaw -= angularVelocity * deltaTime;
	updateFront();
}

void Car::UpdateDelayYaw()
{;
    historyYaw.push(yaw);
    while (historyYaw.size() > delayFrameNum) {
        historyYaw.pop();
    }
    delayYaw = historyYaw.front();
}

void Car::UpdateDelayPosition()
{
    historyPosition.push(position);
    while (historyPosition.size() > delayFrameNum) {
        historyPosition.pop();
    }
    delayPosition = historyPosition.front();
}

void Car::ProcessKeyboard_static(float deltaTime) {
	if (speed > 0 && speed - staticAcceleration * deltaTime > 0) {
		speed -= staticAcceleration * deltaTime;
		position += front * speed * deltaTime;
	}
	else if (speed < 0 && speed + staticAcceleration * deltaTime < 0) {
		speed += staticAcceleration * deltaTime;
		position += front * speed * deltaTime;
	}
	else if (speed < 0 && speed + staticAcceleration * deltaTime > 0) {
		speed = 0;
		position += front * speed * deltaTime;
	}
	else if (speed > 0 && speed - staticAcceleration * deltaTime < 0) {
		speed = 0;
		position += front * speed * deltaTime;
	}
	updateFront();
}