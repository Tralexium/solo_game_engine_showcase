#pragma once
//#include <glad/glad.h>
#include <iostream>
#include <string>
#include <glm/mat4x4.hpp>
//#include <vector>
#include <fstream>
#include <sstream>
#include "CameraComponent.h"

class ShaderComponent : public Component
{
public:
	// Public Vars
	GLuint shaderProgram;
	std::string vertexShaderFile;
	std::string fragmentShaderFile;

	ShaderComponent(std::string shaderName)
	{
		vertexShaderFile = "assets/shaders/" + shaderName + ".vert";
		fragmentShaderFile = "assets/shaders/" + shaderName + ".frag";

		// Load contents of vertex file
		std::ifstream inFile(vertexShaderFile);
		if (!inFile) {
			std::string errorMsg = "Error opening shader file: " + vertexShaderFile + "\n";
			fprintf(stderr, errorMsg.c_str());
			exit(1);
		}

		std::stringstream code;
		code << inFile.rdbuf();
		inFile.close();
		std::string codeStr(code.str());
		const GLchar* vertex_shader[] = { codeStr.c_str() };

		// Load contents of fragment file
		std::ifstream inFile2(fragmentShaderFile);
		if (!inFile2) {
			std::string errorMsg = "Error opening shader file: " + fragmentShaderFile + "\n";
			fprintf(stderr, errorMsg.c_str());
			exit(1);
		}

		std::stringstream code2;
		code2 << inFile2.rdbuf();
		inFile2.close();
		std::string codeStr2(code2.str());
		const GLchar* fragment_shader[] = { codeStr2.c_str() };

		// vertex shader
		int vertexShader = glCreateShader(GL_VERTEX_SHADER);
		glShaderSource(vertexShader, 1, vertex_shader, NULL);
		glCompileShader(vertexShader);
		// check for shader compile errors
		int success;
		char infoLog[512];
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// fragment shader
		int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		glShaderSource(fragmentShader, 1, fragment_shader, NULL);
		glCompileShader(fragmentShader);
		// check for shader compile errors
		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		}
		// link shaders
		shaderProgram = glCreateProgram();
		glAttachShader(shaderProgram, vertexShader);
		glAttachShader(shaderProgram, fragmentShader);
		glLinkProgram(shaderProgram);
		// check for linking errors
		glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
		if (!success) {
			glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
			std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		}
		glDeleteShader(vertexShader);
		glDeleteShader(fragmentShader);
	}

	~ShaderComponent()
	{
	}

	inline void useShader()
	{
		glUseProgram(shaderProgram);
	}


	////////////
	// SETERS //
	////////////
	void setProjection(glm::mat4 proj)
	{
		u_projection = proj;
	}

	void setView(CameraComponent* view)
	{
		u_view = view;
	}

	void setColor(glm::vec3 color)
	{
		u_color = color;
	}

	void setLightColor(glm::vec3 l_color)
	{
		u_lightColor = l_color;
	}

	void setLightPos(glm::vec3 p_color)
	{
		u_lightPos = p_color;
	}

	void setUniforms()
	{
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(u_projection));
		glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"), 1, GL_FALSE, glm::value_ptr(u_view->getViewMatrix()));
		// be sure to activate shader when setting uniforms/drawing objects
		glUniform3f(glGetUniformLocation(shaderProgram, "objectColour"), u_color.x, u_color.y, u_color.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "lightColour"), u_lightColor.x, u_lightColor.y, u_lightColor.z);
		glUniform3f(glGetUniformLocation(shaderProgram, "lightPos"), u_lightPos.x, u_lightPos.y, u_lightPos.z);
		glUniform3fv(glGetUniformLocation(shaderProgram, "viewPos"), 1, glm::value_ptr(u_view->position()));
	}

	GLuint getShaderID()
	{
		return shaderProgram;
	}

	void OnUpdate(float dt) override{}
	void OnMessage(const std::string m) override{}
private:
	CameraComponent* u_view;
	glm::mat4 u_projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
	glm::vec3 u_color = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 u_lightColor = glm::vec3(0.45f, 0.45f, 0.4f);
	glm::vec3 u_lightPos = glm::vec3(0.0f, 10.0f, 10.0f);
};