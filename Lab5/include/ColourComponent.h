#pragma once
#include "Component.h"

#define COLOUR_ADJUSTMENT_VALUE 0.001f

class ColourComponent : public Component
{
public:
	ColourComponent() : m_colourValue(0) {}

	void OnUpdate(float dt) override
	{

	}

	void OnMessage(const std::string m) override
	{
		if (m == "increase")
		{
			m_colourValue += COLOUR_ADJUSTMENT_VALUE;
		}
		else if (m == "decrease")
		{
			m_colourValue -= COLOUR_ADJUSTMENT_VALUE;
		}
	}

	float m_colourValue;
};

class RedComponent : public ColourComponent {};
class GreenComponent : public ColourComponent {};
class BlueComponent : public ColourComponent {};