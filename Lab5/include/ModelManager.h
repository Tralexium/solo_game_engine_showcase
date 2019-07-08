#pragma once
#include "Model.h"
#include <string>
#include <list>
#include <map>

class ModelManager 
{
public:
	ModelManager();
	Model* getModelFromMap(string path);

private:
	std::map<string, Model*> m_modelMap;
};
