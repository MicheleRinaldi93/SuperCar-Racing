#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <irrKlang/irrKlang.h>
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
#include "stb_image.h"
#include "skyBox.h"
#include "mesh.h"
#include "texture.h"
#include "model.h"
#include "car.h"
#include "fixed_camera.h"
#include "carselect.h"
#include "terrain.h"
#include "effects.h"
#include "billboard.h"
#include "sphere.h"

#pragma comment(lib, "irrKlang.lib")

using namespace glm;
using namespace std;

// settings
const unsigned int SCR_WIDTH=1920;
const unsigned int SCR_HEIGHT=1080;

const unsigned int SHADOW_WIDTH = 1024 * 10;
const unsigned int SHADOW_HEIGHT = 1024 * 10;

vec3 WORLD_UP(0.0f, 1.0f, 0.0f);

// timing
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int countdownValue = 4;
double startTime;
double countdownDuration = 120.0; // 2 minuti
int minute = 0;
int second = 0;
int countTimeValue = 0;
double lastTime = 0.0;
vec3 posVecchia;

//Camera
vec3 pos(0.0, 1.0, 0.0);		// Posizione camera
vec3 at(0.0, 0.0, -1.0);	// Punto in cui "guarda" la camera
vec3 up(0.0, 1.0, 0.0);		// Vettore up...la camera è sempre parallela al piano

vec3 dir(0.0, 0.0, -0.1);	// Direzione dello sguardo
vec3 side(1.0, 0.0, 0.0);	// Direzione spostamento laterale
vec3 upM = vec3(0.0, 1.0, 0.0);	// Direzione spostamento UpPos = pos + 0.1f * up;

Camera camera(pos);
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;

//Texture
unsigned int coinTexture;
unsigned int pressStartTexture;
unsigned int textureNum;
unsigned int number0;
unsigned int number1;
unsigned int number2;
unsigned int number3;
unsigned int number4;
unsigned int number5;
unsigned int number6;
unsigned int number7;
unsigned int number8;
unsigned int number9;
unsigned int separator;
unsigned int textureGo;
unsigned int menutexture;
unsigned int menuSelectiontexture1;
unsigned int menuSelectiontexture2;
unsigned int menuSelectiontexture3;
unsigned int modeSelectiontexture1;
unsigned int modeSelectiontexture2;
unsigned int helpMenutexture1;
unsigned int helpMenutexture2;
unsigned int wintexture;
unsigned int gameovertexture;
unsigned int bgtexture;
unsigned int bgtexture2;
unsigned int textureFlag;

//CubeSkyBox
skyBox* csb;
unsigned int cubemapTexture1;
unsigned int cubemapTexture2;
unsigned int cubemapTexture3;
unsigned int cubemapTexture4;
unsigned int skyboxVAO, skyboxVBO;
int facesindex = 0;
const vector<string> faces_1{
	"asset/image/skybox/skybox2/right.jpg",
	"asset/image/skybox/skybox2/left.jpg",
	"asset/image/skybox/skybox2/top.jpg",
	"asset/image/skybox/skybox2/bottom.jpg",
	"asset/image/skybox/skybox2/front.jpg",
	"asset/image/skybox/skybox2/back.jpg"
};
const vector<string> faces_2{
	"asset/image/skybox/skybox3/right.jpg",
	"asset/image/skybox/skybox3/left.jpg",
	"asset/image/skybox/skybox3/top.jpg",
	"asset/image/skybox/skybox3/bottom.jpg",
	"asset/image/skybox/skybox3/front.jpg",
	"asset/image/skybox/skybox3/back.jpg"
};
const vector<string> faces_3{
	"asset/image/skybox/skybox4/right.jpg",
	"asset/image/skybox/skybox4/left.jpg",
	"asset/image/skybox/skybox4/top.jpg",
	"asset/image/skybox/skybox4/bottom.jpg",
	"asset/image/skybox/skybox4/front.jpg",
	"asset/image/skybox/skybox4/back.jpg"
};
const vector<string> faces_4{
	"asset/image/skybox/skybox5/right.jpg",
	"asset/image/skybox/skybox5/left.jpg",
	"asset/image/skybox/skybox5/top.jpg",
	"asset/image/skybox/skybox5/bottom.jpg",
	"asset/image/skybox/skybox5/front.jpg",
	"asset/image/skybox/skybox5/back.jpg"
};

//depth map
unsigned int depthMap;
unsigned int depthMapFBO;

//Shader
Shader* skyBoxShader;
Shader* depthShader;
Shader* myShader;
Shader* pointLightShader;
Shader* carShader;
Shader* blendingShader;
Shader* menuShader;
Shader* lightShader;

//proprietà illuminazione
vec3 lightPos(-1.0f, 5.0f, -1.0f);
vec3 lightCol(1.0f, 1.0f, 1.0f);
vec3 lightDirection = normalize(lightPos);
mat4 lightSpaceMatrix;

//Buffer
unsigned int bgVBO, bgVAO, bgEBO;
unsigned int bgVBO2, bgVAO2, bgEBO2;
unsigned int carVBO, carVAO;
int bgshaderProgram;
int bgshaderProgram2;
int carshaderProgram;
float bgvertices[] = {	// creazione di un quadrato
	-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	1.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
};
unsigned int bgindices[] = {	//indici texture quadrato
	0,1,2,
	0,2,3,
};

// Status game setting
Car car(vec3(0, 0, 0));
int carindex = 0;
int coinindex = 0;
int modeindex = 0;
int helpIndex = 0;
int coinTot = 0;
int laps = 0;
int menuindex = 0;
int gamestatus = 0;
int oldgamestatus = -1;
bool isPolygonMode = false;
bool isCountdown = true;
bool isLaps = false;
float totalTime=1;
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void key_callback_menu(GLFWwindow* window, int key, int scancode, int action, int mods);
void key_callback_menuSelection(GLFWwindow* window, int key, int scancode, int action, int mods);
void key_callback_modeSelection(GLFWwindow* window, int key, int scancode, int action, int mods);
void key_callback_carselect(GLFWwindow* window, int key, int scancode, int action, int mods);
void key_callback_menuhelp(GLFWwindow* window, int key, int scancode, int action, int mods);
void handleKeyInput(GLFWwindow* window);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);

