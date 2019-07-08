#include "GLFW_EngineCore.h"
#include "Game.h"
#include <fstream>
#include <sstream>
#include <glm/detail/type_vec3.hpp>
#include <glm/glm.hpp>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <chrono>
#include <ft2build.h>
#include FT_FREETYPE_H 

std::vector<bool> GLFW_EngineCore::m_keyBuffer;
glm::vec2 GLFW_EngineCore::m_mousePos;
glm::vec2 GLFW_EngineCore::m_mouseTravel;
int GLFW_EngineCore::m_screenWidth;
int GLFW_EngineCore::m_screenHeight;

using namespace std::chrono_literals;


GLFW_EngineCore::~GLFW_EngineCore()
{
	// cleanup
	glfwTerminate();
}

bool GLFW_EngineCore::initWindow(int width, int height, std::string windowName)
{
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);

	m_screenWidth = width;
	m_screenHeight = height;

	m_window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
	if (m_window == nullptr)
	{
		std::cout << "Failed to create GLFW m_window" << std::endl;
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(m_window);
	
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialise GLAD" << std::endl;
		return false;
	}

	// callback functions
	glfwSetFramebufferSizeCallback(m_window, windowResizeCallbackEvent);
	glfwSetKeyCallback(m_window, keyCallbackEvent);
	glfwSetCursorPosCallback(m_window, mouseMoveCallbackEvent);

	// make space for the keybuffer
	m_keyBuffer.resize(m_keyBufferSize);
	std::fill(m_keyBuffer.begin(), m_keyBuffer.end(), false);

	// define mouse vectors
	m_mousePos = glm::vec2(0.0f, 0.0f);
	m_mouseTravel = glm::vec2(0.0f, 0.0f);

	// set the shaders to the given default ones
	//setDefaultShaders();
	setupDefaultFont();

	// set the drawable model as a cube
	// note: this will be changed later when we can draw many kinds of objects
	initCubeModel();

	// enable depth test
	glEnable(GL_DEPTH_TEST);

	// enable alpha transparency
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	return true;
}

bool GLFW_EngineCore::runEngine(Game* game)
{
	// for this example just give the game direct access to the engine
	// there are other ways this could be handled
	game->m_engineInterfacePtr = this;

	// use a fixed timestep of 1 / (60 fps) approx = 16 milliseconds
	constexpr std::chrono::milliseconds dt(16ms);

	// using an alias (similar to typedef)
	using clock = std::chrono::high_resolution_clock;

	auto simulationTime = clock::now();

	// game loop
	while (!glfwWindowShouldClose(m_window))
	{
		game->m_inputHandler->handleInputs(m_keyBuffer);
		game->m_inputHandler->handlePressedInputs(m_keyBuffer);
		if (glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT)) // Make sure the user clicks and holds the mouse before moving
		{
			game->m_inputHandler->handleMouseTravelInputs(m_mouseTravel);
		}
		game->update(1.0f); // update game logic, the float value is temporary!!
		game->render(); // prepare game to send info to the renderer in engine core

		// swap buffers
		glfwSwapBuffers(m_window);
		glfwPollEvents();
	}

	return true;
}

void GLFW_EngineCore::renderColouredBackground(float r, float g, float b)
{
	glClearColor(r, g, b, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

//-----------------------------Private functions------------------------------
void GLFW_EngineCore::mouseMoveCallbackEvent(GLFWwindow * window, double xPos, double yPos)
{
	glm::vec2 currentPos(xPos, yPos);

	// Get travel difference from old and current mouse position
	m_mouseTravel = currentPos - m_mousePos;

	// Asign current mouse position
	m_mousePos = currentPos;
}

void GLFW_EngineCore::keyCallbackEvent(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_UNKNOWN || key > m_keyBufferSize)
	{
		return;
	}
	m_keyBuffer[key] = ((action == GLFW_PRESS || action == GLFW_REPEAT));

	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, true);
	}
}

void GLFW_EngineCore::windowResizeCallbackEvent(GLFWwindow* window, int width, int height)
{
	// change the opengl viewport to match the new m_window size
	m_screenWidth = width;
	m_screenHeight = height;
	glViewport(0, 0, width, height);
}

