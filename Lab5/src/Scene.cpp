#include "Scene.h"
#include "TransformComponent.h"
#include "PlayerObject.h"
#include "StaticEnvironmentObject.h"
#include "SizedEnvironmentObject.h"
#include "WigglyEnvironmentObject.h"
#include "RotatingEnvironmentObject.h"
#include "ColourComponent.h"
#include "ModelComponent.h"
#include "ShaderComponent.h"
#include <fstream>
#include <sstream>

using namespace std;

Scene::Scene()
{
}

Scene::~Scene()
{
	// need to reclaim memory for transform components

	for (GameObject* gameObject : v_sceneObjects)
	{
		// Delete all assosiated Transform components
		if (gameObject->getComponent<TransformComponent>())
		{
			TransformComponent* tc = gameObject->getComponent<TransformComponent>();

			delete tc;	//reclaim ram!

		}

		// Delete all assosiated Model components
		if (gameObject->getComponent<ModelComponent>())
		{
			ModelComponent* mc = gameObject->getComponent<ModelComponent>();

			delete mc;

		}

		// Delete assosiated Camera component
		if (gameObject->getComponent<CameraComponent>())
		{
			CameraComponent* mc = gameObject->getComponent<CameraComponent>();

			delete mc;

		}
		//v_sceneObjects is illegal now!!! 

	}

	v_sceneObjects.clear();
}

GameObject* Scene::getObjectAt(int index)
{
	return v_sceneObjects[index];
}

std::vector<GameObject*> Scene::getInputObjects()
{
	return v_inputObjects;
}

// LEGACY LEVEL LOADING
/*void Scene::loadLevel(std::string levelFile)
{
	vector<int> g_inVector;
	int numCubesToRead;
	stringstream ss;
	string s;

	ifstream myInputFile;
	myInputFile.open(levelFile, ios_base::in);

	if (myInputFile.is_open())
	{
		// read the first line which has the number of elements
		getline(myInputFile, s);
		ss.str(s);
		ss.ignore(17); // ignore the first 20 characters
		ss >> numCubesToRead;

		// resize the correct size for the vector
		v_sceneObjects.resize(numCubesToRead); // Resize the ammount of cubes
		ss.clear();

		for (int i = 0; i < numCubesToRead; i++)
		{
			// use a stringstream to get integer values
			getline(myInputFile, s);

			// POSITION
			getline(myInputFile, s);
			ss.clear();
			ss.str(s);

			float xPos = 0.0f;
			float yPos = 0.0f;
			float zPos = 0.0f;

			ss >> xPos;
			ss >> yPos;
			ss >> zPos;

			glm::vec3 cubePosition(xPos, yPos, zPos);

			// ORIENTATION
			getline(myInputFile, s);
			ss.clear();
			ss.str(s);

			float wOri = 1.0f;
			float xOri = 0.0f;
			float yOri = 0.0f;
			float zOri = 0.0f;

			ss >> wOri;
			ss >> xOri;
			ss >> yOri;
			ss >> zOri;

			glm::quat cubeOrientation(wOri, xOri, yOri, zOri);

			// SCALE
			getline(myInputFile, s);
			ss.clear();
			ss.str(s);

			float xSca = 1.0f;
			float ySca = 1.0f;
			float zSca = 1.0f;

			ss >> xSca;
			ss >> ySca;
			ss >> zSca;

			glm::vec3 cubeScale(xSca, ySca, zSca);

			Model *modelFromFile = m_modelManager.getModelFromMap("assets/models/sphere.obj"); // Load the model from the current Json node

			v_sceneObjects[i]->addComponent(new TransformComponent(cubePosition, cubeOrientation, cubeScale)); // save it to the vector
			v_sceneObjects[i]->addComponent(new ModelComponent(modelFromFile)); // Add the appropriate model id
		}

		myInputFile.close();
	}
}*/