// Model
Terrain* terreno{};
Terrain* guardrailRosso{};
Terrain* guardrailBianco{};
Model* raceOval;
Model* ford;
Model* jeep;
Model* delorean;
Model* superCar;
Model* carModel;
Model* coin;
Model* green;
Model* race;
Model* fasce;
Model* costruzioni;
Model* battistrada;
Model* battistrada1;
Model* alberi;
Model* semaforo;
Model* sfere3R;
Model* sfere2R;
Model* sfere1R;
Model* sfere3V;

//BillBoard
Billboard bboard;

//Particellari
SmokeHendler* particelle;

//Musica
bool soundCountdown = true;
bool soundGame = true;
irrklang::ISoundEngine* SoundEngine_menu = irrklang::createIrrKlangDevice();
irrklang::ISoundEngine* SoundEngine_coin = irrklang::createIrrKlangDevice();
irrklang::ISoundEngine* SoundEngine_countdown = irrklang::createIrrKlangDevice();

//creazione di un cubo per la macchina
float carvertices[] = {
	-0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	0.1f, 0.1f, -0.1f,
	0.1f, 0.1f, -0.1f,
	-0.1f, 0.1f, -0.1f,
	-0.1f, -0.1f, -0.1f,

	-0.1f, -0.1f, 0.1f,
	0.1f, -0.1f, 0.1f,
	0.1f, 0.1f, 0.1f,
	0.1f, 0.1f, 0.1f,
	-0.1f, 0.1f, 0.1f,
	-0.1f, -0.1f, 0.1f,

	-0.1f, 0.1f, 0.1f,
	-0.1f, 0.1f, -0.1f,
	-0.1f, -0.1f, -0.1f,
	-0.1f, -0.1f, -0.1f,
	-0.1f, -0.1f, 0.1f,
	-0.1f, 0.1f, 0.1f,

	0.1f, 0.1f, 0.1f,
	0.1f, 0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, 0.1f,
	0.1f, 0.1f, 0.1f,

	-0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, -0.1f,
	0.1f, -0.1f, 0.1f,
	0.1f, -0.1f, 0.1f,
	-0.1f, -0.1f, 0.1f,
	-0.1f, -0.1f, -0.1f,

	-0.1f, 0.1f, -0.1f,
	0.1f, 0.1f, -0.1f,
	0.1f, 0.1f, 0.1f,
	0.1f, 0.1f, 0.1f,
	-0.1f, 0.1f, 0.1f,
	-0.1f, 0.1f, -0.1f,
};

vec3 coinpos[13]{
	vec3(1.4743,1,35.899),
	vec3(-18.6387,1,60.1675),
	vec3(-55.94,1,69.2311),
	vec3(-48.35,1,107.464),
	vec3(-19.23,1,114.39),
	vec3(-64.5467,1,135.23),
	vec3(-72.97,1,76.87),
	vec3(-72.40,1,29.22),
	vec3(-123.69, 1 ,17.38),
	vec3(89.98,1,7.112),
	vec3(-20.8,1,2.7774),
	vec3(-20.70,1,-30.68),
	vec3(-14.65,1,-63.23)
};
// ------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	// make sure the viewport matches the new window dimensions; note that width and 
	// height will be significantly larger than specified on retina displays.
	glViewport(0, 0, width, height);
}

unsigned int loadTexture(string filename)
{
	unsigned int texture;

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	// set the texture wrapping parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	// set texture filtering parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// load image, create texture and generate mipmaps
	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
																					// The FileSystem::getPath(...) is part of the GitHub repository so we can find files on any IDE/platform; replace it with your own image path.
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0);
	if (data)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return -1;
	}
	stbi_image_free(data);

	return texture;
}

unsigned int loadTextureAlpha(char const* path)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT); // for this tutorial: use GL_CLAMP_TO_EDGE to prevent semi-transparent borders. Due to interpolation it takes texels from next repeat 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, format == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        std::cout << "Texture loaded " << path << std::endl;
        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

void initShader(Shader* shaderToUse) {
	// create transformations
	mat4 projection = mat4(1.0f);	//identity matrix
	projection = perspective(radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 1000.0f);
	shaderToUse->use();
	shaderToUse->setMat4("projection", projection);
	mat4 view = camera.GetViewMatrix();
	shaderToUse->setMat4("view", view);
}

void pointLight(Shader* lightingShader) {
	myShader->use();
	lightingShader->setVec3("light.position", vec3(1.0,3.0,1.0));
	lightingShader->setVec3("viewPos", pos);

	lightingShader->setVec3("light.ambient", 4000, 4000, 4000);
	lightingShader->setVec3("light.diffuse", 4000,4000,4000);
	lightingShader->setVec3("light.specular", 4000,4000,4000);
	lightingShader->setFloat("light.constant", 1.0f);
	lightingShader->setFloat("light.linear", 0.09f);
	lightingShader->setFloat("light.quadratic", 0.032f);

	// material properties
	lightingShader->setFloat("material.shininess", 32.0f);
}

void pointLightRed(Shader* lightingShader) {
	lightingShader->use();
	lightingShader->setVec3("light.position", vec3(0,1,0));
	lightingShader->setVec3("viewPos", camera.Position);

	lightingShader->setVec3("lights.ambient", 1,0,0);
	lightingShader->setVec3("lights.diffuse", 0.8,0.8,0.8);
	lightingShader->setVec3("lights.specular", 1.0f, 1.0f, 1.0f);
	lightingShader->setFloat("lights.constant", 1.0f);
	lightingShader->setFloat("lights.linear", 0.09f);
	lightingShader->setFloat("lights.quadratic", 0.032f);

	// material properties
	lightingShader->setFloat("material.shininess", 32.0f);
}

