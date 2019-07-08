#pragma once
#include <map>
#include "GameObject.h"
#include "PlayerObject.h"
#include <vector>
#include "TransformComponent.h"
#include "SceneComponent.h"
#include "CameraComponent.h"


class InputCommand
{
public:
	virtual ~InputCommand() {}
	virtual void execute(GameObject& object) = 0;
};

class InputCommandPressed
{
public:
	virtual ~InputCommandPressed() {}
	virtual void execute(GameObject& object) = 0;
};

class InputMouseTravel
{
public:
	virtual ~InputMouseTravel() {}
	virtual void execute(GameObject& object, glm::vec2 travelDist) = 0;
};


//e.g. class RotateLeftCommand : public InputCommand


//// OBJECTS //////////////////////////////////////////////////////////////////////

// ROTATION
class RotateLeft : public InputCommand
{
public:
	void execute(GameObject& object) override {
		object.OnMessage("rotateCameraLeft");
	}
};

class RotateRight : public InputCommand
{
public:
	void execute(GameObject& object) override {
		object.OnMessage("rotateCameraRight");
	}
};

class RotateUp : public InputCommand
{
public:
	void execute(GameObject& object) override {
		object.OnMessage("rotU");
	}
};

class RotateDown : public InputCommand
{
public:
	void execute(GameObject& object) override {
		object.OnMessage("rotD");
	}
};

// TRANSLATION
class TranslateLeft : public InputCommand
{
public:
	void execute(GameObject& object) override {
		object.OnMessage("movePlayerLeft");
	}
};

class TranslateRight : public InputCommand
{
public:
	void execute(GameObject& object) override {
		object.OnMessage("movePlayerRight");
	}
};

class TranslateForward : public InputCommand
{
public:
	void execute(GameObject& object) override {
		object.OnMessage("movePlayerForward");
	}
};

class TranslateBackward : public InputCommand
{
public:
	void execute(GameObject& object) override {
		object.OnMessage("movePlayerBackward");
	}
};

// SCENE SWITCHING
class NextScene : public InputCommandPressed
{
public:
	void execute(GameObject& object) override {
		object.OnMessage("NextScene");
	}
};

class PreviousScene : public InputCommandPressed
{
public:
	void execute(GameObject& object) override {
		object.OnMessage("PreviousScene");
	}
};

// CAMERA ROTATION
class RotateCamera : public InputMouseTravel
{
public:
	void execute(GameObject& object, glm::vec2 travelDist) override {
		if (object.getComponent<CameraComponent>())
		{
			PlayerObject* playerObj = dynamic_cast<PlayerObject*>(&object);
			playerObj->MouseTravel(travelDist);
		}
	}
};

// CAMERA MODE
class SwitchCamMode : public InputCommandPressed
{
public:
	void execute(GameObject& object) override {
		object.OnMessage("switchCamera");
	}
};



//// Input handling /////////////////////////////////////////////////////////
struct InputHandler
{	
	std::vector<GameObject*> v_objects;

	std::vector<int> v_keyPressedBuffer;
	glm::vec2 m_previousMouseTravel = glm::vec2(0.0f, 0.0f);

	std::map<int, InputCommand*> m_controlMapping;
	std::map<int, InputCommandPressed*> m_controlMappingPressed;
	std::vector<InputMouseTravel*> m_controlMouseTravel;

	// Input handler for objects
	InputHandler(std::vector<GameObject*> object) : v_objects(object)
	{
		// Player object movement controls
		m_controlMapping[(int)'A'] = new TranslateLeft;
		m_controlMapping[(int)'D'] = new TranslateRight;
		m_controlMapping[(int)'W'] = new TranslateForward;
		m_controlMapping[(int)'S'] = new TranslateBackward;

		// Player/Camera rotation
		m_controlMapping[(int)'Q'] = new RotateLeft;
		m_controlMapping[(int)'E'] = new RotateRight;

		// Scene controls
		m_controlMappingPressed[(int)'.'] = new NextScene;
		m_controlMappingPressed[(int)','] = new PreviousScene;
		m_controlMappingPressed[(int)'R'] = new SwitchCamMode;

		// Mouse related controls
		m_controlMouseTravel.push_back(new RotateCamera);

		// Add the existing inputs into the key buffers
		fillBuffers();
	}

	void fillBuffers() 
	{
		v_keyPressedBuffer.resize(400);
		std::fill(v_keyPressedBuffer.begin(), v_keyPressedBuffer.end(), false);
	}

	void addObject(GameObject& object)
	{
		v_objects.push_back(&object);
	}

	void clearObjects()
	{
		v_objects.clear();
	}

	void checkForNulls()
	{
		v_objects.erase(std::remove(begin(v_objects), end(v_objects), nullptr), 
						end(v_objects));
	}

	void handleInputs(const std::vector<bool>& keyBuffer)
	{
		checkForNulls();

		for(const auto& mapEntry : m_controlMapping)
		{
			if(keyBuffer[mapEntry.first] && !v_objects.empty())
			{
				int tempSize = v_objects.size();
				for (int i = 0; i < tempSize; i++)
					mapEntry.second->execute(*v_objects.at(i));
			}
		}
	}

	void handlePressedInputs(const std::vector<bool>& keyBuffer)
	{
		checkForNulls();

		for (const auto& mapEntry : m_controlMappingPressed)
		{
			if (keyBuffer[mapEntry.first] && !v_objects.empty())
			{
				if (!v_keyPressedBuffer[mapEntry.first])
				{
					int tempSize = v_objects.size();
					for (int i = 0; i < tempSize; i++)
					{
						mapEntry.second->execute(*v_objects.at(i));
					}
					v_keyPressedBuffer[mapEntry.first] = true;
				}
			}
			else v_keyPressedBuffer[mapEntry.first] = false;
		}
	}

	void handleMouseTravelInputs(glm::vec2 travelDist) 
	{
		if (travelDist.x != m_previousMouseTravel.x || travelDist.y != m_previousMouseTravel.y)
		{
			for (const auto& mapEntry : m_controlMouseTravel)
			{
				int tempSize = v_objects.size();
				for (int i = 0; i < tempSize; i++)
				{
					mapEntry->execute(*v_objects.at(i), travelDist);
				}
			}

			m_previousMouseTravel = travelDist;
		}
	}
};
