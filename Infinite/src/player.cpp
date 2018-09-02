#include "player.h"
#include "constants.h"
#include "common.h"
#include "meshRenderer.h"

CPlayer::CPlayer() : m_Position(0.0f), m_MoveSpeed(0.0f), m_Radius(0.01f)
{

}

CPlayer::~CPlayer()
{

}

//*********************************************************************************
//FUNCTION:
void CPlayer::moveUp()
{
	if (m_Position.y > 1.0f)
		m_Position.y = 1.0f;
	else
		m_Position.y += m_MoveSpeed;
}

//*********************************************************************************
//FUNCTION:
void CPlayer::moveDown()
{
	if (m_Position.y < -1.0f)
		m_Position.y = -1.0f;
	else
		m_Position.y -= m_MoveSpeed;
}

//*********************************************************************************
//FUNCTION:
void CPlayer::moveLeft()
{
	if (m_Position.x < -1.0f)
		m_Position.x = -1.0f;
	else
		m_Position.x -= m_MoveSpeed;
}

//*********************************************************************************
//FUNCTION:
void CPlayer::moveRight()
{
	if (m_Position.x > 1.0f)
		m_Position.x = 1.0f;
	else
		m_Position.x += m_MoveSpeed;
}