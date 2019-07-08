#include "ModelManager.h"

ModelManager::ModelManager(){}

Model* ModelManager::getModelFromMap(string modelPath) 
{
	if (m_modelMap.find(modelPath) == m_modelMap.end()) {
		// Model not found, load it and store it in the map
		Model *loadedModel = new Model(modelPath);
		m_modelMap.insert(std::pair<string, Model*>(modelPath, loadedModel));

		return m_modelMap[modelPath];
	}
	else {
		return m_modelMap[modelPath];
	}
}