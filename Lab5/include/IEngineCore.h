#pragma once
#include <string>
#include <iostream>
#include <glm/mat4x4.hpp>
#include <Model.h>

class CameraComponent;
class Game;

class IEngineCore
{
public:

	virtual ~IEngineCore() {}

	virtual bool initWindow(int width, int height, std::string windowName) = 0;
	virtual bool runEngine(Game* game) = 0;

	//virtual void renderText(std::string text, float x, float y, float scale, glm::vec3 colour) = 0;
	virtual void renderColouredBackground(float r, float g, float b) = 0;
	virtual	void setCamera(CameraComponent* cam) = 0;
	virtual void drawCube(const glm::mat4& modelMatrix, GLuint shader) = 0;

	// A drawing method that takes a model and the tranformation matrix
	virtual void drawModel(Model* model, glm::mat4& modelMatrix, GLuint shader) = 0;

};
