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
	int radius() const { return m_Radius; }

	void moveUp();
	void moveDown();
	void moveLeft();
	void moveRight();

private:
	glm::vec2 m_Position;
	int m_Radius; // radius in pixel
	float m_MoveSpeed;

	int m_WinWidth;
	int m_WinHeight;
};