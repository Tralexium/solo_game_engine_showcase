#pragma once
#include "GameObject.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <vector>
#include "ShaderComponent.h"

class VBOQuad : public GameObject
{
public:
	VBOQuad();
	~VBOQuad();
	void Render();
	unsigned int GetVaoHandle();
	void OnUpdate(float dt) override;
	void OnMessage(const std::string msg) override;

private:
	unsigned int m_VaoHandle;
	unsigned int m_VboHandle;
};