void pointLightGreen(Shader* lightingShader) {
	lightingShader->use();
	lightingShader->setVec3("light.position", vec3(0, 1, 0));
	lightingShader->setVec3("viewPos", camera.Position);

	lightingShader->setVec3("lights.ambient", 0, 1, 0);
	lightingShader->setVec3("lights.diffuse", 0.8, 0.8, 0.8);
	lightingShader->setVec3("lights.specular", 1.0f, 1.0f, 1.0f);
	lightingShader->setFloat("lights.constant", 1.0f);
	lightingShader->setFloat("lights.linear", 0.09f);
	lightingShader->setFloat("lights.quadratic", 0.032f);

	// material properties
	lightingShader->setFloat("material.shininess", 32.0f);
}

//--------------------------SKYBOX--------------------------

void cubeSkyboxInit()
{
	csb = new skyBox();
	skyboxVAO = csb->generateCubeBuffer(skyboxVAO, skyboxVBO);

	cubemapTexture1 = csb->loadCubemap(faces_1);

	cubemapTexture2 = csb->loadCubemap(faces_2);

	cubemapTexture3 = csb->loadCubemap(faces_3);

	cubemapTexture4 = csb->loadCubemap(faces_4);
}

void renderCubeSkyBox(Shader* shader)
{
	glDepthFunc(GL_LEQUAL);
	initShader(skyBoxShader);

	mat4 viewMatrix = mat4(mat3(camera.GetViewMatrix()));	// remove translation from the view matrix
	mat4 projMatrix = camera.GetProjMatrix((float)SCR_WIDTH / (float)SCR_WIDTH);

	shader->setMat4("view", viewMatrix);
	shader->setMat4("projection", projMatrix);

	switch (facesindex) {
		case 0: {
			csb->drawCube(shader, skyboxVAO, cubemapTexture1);
			break;
		}
		case 1: {
			csb->drawCube(shader, skyboxVAO, cubemapTexture2);
			break;
		}
		case 2: {
			csb->drawCube(shader, skyboxVAO, cubemapTexture3);
			break;
		}
		case 3: {
			csb->drawCube(shader, skyboxVAO, cubemapTexture4);
			break;
		}
	}
	glDepthFunc(GL_LESS);
}

//------------RENDER MODEL-------------------
void renderSemafori(Shader* shader) {
	switch (countdownValue) {
		case 0: {
			sfere3V->Draw(shader);
			break;
		}
		case 1: {
			sfere3R->Draw(shader);
			break;
		}
		case 2: {
			sfere2R->Draw(shader);
			break;
		}
		case 3: {
			sfere1R->Draw(shader);
			break;
		}
	}
}

void renderRaceTrack(Model* raceOval, Shader* shader) {

	//////////////PISTA//////////////////
	mat4 viewMatrix = camera.GetViewMatrix();
	shader->setMat4("view", viewMatrix);

	mat4 model = mat4(1);
	model = translate(model, vec3(0, -0.4, 0));
	model = scale(model, vec3(1.5f, 1.5f, 1.5f));
	model = rotate(model, 2.15f, vec3(0.0f, 1.0f, 0.0f));
	shader->setMat4("model", model);
	terreno->Draw(shader);
	guardrailRosso->Draw(shader);
	guardrailBianco->Draw(shader);
	race->Draw(shader);
	fasce->Draw(shader);
	costruzioni->Draw(shader);
	alberi->Draw(shader);
	semaforo->Draw(shader);
	renderSemafori(shader);
}

void renderCar(Model* model, mat4 modelMatrix, Shader* shader) {

	if (carindex == 0) {
		//Jeep
		modelMatrix = translate(modelMatrix, vec3(0.0f, 0.5f, 0.0f));
		modelMatrix = rotate(modelMatrix, 3.15f, vec3(0.0f, 1.0f, 0.0f));
		modelMatrix = scale(modelMatrix, vec3(3, 3, 3));
	}
	else if (carindex == 1) {
		//Ford
		modelMatrix = rotate(modelMatrix, -1.57f, vec3(0.0f, 1.0f, 0.0f));
	}

	shader->setMat4("model", modelMatrix);

	model->Draw(shader);
}

void renderCarModel(Model* carModel, Shader* shader)
{
	mat4 viewMatrix = camera.GetViewMatrix();
	mat4 projMatrix = camera.GetProjMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);
	shader->setMat4("view", viewMatrix);
	shader->setMat4("projection", projMatrix);

	mat4 modelMatrix = mat4(1.0f);
	modelMatrix = translate(modelMatrix, car.getMidValPosition());
	modelMatrix = rotate(modelMatrix, radians(car.getYaw()), WORLD_UP);

	renderCar(carModel, modelMatrix, shader);
}

void renderCoin(Model* coin, Shader* shader, vec3 pos) {

	mat4 viewMatrix = camera.GetViewMatrix();
	shader->setMat4("view", viewMatrix);

	mat4 projMatrix = camera.GetProjMatrix((float)SCR_WIDTH / (float)SCR_HEIGHT);
	shader->setMat4("projection", projMatrix);

	mat4 modelMatrix = mat4(1.0f);
	modelMatrix = translate(modelMatrix, pos);
	modelMatrix = rotate(modelMatrix, 1 * (float)glfwGetTime(), WORLD_UP);
	modelMatrix = scale(modelMatrix, vec3(0.8, 0.8, 0.8));

	shader->setMat4("model", modelMatrix);

	coin->Draw(shader);
}

