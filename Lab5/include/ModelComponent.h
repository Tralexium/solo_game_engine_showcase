#pragma once
#include "Component.h"
#include "Model.h"

class ModelComponent : public Component
{
public:
	Model *m_model;

	void OnUpdate(float dt) override{}
	void OnMessage(const std::string m) override {}

	ModelComponent(Model *modelId) : m_model(modelId){}
	Model* getModel()
	{
		return m_model;
	}
};