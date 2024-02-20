#ifndef CAMERA_H
#define CAMERA_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "include/GLFW/glfw3.h"

#include <vector>
#include <deque>

using namespace glm;
using namespace std;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	ROTATE_LEFT,
	ROTATE_RIGHT,
	UP,
	DOWN,
	STOPPED
};

// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 80.0f;
const float ZOOM_RANGE = 20.0f;
const float ZOOM_SPEED = 2.0f;
const float TurnSpeed = 80.0f;


// An abstract camera class that processes input and calculates the corresponding Euler Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Camera Attributes
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;

	// Euler Angles
	float Yaw;
	float Pitch;

	// Camera options
	float MovementSpeed;
	float MouseSensitivity;
	float Zoom;

	// Data about camera
	Camera_Movement actual_direction;
	float actual_velocity;
	bool fixed = false;
	float FixedYaw = 0.0f;
	float radius = 1.0f;
	float fov;
	float deltaTime = 0.0f;
	float currentTime;
	float lastTime = 0.0f;
	bool shakeflag = true;
	bool shake = false;
	std::deque<float>lastFrontX;
	std::deque<float>lastFrontZ;

	// Constructor with vectors
	Camera(glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f), float yaw = YAW, float pitch = PITCH) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = position;
		WorldUp = up;
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}
	// Constructor with scalar values
	Camera(float posX, float posY, float posZ, float upX, float upY, float upZ, float yaw, float pitch) : Front(glm::vec3(0.0f, 0.0f, -1.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVITY), Zoom(ZOOM)
	{
		Position = glm::vec3(posX, posY, posZ);
		WorldUp = glm::vec3(upX, upY, upZ);
		Yaw = yaw;
		Pitch = pitch;
		updateCameraVectors();
	}

	// Returns the view matrix calculated using Euler Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(Position, Position + Front, Up);
	}

	glm::mat4 GetProjMatrix(float aspect)
	{
		return glm::perspective(glm::radians(Zoom), aspect, 0.1f, 1000.0f);
	}

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, float deltaTime)
	{
		float velocity = MovementSpeed * deltaTime * 10.0f;
		if (direction == FORWARD) {
			//actual_direction = direction;
			Position += Front * velocity;
		}
		if (direction == BACKWARD) {
			//actual_direction = direction;
			Position -= Front * velocity;
		}
		if (direction == LEFT) {
			//actual_direction = direction;
			Position -= Right * velocity;
		}
		if (direction == RIGHT) {
			//actual_direction = direction;
			Position += Right * velocity;
		}
		if (direction == UP) {
			actual_direction = direction;
			Position = Position + velocity * Up;
		}
		if (direction == DOWN) {
			actual_direction = direction;
			Position = Position - velocity * Up;
		}
		velocity /= 10;
		if (direction == ROTATE_RIGHT) {
			actual_direction = direction;
			actual_velocity = -velocity;
			glm::vec3 normal = { 0.0, 1.0, 0.0 };
			glm::vec3 parallel = dot(Front, normal) * normal;
			Front = cos(-velocity) * (Front - parallel) + (sin(-velocity) * cross(normal, Front) + parallel);
			Right = cross(Front, Up);
			normalize(Right);
		}
		if (direction == ROTATE_LEFT) {
			actual_direction = direction;
			actual_velocity = velocity;
			glm::vec3 normal = { 0.0, 1.0, 0.0 };
			glm:: vec3 parallel = dot(Front, normal) * normal;
			Front = cos(velocity) * (Front - parallel) + (sin(velocity) * cross(normal, Front) + parallel);
			Right = cross(Front, Up);
			normalize(Right);
		}
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(float xoffset, float yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= MouseSensitivity;
		yoffset *= MouseSensitivity;

		Yaw += xoffset;
		Pitch += yoffset;

		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (Pitch > 89.0f)
				Pitch = 89.0f;
			if (Pitch < -89.0f)
				Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Euler angles
		updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(float yoffset)
	{
		if (Zoom >= 1.0f && Zoom <= 45.0f)
			Zoom -= yoffset;
		if (Zoom <= 1.0f)
			Zoom = 1.0f;
		if (Zoom >= 45.0f)
			Zoom = 45.0f;
	}

	/*void FixView(glm::vec3 position, float yaw)
	{
		Position = position;
		Yaw = yaw;
		Pitch = 0.0f;
		updateCameraVectors();
	}*/

	void ZoomIn() {
		if (Zoom >= ZOOM - ZOOM_RANGE)
			Zoom -= ZOOM_SPEED;
	}

	void ZoomOut(){
		if (Zoom <= ZOOM + ZOOM_RANGE)
			Zoom += ZOOM_SPEED;
	}

	void ZoomRecover()
	{
		if (Zoom < ZOOM)
			Zoom += ZOOM_SPEED / 2;
		if (Zoom > ZOOM)
			Zoom -= ZOOM_SPEED / 2;
	}

	void CameraMove(GLFWwindow* window)
	{
		//Esc
		if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
			glfwSetWindowShouldClose(window, true);

		float cameraSpeed = /*2.5f*/4 * deltaTime;
		if (!fixed && glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
			Position += cameraSpeed * Front;
		if (!fixed && glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
			Position -= cameraSpeed * Front;
		if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
			if (!fixed)
				Position -= normalize(cross(Front, Up)) * cameraSpeed;
			else
				FixedYaw -= TurnSpeed * deltaTime;

		}
		if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
			if (!fixed)
				Position += normalize(cross(Front, Up)) * cameraSpeed;
			else
				FixedYaw += TurnSpeed * deltaTime;
		}
		if (!fixed && glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
			Position -= cameraSpeed * Up;
		if (!fixed && glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
			Position += cameraSpeed * Up;
		if (fixed) {
			Front.x = radius * sin(radians(YAW));
			Front.y = 0;
			Front.z = radius * cos(radians(YAW));
			Front = normalize(Front);
		}
	}

	void ForwardEffect()
	{
		if (fov >= 70.0f)
			fov -= 0.2f;
	}

	void BackEffect()
	{
		if (fov <= 90.0f)
			fov += 0.2f;
	}

	bool isFixed() {
		return fixed;
	}
	
	void FixView(vec3 carPos, vec3 carFront) {
		if (fixed) {
			float sumX = 0, sumZ = 0;
			for (auto p : lastFrontX)
				sumX += p;
			for (auto p : lastFrontZ)
				sumZ += p;
			glm::mat4 trans = mat4(1.0f);
			trans = rotate(trans, radians(FixedYaw), vec3(0.0f, 1.0f, 0.0f));
			glm::vec4 tempFront = vec4((carFront.x + sumX / lastFrontX.size()) / 2.0f, \
				carFront.y, (carFront.z + sumZ / lastFrontZ.size()) / 2.0f, 1.0f);

			tempFront = trans * tempFront;
			Front = vec3(tempFront.x, tempFront.y, tempFront.z);
			Position.x = carPos.x - 5.2f * tempFront.x;
			Position.z = carPos.z - 5.2f * tempFront.z;
			//Position.y = 2.2f;
			Position.y = carPos.y+2.2f;
			lastFrontX.push_back(carFront.x);
			lastFrontZ.push_back(carFront.z);
			while (lastFrontX.size() > 25) {
				lastFrontX.pop_front();
				lastFrontZ.pop_front();
			}
		}
	}

	void shakewindow() {
		static float lastShakeTime = 0.0f;
		if (shake && (currentTime - lastShakeTime) > 0.5f) {
			static float sign = -1.0f;
			if (shakeflag) {
				sign = -sign;
				Position.x -= 4.5f * deltaTime;
				Position.z -= 4.5f * deltaTime;
				Position.y += sign * 1.0f * deltaTime;
			}
			else {
				Position.x += 4.5f * deltaTime;
				Position.z += 4.5f * deltaTime;
				Position.y += sign * 1.0f * deltaTime;
			}
			//shakeflag = !shakeflag;
			lastShakeTime = currentTime;
		}
	}

	void setDeltaTime()
	{
		currentTime = glfwGetTime();
		deltaTime = currentTime - lastTime;
		lastTime = currentTime;
	}

	void setFixed(glm::vec3& carfront) {
		fixed = !fixed;
		Yaw = 0;
		FixedYaw = 0;
		Pitch = 0;
		lastFrontX.push_back(carfront.x);
		lastFrontZ.push_back(carfront.z);
	}

private:
	// Calculates the front vector from the Camera's (updated) Euler Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		/*front.x = -sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
		front.y = sin(glm::radians(Pitch));
		front.z = -cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));*/
		Front = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		Right = glm::normalize(glm::cross(Front, WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		Up = glm::normalize(glm::cross(Right, Front));
	}
};
#endif