//---------------RENDER BILLBOARD---------------
unsigned int findNumber(int number) {

	switch (number) {
		case 0: {
			textureNum = number0;
			break;
		}
		case 1: {
			textureNum = number1;
			break;
		}
		case 2: {
			textureNum = number2;
			break;
		}
		case 3: {
			textureNum = number3;
			break;
		}
		case 4: {
			textureNum = number4;
			break;
		}
		case 5: {
			textureNum = number5;
			break;
		}
		case 6: {
			textureNum = number6;
			break;
		}
		case 7: {
			textureNum = number7;
			break;
		}
		case 8: {
			textureNum = number8;
			break;
		}
		case 9: {
			textureNum = number9;
			break;
		}
	}
	return textureNum;
}

void countdownTimeBillBoard(Shader* shader, Shader* shaderSphere)
{
	double currentTime = glfwGetTime();
	double deltaTime = currentTime - lastTime;

	if (deltaTime >= 1.0) { // Aggiorna il countdown ogni secondo
		countdownValue--;
		lastTime = currentTime;
	}

	if (countdownValue == 0) {
		bboard.drawFigure(shader, textureGo, vec3(0.02f, 0.02f, 0.1f), 0, 0);
	}
	else if (countdownValue > 0) {
		bboard.drawFigure(shader, findNumber(countdownValue), vec3(0.02f, 0.02f, 0.1f), 0, 0);
	}
	else if (countdownValue < 0) {
		isCountdown = false;
	}
}

void showTimeBillBoard(Shader* shader, bool countdown) {
	double currentTime = glfwGetTime();
	if (startTime == 0) {
		startTime = currentTime;
	}

	double remainingTime;
	if(countdown){
		remainingTime = countdownDuration - (currentTime - startTime);
		if (remainingTime < 0) {
			gamestatus = 6;
		}
	}
	else {
		remainingTime = currentTime - startTime;
	}

	// Calcola minuti e secondi rimanenti
	int minutes = static_cast<int>(remainingTime) / 60;
	int seconds = static_cast<int>(remainingTime) % 60;

	if (minutes == 9 && seconds == 59) {
		gamestatus = 6;
	}
	
	//Minutes
	bboard.drawFigure(shader, findNumber(minutes), vec3(0.01f, 0.01f, 0.1f), -0.008, 0.0335);
	
	//Separatore
	bboard.drawFigure(shader, separator, vec3(0.008f, 0.008f, 0.1f), 0.0, 0.0335);
	
	//Seconds
	string numberString = to_string(seconds);
	if (numberString.length() == 1) {
		bboard.drawFigure(shader, number0, vec3(0.01f, 0.01f, 0.1f), 0.008, 0.0335);
		bboard.drawFigure(shader, findNumber(numberString[0] - '0'), vec3(0.01f, 0.01f, 0.1f), 0.017, 0.0335);
	}
	else {
		bboard.drawFigure(shader, findNumber(numberString[0] - '0'), vec3(0.01f, 0.01f, 0.1f), 0.008, 0.0335);
		bboard.drawFigure(shader, findNumber(numberString[1]-'0'), vec3(0.01f, 0.01f, 0.1f), 0.017, 0.0335);
	}
}

void showCoinBB(Shader* shader, int coin) {
	string numberCoin = to_string(coin);
	if (numberCoin.length() == 1) {
		bboard.drawFigure(shader, number0, vec3(0.01f, 0.01f, 0.1f), -0.065f, 0.0335f);
		bboard.drawFigure(shader, findNumber(numberCoin[0] - '0'), vec3(0.01f, 0.01f, 0.1f), -0.055f, 0.0335f);
	}
	else {
		bboard.drawFigure(shader, findNumber(numberCoin[0] - '0'), vec3(0.01f, 0.01f, 0.1f), -0.065f, 0.0335f);
		bboard.drawFigure(shader, findNumber(numberCoin[1] - '0'), vec3(0.01f, 0.01f, 0.1f), -0.055f, 0.0335f);
	}

}
	
//------------------COLLISION MODEL--------------------
bool collisionDetectCoin(vec3 modelRace) {
	if ((modelRace.x - car.getMidValPosition().x) * (modelRace.x - car.getMidValPosition().x) + (modelRace.z - car.getMidValPosition().z) * (modelRace.z - car.getMidValPosition().z) + (modelRace.y - car.getMidValPosition().y) * (modelRace.y - car.getMidValPosition().y) < 5.0f) {
		SoundEngine_coin->play2D("asset/music/yesclick.wav", GL_FALSE);
		return true;
	}
	return false;
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		camera.ProcessKeyboard(FORWARD, deltaTime);	
	}
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
		camera.ProcessKeyboard(BACKWARD, deltaTime);
		
	}
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		camera.ProcessKeyboard(LEFT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		camera.ProcessKeyboard(RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		camera.ProcessKeyboard(UP, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		camera.ProcessKeyboard(DOWN, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
		camera.ProcessKeyboard(ROTATE_RIGHT, deltaTime);
	}
	if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
		camera.ProcessKeyboard(ROTATE_LEFT, deltaTime);
	}
}

void setDeltaTime() {
	float currentFrame = static_cast<float>(glfwGetTime());
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;
}

void createBuffer() {
	//-----------SelectFistCar---------------
	glGenVertexArrays(1, &bgVAO);
	glGenBuffers(1, &bgVBO);
	glGenBuffers(1, &bgEBO);

	glBindVertexArray(bgVAO);

	glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bgvertices), bgvertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bgEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bgindices), bgindices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);


	bgshaderProgram = glCreateProgram();
	bgtexture = BgInit_1(bgshaderProgram, "asset/image/carSelection/boxSelectionJeep.jpg");

	//-------------SelectSecondCar-------------------
	glGenVertexArrays(1, &bgVAO2);
	glGenBuffers(1, &bgVBO2);
	glGenBuffers(1, &bgEBO2);

	glBindVertexArray(bgVAO2);

	glBindBuffer(GL_ARRAY_BUFFER, bgVBO2);
	glBufferData(GL_ARRAY_BUFFER, sizeof(bgvertices), bgvertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bgEBO2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(bgindices), bgindices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	bgshaderProgram2 = glCreateProgram();
	bgtexture2 = BgInit_2(bgshaderProgram2, "asset/image/carSelection/boxSelectionFord.jpg");

	//---------- BillBoard -----------------
	bboard.generateBuffers();
}