bool Scene::loadJsonLevel(std::string levelJsonFile) 
{
	std::fstream jsonData;
	Json::Value root;
	Json::Reader reader;

	jsonData.open(levelJsonFile.c_str());
	// check for errors
	if (!reader.parse(jsonData, root))
	{
		std::cout << "Failed to parse the data from: "
				  << levelJsonFile
				  << reader.getFormattedErrorMessages();
		return false;
	}

	const Json::Value gameObjects = root["GameObjects"];

	// Resize v_sceneObjects
	//v_sceneObjects.resize(gameObjects.size());

	// size() tells us ho large the array is
	for (int i = 0; i < gameObjects.size(); i++)
	{
		// POSITION
		float xPos, yPos, zPos;
		glm::vec3 objPosition;
		const Json::Value posNode = gameObjects[i]["position"];
		if (posNode.type() != Json::nullValue) { // Check if this data exists
			xPos = posNode[0].asFloat(); // Get floats and asign to each axis
			yPos = posNode[1].asFloat();
			zPos = posNode[2].asFloat();

			objPosition = glm::vec3(xPos, yPos, zPos);
		}else objPosition = glm::vec3(0.0f, 0.0f, 0.0f);


		// ORIENTATION
		float wOri, xOri, yOri, zOri;
		glm::quat objOrientation;
		const Json::Value orientNode = gameObjects[i]["orientation"];
		if (orientNode.type() != Json::nullValue) { // Check if this data exists
			wOri = orientNode[0].asFloat(); // Get floats and asign to each axis
			xOri = orientNode[1].asFloat();
			yOri = orientNode[2].asFloat();
			zOri = orientNode[3].asFloat();

			objOrientation = glm::quat(wOri, xOri, yOri, zOri);
		}else objOrientation = glm::quat(1.0f, .0f, .0f, .0f);

	
		// SCALE
		float xSca, ySca, zSca;
		glm::vec3 objScale;
		const Json::Value scaleNode = gameObjects[i]["scale"];
		if (scaleNode.type() != Json::nullValue) { // Check if this data exists
			xSca = scaleNode[0].asFloat(); // Get floats and asign to each axis
			ySca = scaleNode[1].asFloat();
			zSca = scaleNode[2].asFloat();

			objScale = glm::vec3(xSca, ySca, zSca);
		}else objScale = glm::vec3(1.0f, 1.0f, 1.0f);


		// MODEL NAME
		string modelId;
		const Json::Value modelNode = gameObjects[i]["name"];
		if (modelNode.type() != Json::nullValue) { // Check if this data exists
			modelId = modelNode.asString() + ".obj"; // Get the right model for this object
		}else modelId = "assets/models/sphere.obj";

		// SHADER NAME
		string shaderId;
		const Json::Value shaderNode = gameObjects[i]["shader"];
		shaderId = shaderNode.asString(); // Get the right model for this object

		// TYPE
		string type = gameObjects[i]["type"].asString();
		if (type == "PlayerObject")
		{
			Model *modelFromFile = m_modelManager.getModelFromMap("assets/models/" + modelId); // Load the model from the current Json node
			v_sceneObjects.push_back(new PlayerObject(modelFromFile, objPosition, objOrientation, objScale));
			v_inputObjects.push_back(v_sceneObjects[i]);
		}
		else if (type == "StaticEnvironmentObject")
		{
			Model *modelFromFile = m_modelManager.getModelFromMap("assets/models/" + modelId); // Load the model from the current Json node
			v_sceneObjects.push_back(new StaticEnvironmentObject(modelFromFile, objPosition, objOrientation, objScale));
		}
		else if (type == "SizedEnvironmentObject")
		{
			Model *modelFromFile = m_modelManager.getModelFromMap("assets/models/" + modelId); // Load the model from the current Json node
			v_sceneObjects.push_back(new SizedEnvironmentObject(modelFromFile, objPosition, objOrientation));
		}
		else if (type == "WigglyEnvironmentObject")
		{
			Model *modelFromFile = m_modelManager.getModelFromMap("assets/models/" + modelId); // Load the model from the current Json node
			v_sceneObjects.push_back(new WigglyEnvironmentObject(modelFromFile, objPosition, objOrientation));
		}
		else if (type == "RotatingEnvironmentObject")
		{
			Model *modelFromFile = m_modelManager.getModelFromMap("assets/models/" + modelId); // Load the model from the current Json node
			v_sceneObjects.push_back(new RotatingEnvironmentObject(modelFromFile, objPosition, objOrientation, -0.01f));
		}
		else
		{
			std::cout << type << ": unknown type\n";
			continue; // not an object we can create
		}

		// Apply proper shader to the current object
		v_sceneObjects[i]->addComponent(new ShaderComponent(shaderId));

		// Display which objects have been loaded successfully
		std::cout << gameObjects[i]["name"].asString() << "-" << type << " loaded\n";
	}

	jsonData.close();
	return true;
}

void Scene::updateScene(float dt)
{
	// Update each individual object
	for (GameObject* gameObject : v_sceneObjects)
	{
		gameObject->OnUpdate(dt);
	}
}


void Scene::renderScene(IEngineCore* engine, CameraComponent* cam)
{
	glm::mat4 projection = cam->getProjectionMatrix();

	glClearColor(0.1f, 0.1f, 0.1f, 1.0f); // make sure we clear the framebuffer's content
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST); // enable depth testing (is disabled for rendering screen-space quad)

	// render the background
	float redValue = 0, greenValue = 0, blueValue = 0;
	if (m_playerBackground->getComponent<RedComponent>())
		redValue = m_playerBackground->getComponent<RedComponent>()->m_colourValue;
	if (m_playerBackground->getComponent<GreenComponent>())
		greenValue = m_playerBackground->getComponent<GreenComponent>()->m_colourValue;
	if (m_playerBackground->getComponent<BlueComponent>())
		blueValue = m_playerBackground->getComponent<BlueComponent>()->m_colourValue;

	// e.g. pass object details to the engine to render the next frame
	engine->renderColouredBackground(redValue, greenValue, blueValue);

	// render the objects
	for (GameObject* gameObject : v_sceneObjects)
	{
		// change here to get both model* and model matrix and use new game engine render.
		gameObject->getComponent<ShaderComponent>()->useShader();
		gameObject->getComponent<ShaderComponent>()->setProjection(projection);
		gameObject->getComponent<ShaderComponent>()->setView(cam);
		gameObject->getComponent<ShaderComponent>()->setUniforms();

		Model *ModelId = gameObject->getComponent<ModelComponent>()->getModel();
		glm::mat4 TMatrix = gameObject->getComponent<TransformComponent>()->getModelMatrix();
		GLuint shaderProgram = gameObject->getComponent<ShaderComponent>()->getShaderID();
		engine->drawModel(ModelId, TMatrix, shaderProgram);
	}
}