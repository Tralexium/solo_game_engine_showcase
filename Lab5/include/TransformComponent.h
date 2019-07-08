#pragma once
#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/mat4x4.hpp>


class TransformComponent : public Component
{
public:
	glm::vec3 m_position;
	glm::quat m_orientation;
	glm::vec3 m_scale;

	void OnUpdate(float dt) override{}
	void OnMessage(const std::string m) override
	{
		// Rotation
		if (m == "rotL") { yaw(-.05f); }
		else if (m == "rotR") { yaw(.05f); }
		else if (m == "rotU") { pitch(-.05f); }
		else if (m == "rotD") { pitch(.05f); }
		// Translation
		else if (m == "transLeft") { translate(-.05f, .0f, .0f); }
		else if (m == "transRight") { translate(.05f, .0f, .0f); }
		else if (m == "transForw") { translate(.0f, .0f, -.05f); }
		else if (m == "transBack") { translate(.0f, .0f, .05f); }
		// Scale
		else if (m == "scalePosX") { scaleUp(.05f, .0f, .0f); }
		else if (m == "scaleNegX") { scaleUp(-.05f, .0f, .0f); }
		
	}
	TransformComponent() : m_position(0), m_orientation(1,0,0,0), m_scale(1.0f) {}
	TransformComponent(const glm::vec3& pos) : m_position(pos), m_orientation(1, 0, 0, 0), m_scale(1.0f) {}
	TransformComponent(const glm::vec3& pos, const glm::quat& orient) : m_position(pos), m_orientation(orient), m_scale(1.0f) {}
	TransformComponent(const glm::vec3& pos, const glm::quat& orient, const glm::vec3& scale) : m_position(pos), m_orientation(orient), m_scale(scale) {}

	const glm::vec3& position() const { return m_position; }
	const glm::quat& orientation() const { return m_orientation; }
	const glm::vec3& scale() const { return m_scale; }

	glm::mat4 getModelMatrix()
	{
		glm::mat4 transMatrix = glm::translate(glm::mat4(1.0f), m_position);
		glm::mat4 scaleMatrix = glm::scale(glm::mat4(1.0f), m_scale);
		glm::mat4 rotMatrix = glm::mat4_cast(m_orientation);
		return transMatrix * rotMatrix * scaleMatrix;
	}

	void translate(const glm::vec3 &v) { m_position += v; }
	void translate(float x, float y, float z) { m_position += glm::vec3(x, y, z); }
	void setPosition(const glm::vec3 &v) { m_position = v; }
	void setPosition(float x, float y, float z) { m_position = glm::vec3(x, y, z); }

	void rotate(float angle, const glm::vec3 &axis) { m_orientation *= glm::angleAxis(angle, axis); }
	void rotate(float angle, float x, float y, float z) { m_orientation *= glm::angleAxis(angle, glm::vec3(x, y, z)); }
	void setOrientation(float angle, const glm::vec3 &axis) { m_orientation = glm::angleAxis(angle, axis * m_orientation); }
	void setOrientation(float angle, float x, float y, float z) { m_orientation = glm::angleAxis(angle, glm::vec3(x, y, z) * m_orientation); }

	void scaleUp(const glm::vec3 &v) { m_scale += v; }
	void scaleUp(float x, float y, float z) { m_scale += glm::vec3(x, y, z); }
	void setScale(const glm::vec3 &v) { m_scale = v; }
	void setScale(float x, float y, float z) { m_scale = glm::vec3(x, y, z); }

	void yaw(float angle) { rotate(angle, 0.0f, 1.0f, 0.0f); }
	void pitch(float angle) { rotate(angle, 1.0f, 0.0f, 0.0f); }
	void roll(float angle) { rotate(angle, 0.0f, 0.0f, 1.0f); }

	glm::vec3 getEulerAngles()
	{
		return glm::eulerAngles(m_orientation); //pitch yaw roll
	}
};