void textureLoad() {
	coinTexture = loadTextureAlpha("asset/image/coin/coin2.png");
	number0 = loadTextureAlpha("asset/image/countdown/00.png");
	number1 = loadTextureAlpha("asset/image/countdown/1.png");
	number2 = loadTextureAlpha("asset/image/countdown/2.png");
	number3 = loadTextureAlpha("asset/image/countdown/3.png");
	number4 = loadTextureAlpha("asset/image/countdown/4.png");
	number5 = loadTextureAlpha("asset/image/countdown/5.png");
	number6 = loadTextureAlpha("asset/image/countdown/6.png");
	number7 = loadTextureAlpha("asset/image/countdown/7.png");
	number8 = loadTextureAlpha("asset/image/countdown/8.png");
	number9 = loadTextureAlpha("asset/image/countdown/9.png");
	separator = loadTextureAlpha("asset/image/countdown/separator.png");
	textureGo = loadTextureAlpha("asset/image/countdown/Go.png");
	pressStartTexture = loadTextureAlpha("asset/image/menuStart/press_start.png");
	textureFlag = loadTextureAlpha("asset/image/countdown/start_flag.png");

	/////// MENU TEXTURE /////////
	menutexture = loadTexture("asset/image/menuStart/menuOpen2.jpg");
	menuSelectiontexture1 = loadTexture("asset/image/menuStart/menuSelection1.jpg");
	menuSelectiontexture2 = loadTexture("asset/image/menuStart/menuSelection2.jpg");
	menuSelectiontexture3 = loadTexture("asset/image/menuStart/menuSelection3.jpg");
	modeSelectiontexture1 = loadTexture("asset/image/menuStart/modeSelection1.jpg");
	modeSelectiontexture2 = loadTexture("asset/image/menuStart/modeSelection2.jpg");
	gameovertexture = loadTexture("asset/image/menuStart/gameover.jpg");
	helpMenutexture1 = loadTexture("asset/image/menuStart/helpMenu1.jpg");
	helpMenutexture2 = loadTexture("asset/image/menuStart/helpMenu2.jpg");
	wintexture = loadTexture("asset/image/menuStart/win.jpg");
}

void modelLoad() {
	////////////// MONDO //////////////////
	race = new Model("asset/object/circuito/race.obj", false);
	fasce = new Model("asset/object/circuito/fasceStrada.obj");
	costruzioni = new Model("asset/object/circuito/costruzioni.obj");
	battistrada = new Model("asset/object/circuito/battistrada/battistradaUnito.obj");
	alberi = new Model("asset/object/alberi/alberiDiversi.obj");

	///////////// SEMAFORO //////////////////
	semaforo = new Model("asset/object/semaforo/semaforo3.obj");
	sfere3R = new Model("asset/object/semaforo/sfere3R.obj");
	sfere2R = new Model("asset/object/semaforo/sfere2R.obj");
	sfere1R = new Model("asset/object/semaforo/sfere1R.obj");
	sfere3V = new Model("asset/object/semaforo/sfere3V.obj");

	////////// MODEL ///////////
	coin = new Model("asset/object/coin/coin.obj");
	jeep = new Model("asset/object/jeep/car.obj");
	delorean = new Model("asset/object/delorean/delorean.obj");
	superCar = new Model("asset/object/superCar/car1.obj");
	ford = new Model("asset/object/ford/car.obj");
	particelle = new SmokeHendler("asset/object/cloud/n/cloud.obj", 2, 0.1, 0.3);

	///////TERRAIN//////
	terreno = new Terrain("asset/object/circuito/green.obj", "asset/object/circuito/verde.jpg");
	guardrailRosso = new Terrain("asset/object/circuito/battistrada/battistradaUnito.obj", "asset/object/circuito/battistrada/Rosso.png");
	guardrailBianco = new Terrain("asset/object/circuito/battistrada/battistradaBianco.obj", "asset/object/circuito/battistrada/bianco.jpg");
}

void shaderLoad() {
	myShader = new Shader("shader/model_loading.vs", "shader/model_loading.fs");
	pointLightShader = new Shader("shader/point_light.vert", "shader/point_light.frag");
	menuShader = new Shader("shader/vertex_shader.vert", "shader/fragment_shader.frag");
	depthShader = new Shader("shader/shadow_mapping_depth.vs", "shader/shadow_mapping_depth.fs");
	carShader = new Shader("shader/car_select.vs", "shader/car_select.fs");
	blendingShader = new Shader("shader/blending.vs", "shader/blending.fs");
	skyBoxShader = new Shader("shader/skybox.vs","shader/skybox.fs");
	
	skyBoxShader->use();
	skyBoxShader->setInt("skybox", 0);

	myShader->use();
	myShader->setInt("diffuseTexture", 0);
	myShader->setInt("shadowMap", 15);

	pointLightShader->use();
	pointLightShader->setInt("material.diffuse", 0);
	pointLightShader->setInt("material.specular", 1);
}

void initLoad()  {
	//---------SkyBox--------------
	cubeSkyboxInit();
	//---------Shader--------------
	shaderLoad();
	// --------Model---------------
	modelLoad();
	//------createBuffer----------
	createBuffer();
	//--------Texture------------
	textureLoad();
}

