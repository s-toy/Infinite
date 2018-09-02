#pragma once

#include <glm/glm.hpp>

class CMeshRenderer;

class CPlayer
{
public:
	CPlayer::CPlayer();
	virtual CPlayer::~CPlayer();

	void setPosition(const glm::vec2& vVec2) { m_Position = vVec2; }
	void setSpeed(float vSpeed) { m_MoveSpeed = vSpeed; }
	glm::vec2 position() const { return m_Position; }
	float radius() const { return m_Radius; }

	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

private:
	glm::vec2 m_Position;
	float m_Radius;
	float m_MoveSpeed;
};