/*
void GLFW_EngineCore::loadShader(string vertexShaderFile, string fragmentShaderFile, GLuint& shaderProgram)
{
	
}

// loading some default shaders to get things up and running
void GLFW_EngineCore::setDefaultShaders()
{
	loadShader("assets/shaders/defaultShader.vert", "assets/shaders/defaultShader.frag", m_defaultShaderProgram);
	loadShader("assets/shaders/fontShader.vert", "assets/shaders/fontShader.frag", m_fontShaderProgram);

	// set the default shader
	glUseProgram(m_defaultShaderProgram);
}
*/

// based on tutorial from https://learnopengl.com/#!In-Practice/Text-Rendering
void GLFW_EngineCore::setupDefaultFont()
{
	// FreeType
	FT_Library ft;
	// All functions return a value different than 0 whenever an error occurred
	if (FT_Init_FreeType(&ft))
		std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;

	// Load font as face
	FT_Face face;
	if (FT_New_Face(ft, "assets/fonts/Square.ttf", 0, &face))
	{
		std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
		return;
	}
	// Set size to load glyphs as
	FT_Set_Pixel_Sizes(face, 0, 48);

	// Disable byte-alignment restriction
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	// Load first 128 characters of ASCII set
	for (GLubyte c = 0; c < 128; c++)
	{
		// Load character glyph 
		if (FT_Load_Char(face, c, FT_LOAD_RENDER))
		{
			std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
			continue;
		}
		// Generate texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			face->glyph->bitmap.width,
			face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);
		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		// Now store character for later use
		Character character = {
			texture,
			glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
			face->glyph->advance.x
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);


	// Configure VAO/VBO for texture quads
	glGenVertexArrays(1, &font_VAO);
	glGenBuffers(1, &font_VBO);
	glBindVertexArray(font_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, font_VBO);
	// dynamic draw as the text may change frequently
	glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

// a simple function to initialise a cube model in memory
void GLFW_EngineCore::initCubeModel()
{
	// set up vertex and normal data
	float vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f,  1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f,  1.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};
	
	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// normal attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	
}

// screen is between 0 and 1 with (0,0) at the bottom left corner
// typical use: m_engineInterfacePtr->renderText("Awesome Game Debug Mode", 0.05f, 0.05f, 1.0f, glm::vec3(0.9, 0.9f, 0.1f));
/*void GLFW_EngineCore::renderText(std::string text, float x, float y, float scale, glm::vec3 colour)
{
	// set the window to orthographic
	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(m_screenWidth), 0.0f, static_cast<float>(m_screenHeight));

	float pixelValueX = x * m_screenWidth;
	float pixelValueY = y * m_screenHeight;

	glUseProgram(m_fontShaderProgram);
	glUniformMatrix4fv(glGetUniformLocation(m_fontShaderProgram, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	// Activate corresponding render state	
	glUniform3f(glGetUniformLocation(m_fontShaderProgram, "textColour"), colour.x, colour.y, colour.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(font_VAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = pixelValueX + ch.bearing.x * scale;
		GLfloat ypos = pixelValueY - (ch.size.y - ch.bearing.y) * scale;

		GLfloat w = ch.size.x * scale;
		GLfloat h = ch.size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos,     ypos,       0.0, 1.0 },
			{ xpos + w, ypos,       1.0, 1.0 },

			{ xpos,     ypos + h,   0.0, 0.0 },
			{ xpos + w, ypos,       1.0, 1.0 },
			{ xpos + w, ypos + h,   1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, font_VBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1/64 pixels)
		pixelValueX += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);

	// go back to default shader
	glUseProgram(m_defaultShaderProgram);
}*/


void GLFW_EngineCore::setCamera(CameraComponent* cam)
{
	// set the window dimensions in case the window resizes
	cam->setWindowDimensions(m_screenWidth, m_screenHeight);
}

void GLFW_EngineCore::drawCube(const glm::mat4& modelMatrix, GLuint shader)
{
	// set the model component of our shader to the cube model
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	// the only thing we can draw so far is the cube, so we know it is bound already
	// this will obviously have to change later
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void GLFW_EngineCore::drawModel(Model* model, glm::mat4& modelMatrix, GLuint shader)
{
	// set the model component of our shader to the object model
	glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(modelMatrix));

	model->render(shader);
}