int main()
{
	// glfw: initialize and configure
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

	// glfw window creation e imposta la finestra in full screen
	GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Super Car Racing", NULL, NULL);
	//GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Super Car Racing", glfwGetPrimaryMonitor(), NULL);
	if (window == NULL) {
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	glfwSetCursorPosCallback(window, mouse_callback);
	glfwSetScrollCallback(window, scroll_callback);

	// tell GLFW to capture our mouse
	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// glad: load all OpenGL function pointers
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// configure global opengl state
	// -----------------------------
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	//depthMapFBOInit();

	initLoad();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
	// ---------------------------------
	// RENDER CICLO
	// ---------------------------------
	
	while (!glfwWindowShouldClose(window))
	{
		posVecchia = car.position;

		//------- Menu Start---------------
		if (gamestatus == 0) {
			if(oldgamestatus == -1 || oldgamestatus == 4 ) {
				SoundEngine_menu->stopAllSounds();
				SoundEngine_menu->play2D("asset/music/mainmenu.wav", GL_TRUE);
			}
			oldgamestatus = gamestatus;

			if (!camera.isFixed())
				camera.setFixed(car.front);
			
			glfwSetKeyCallback(window, key_callback_menu);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			initShader(blendingShader);
			bboard.drawFigure(blendingShader, pressStartTexture, vec3(0.05f, 0.01f, 0.1f), 0.035, -0.036f);
			initShader(menuShader);
			bboard.drawFigure(menuShader, menutexture, vec3(0.164,0.095,0.095), 0.0, 0.0);

			glfwPollEvents();
			glfwSwapBuffers(window);
		}
		//------- Menu --------------
		else if (gamestatus == 1) {
			oldgamestatus = gamestatus;

			glfwSetKeyCallback(window, key_callback_menuSelection);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			switch (menuindex)
			{
			case 0: {
				initShader(menuShader);
				bboard.drawFigure(menuShader, menuSelectiontexture1, vec3(0.164, 0.095, 0.095), 0.0, 0.0);
				break;
			}
			case 1: {
				initShader(menuShader);
				bboard.drawFigure(menuShader, menuSelectiontexture2, vec3(0.164, 0.095, 0.095), 0.0, 0.0);
				break;
			}
			case 2: {
				initShader(menuShader);
				bboard.drawFigure(menuShader, menuSelectiontexture3, vec3(0.164, 0.095, 0.095), 0.0, 0.0);
				break;
			}
			default:
				break;
			}

			glfwPollEvents();
			glfwSwapBuffers(window);
		}
		//------- Car Selection ------------
		else if (gamestatus == 2) {
			oldgamestatus = gamestatus;
			glfwSetKeyCallback(window, key_callback_carselect);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			switch (carindex)
			{
				case 0: {
					car.setAngularVelocity(50);
					car.setMaxSpeed(15);
					car.setAcceleration(4);
					Drawcarselect1(bgtexture, bgshaderProgram, bgVAO, carShader, jeep, window);
					break;
				}
				case 1: {
					car.setAngularVelocity(40);
					car.setMaxSpeed(35);
					car.setAcceleration(8);
					Drawcarselect2(bgtexture2, bgshaderProgram2, bgVAO, carShader, ford, window);
					break;
				}
			}

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		//------- Menu Help -------------
		else if (gamestatus == 3) {
			oldgamestatus = gamestatus;

			glfwSetKeyCallback(window, key_callback_menuhelp);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			switch (helpIndex) {
				case 0: {
					initShader(menuShader);
					bboard.drawFigure(menuShader, helpMenutexture1, vec3(0.164, 0.095, 0.095), 0.0, 0.0);
					break;
				}
				case 1: {
					initShader(menuShader);
					bboard.drawFigure(menuShader, helpMenutexture2, vec3(0.164, 0.095, 0.095), 0.0, 0.0);
					break;
				}
			}

			glfwPollEvents();
			glfwSwapBuffers(window);
		}
		//------- Game Play----------------
		else if (gamestatus == 4) {

			if (oldgamestatus != 4) {
				SoundEngine_menu->stopAllSounds();
			}

			oldgamestatus = gamestatus;
			setDeltaTime();
			camera.setDeltaTime();

			glfwSetKeyCallback(window, NULL);

			handleKeyInput(window);

			camera.CameraMove(window);
			camera.FixView(car.getMidValPosition(), car.front);

			//////// TERRAIN COLLISION /////////
			car.position = terreno->updateCameraPositionOnMap(car.position, posVecchia, 0.3, true);
			if (guardrailRosso->updateCameraPositionOnMapCollision(car.position, posVecchia, 0.1, true) || guardrailBianco->updateCameraPositionOnMapCollision(car.position, posVecchia, 0.1, true)) {
				car.position = posVecchia;
			}

			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			//////// SKYBOX ////////// 	
			renderCubeSkyBox(skyBoxShader);

			//----Mode A TEMPO-----
			if (modeindex == 1) {

				///////BILLBOARD COIN////////////
				initShader(carShader);
				bboard.drawObjectCoin(vec3(0.0f, 0.0f, -0.1f), vec3(-7.5f, 5.5f, -4.0f), vec3(0.4f, 0.4f, 0.4f), carShader, coin);
				initShader(blendingShader);
				showCoinBB(blendingShader, coinTot);

				if (isCountdown) {
					if (soundCountdown) {
						SoundEngine_menu->play2D("asset/music/mariostart.mp3", GL_FALSE);
						soundCountdown = false;
					}
					countdownTimeBillBoard(blendingShader, myShader);
				}
				else if (!isCountdown) {
					if (soundGame) {
						SoundEngine_menu->stopAllSounds();
						SoundEngine_menu->play2D("asset/music/ingame.wav", GL_TRUE);
						soundGame = false;
					}
					showTimeBillBoard(blendingShader, true);
				}

				for (int i = 0; i < 13; i++) {
					if (collisionDetectCoin(coinpos[i])) {
						coinpos[i].y = -10;
						coinTot++;
					}
					initShader(myShader);
					renderCoin(coin, myShader, coinpos[i]);
				}

				if (coinTot == 12) {
					gamestatus = 5;
				}
			}
			//-----MODE LIBERO--------
			else if (modeindex == 0){
				////////BILLBOARD LAPS////////////
				initShader(blendingShader);
				bboard.drawFigure(blendingShader, textureFlag, vec3(0.01f, 0.01f, 0.1f), -0.075f, 0.0335f);
				if (laps > 1) {
					initShader(blendingShader);
					bboard.drawFigure(blendingShader, findNumber(laps-1), vec3(0.01f, 0.01f, 0.1f), -0.065f, 0.0335f);
				}
				else {
					initShader(blendingShader);
					bboard.drawFigure(blendingShader, findNumber(0), vec3(0.01f, 0.01f, 0.1f), -0.065f, 0.0335f);
				}

				if (isCountdown) {
					if (soundCountdown) {
						SoundEngine_menu->play2D("asset/music/mariostart.mp3", GL_FALSE);
						soundCountdown = false;
					}
					countdownTimeBillBoard(blendingShader, myShader);
				}
				else if (!isCountdown) {
					if (soundGame) {
						SoundEngine_menu->stopAllSounds();
						SoundEngine_menu->play2D("asset/music/ingame.wav", GL_TRUE);
						soundGame = false;
					}
					showTimeBillBoard(blendingShader, false);
				}

				if ((car.position.z > -4.0517 && car.position.z < 5.7645)) {
					if (isLaps) {
						laps++;
					}
					isLaps = false;
				}
				else {
					isLaps = true;
				}

				////// GIRI MENO 2/////////
				if (laps == 5) {
					gamestatus = 5;
				}
			}			
			//--------------------------

			mat4 lightProjection = ortho(
				-200.0f, 200.0f,
				-200.0f, 200.0f,
				-200.0f, 200.0f);

			mat4 lightView = lookAt(lightPos, vec3(0.0f), WORLD_UP);
			lightSpaceMatrix = lightProjection * lightView;

			initShader(depthShader);
			depthShader->setMat4("lightSpaceMatrix", lightSpaceMatrix);

			switch (carindex)
				{
				case 0: {
					carModel = jeep;
					break;
				}
				case 1: {
					carModel = ford;
					break;
				}
			}

			initShader(pointLightShader);
			pointLight(pointLightShader);

			car.UpdateDelayYaw();
			car.UpdateDelayPosition();

			initShader(myShader);
			renderCarModel(carModel, myShader);
			renderRaceTrack(raceOval, myShader);

			/////////// PARTICELLARI MOTORE ///////////////////
			/*initShader(myShader);
			particelle->setShaders(myShader);
			particelle->addSmokeGenerator(lastFrame, car.position+vec3(0,0,-1), car.position + vec3(0, 1, 0));
			particelle->Draw(lastFrame, vec3(0.2f, 0.4f, 0.2f));*/
			

			glfwSwapBuffers(window);
			glfwPollEvents();
		}
		// --------Win Menu----------
		else if (gamestatus == 5) {

			if (oldgamestatus != 5) {
				SoundEngine_menu->stopAllSounds();
				SoundEngine_menu->play2D("asset/music/firstplace.wav", GL_FALSE);
			}
			oldgamestatus = gamestatus;

			glfwSetKeyCallback(window, key_callback_menu);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			initShader(menuShader);
			bboard.drawFigure(menuShader, wintexture, vec3(0.164, 0.095, 0.095), 0.0, 0.0);

			glfwPollEvents();
			glfwSwapBuffers(window);

		}
		// --------Game Over--------
		else if (gamestatus == 6) {

			if (oldgamestatus != 6) {
				SoundEngine_menu->stopAllSounds();
				SoundEngine_menu->play2D("asset/music/gameover.mp3", GL_FALSE);
			}
			oldgamestatus = gamestatus;

			glfwSetKeyCallback(window, key_callback_menu);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			initShader(menuShader);
			bboard.drawFigure(menuShader, gameovertexture, vec3(0.164, 0.095, 0.095), 0.0, 0.0);

			glfwPollEvents();
			glfwSwapBuffers(window);
		}
		//-------Selection Mode-----------
		else if (gamestatus == 7) {
			oldgamestatus = gamestatus;

			glfwSetKeyCallback(window, key_callback_modeSelection);

			glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
			glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			switch (modeindex)
			{
				case 0: {
					initShader(menuShader);
					bboard.drawFigure(menuShader, modeSelectiontexture1, vec3(0.164, 0.095, 0.095), 0.0, 0.0);
					break;
				}
				case 1: {
					initShader(menuShader);
					bboard.drawFigure(menuShader, modeSelectiontexture2, vec3(0.164, 0.095, 0.095), 0.0, 0.0);
					break;
				}
			}

			glfwPollEvents();
			glfwSwapBuffers(window);
		}
	}

	// optional: de-allocate all resources once they've outlived their purpose:

	glDeleteVertexArrays(1, &skyboxVAO);
	glDeleteVertexArrays(1, &skyboxVBO);

	glfwTerminate();
	return 0;
}

// ------------- KEY INPUT-----------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
	float xpos = static_cast<float>(xposIn);
	float ypos = static_cast<float>(yposIn);


	if (firstMouse)
	{
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}

	float xoffset = (xpos - lastX)*0.3;
	float yoffset = (lastY - ypos)*0; // cosi blocco l'asse y e il mouse si muove solo sull'asse x

	lastX = xpos;
	lastY = ypos;

	camera.ProcessMouseMovement(xoffset, yoffset);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.ProcessMouseScroll(yoffset);
}

void key_callback_menu(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS)
		return;
	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_SPACE:
		SoundEngine_coin->play2D("asset/music/yesclick.wav", GL_FALSE);
		gamestatus = 1;
		break;
	default:
		break;
	}
}

void key_callback_menuhelp(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS)
		return;

	switch (key)
	{
		case GLFW_KEY_ESCAPE:
			glfwSetWindowShouldClose(window, GL_TRUE);
			break;
		case GLFW_KEY_B:
			SoundEngine_coin->play2D("asset/music/yesclick.wav", GL_FALSE);
			gamestatus = 1;
			break;
		case GLFW_KEY_LEFT:
			SoundEngine_coin->play2D("asset/music/yesclick.wav", GL_FALSE);
			helpIndex = 0;
			break;
		case GLFW_KEY_RIGHT:
			SoundEngine_coin->play2D("asset/music/yesclick.wav", GL_FALSE);
			helpIndex = 1;
			break;
	}
}

void key_callback_menuSelection(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS)
		return;

	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		gamestatus = 1;
		break;
	case GLFW_KEY_DOWN:
		menuindex = (menuindex + 1) % 3;
		SoundEngine_coin->play2D("asset/music/menuselect.wav", GL_FALSE);
		break;
	case GLFW_KEY_UP:
		if (menuindex == 0)menuindex = 3;
		SoundEngine_coin->play2D("asset/music/menuselect.wav", GL_FALSE);
		menuindex = (menuindex - 1) % 3;
		break;
	case GLFW_KEY_ENTER:
		SoundEngine_coin->play2D("asset/music/yesclick.wav", GL_FALSE);
		if (menuindex == 0) {
			gamestatus = 7;
		}
		else if (menuindex == 1) {
			gamestatus = 3;
		}
		else if (menuindex == 2) {
			glfwSetWindowShouldClose(window, GL_TRUE);
		}
		break;
	}
}

void key_callback_modeSelection(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (action != GLFW_PRESS)
		return;

	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		gamestatus = 1;
		break;
	case GLFW_KEY_DOWN:
		//modeindex = (modeindex + 1) % 2;
		SoundEngine_coin->play2D("asset/music/menuselect.wav", GL_FALSE);
		modeindex = 1;
		break;
	case GLFW_KEY_UP:
		//if (modeindex == 0)modeindex = 2;
		SoundEngine_coin->play2D("asset/music/menuselect.wav", GL_FALSE);
		//modeindex = (modeindex - 1) % 2;
		modeindex = 0;
		break;
	case GLFW_KEY_ENTER:
		SoundEngine_coin->play2D("asset/music/yesclick.wav", GL_FALSE);
		gamestatus = 2;
		break;
	case GLFW_KEY_B:
		gamestatus = 1;
		break;
	default:
		break;
	}
}

void key_callback_carselect(GLFWwindow* window, int key, int scancode, int action, int mods) {
	if (action != GLFW_PRESS)
		return;

	switch (key)
	{
	case GLFW_KEY_ESCAPE:
		glfwSetWindowShouldClose(window, GL_TRUE);
		break;
	case GLFW_KEY_LEFT:
		carindex = (carindex + 1) % 2;
		SoundEngine_coin->play2D("asset/music/menuselect.wav", GL_FALSE);
		break;
	case GLFW_KEY_RIGHT:
		if (carindex == 0)carindex = 2;
		SoundEngine_coin->play2D("asset/music/menuselect.wav", GL_FALSE);
		carindex = (carindex - 1) % 2;
		break;
	case GLFW_KEY_ENTER:
		SoundEngine_coin->play2D("asset/music/yesclick.wav", GL_FALSE);
		gamestatus = 4;
		break;
	case GLFW_KEY_B:
		SoundEngine_coin->play2D("asset/music/yesclick.wav", GL_FALSE);
		gamestatus = 1;
		break;
	case GLFW_KEY_H:
		SoundEngine_coin->play2D("asset/music/yesclick.wav", GL_FALSE);
		gamestatus = 3;
		break;
	default:
		break;
	}
}

void handleKeyInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS) {
		if (!isCountdown) {
			car.ProcessKeyboard(CAR_FORWARD, deltaTime);

			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
				car.ProcessKeyboard(CAR_LEFT, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
				car.ProcessKeyboard(CAR_RIGHT, deltaTime);

			if (camera.isFixed()) {
				if (abs(dot(normalize(camera.Front), normalize(car.front)) > 0.5))
					camera.BackEffect();
				else
					camera.ForwardEffect();
			}
		}
	}
	else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS) {
		if (!isCountdown) {
			car.ProcessKeyboard(CAR_BACKWARD, deltaTime);

			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
				car.ProcessKeyboard(CAR_RIGHT, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
				car.ProcessKeyboard(CAR_LEFT, deltaTime);

			if (camera.isFixed()) {
				if (abs(dot(normalize(camera.Front), normalize(car.front)) > 0.5))
					camera.ForwardEffect();
				else
					camera.BackEffect();
			}
		}
	}
	else {
		car.ProcessKeyboard_static(deltaTime);
		if (car.speed != 0) {
			if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
				car.ProcessKeyboard(CAR_LEFT, deltaTime);
			if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
				car.ProcessKeyboard(CAR_RIGHT, deltaTime);
		}
	}
	glfwSetKeyCallback(window, key_callback);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_C && action == GLFW_PRESS) {
		camera.setFixed(car.front);
		string info = camera.isFixed() ? "Prospettiva Fissa" : "Prospettiva Libera";
		std::cout << "[CAMERA]" << info << std::endl;
	}
	if (key == GLFW_KEY_X && action == GLFW_PRESS) {
		isPolygonMode = !isPolygonMode;
		if (isPolygonMode) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		}
		else {
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		}
		string info = isPolygonMode ? "Passa alla modalità di rendering wireframe" : "Passa alla modalità di rendering normale";
		std::cout << "[POLYGON_MODE]" << info << std::endl;
	}
	if (key == GLFW_KEY_M && action == GLFW_PRESS) {
		facesindex = (facesindex + 1);
		facesindex = facesindex % 4;
	}
	if (key == GLFW_KEY_N && action == GLFW_PRESS) {
		if (facesindex == 0)facesindex = 4;
		facesindex = (facesindex - 1) % 4;
	}
	if (key == GLFW_KEY_B) {
		gamestatus = 1;
	}
	if (key == GLFW_KEY_Z) {
		gamestatus = 2;
	}
}

//------------------------------------------------------------------