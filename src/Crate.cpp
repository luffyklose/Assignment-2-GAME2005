#include "Crate.h"
#include "glm/gtx/string_cast.hpp"
#include "PlayScene.h"
#include "TextureManager.h"
#include "Util.h"

Crate::Crate()
{
	TextureManager::Instance()->load("../Assets/textures/crate.png","crate");

	auto size = TextureManager::Instance()->getTextureSize("crate");
	setWidth(32);
	setHeight(24);

	getTransform()->position = glm::vec2(400.0f, 300.0f);
	getRigidBody()->velocity = glm::vec2(0.0f, 0.0f);
	getRigidBody()->acceleration = glm::vec2(0.0f, 0.0f);
	getRigidBody()->isColliding = false;
	setType(CRATE);
	
	m_currentHeading = 0.0f; // current facing angle
	m_currentDirection = glm::vec2(1.0f, 0.0f); // facing right
	m_isMoving = false;
}


Crate::~Crate()
= default;

void Crate::draw()
{
	// alias for x and y
	const auto x = getTransform()->position.x;
	const auto y = getTransform()->position.y;

	// draw the ship
	TextureManager::Instance()->draw("crate", x, y, getWidth(), getHeight(),m_currentHeading, 255, true);
}

void Crate::update()
{
	if(m_isMoving)
	{
		move();
		//std::cout << "moving" << std::endl;
	}		
}

void Crate::clean()
{
}

void Crate::move()
{
	//std::cout << "acc:" << getRigidBody()->acceleration.x << " " << getRigidBody()->acceleration.y << std::endl;
	getTransform()->position += glm::vec2(getRigidBody()->velocity.x/10, getRigidBody()->velocity.y/10);

	getRigidBody()->velocity += glm::vec2(getRigidBody()->acceleration.x, getRigidBody()->acceleration.y);
	//std::cout << "speed: " << getRigidBody()->velocity.x << " " << getRigidBody()->velocity.y << std::endl;
}

float Crate::getAngle()
{
	return m_currentHeading;
}

bool Crate::getIsMoving()
{
	return m_isMoving;
}

void Crate::SetAngle(float angle)
{
	m_currentHeading = angle;
}

void Crate::setIsMoving(bool isMoving)
{
	m_isMoving = isMoving;
}

void Crate::m_checkBounds()
{

	if (getTransform()->position.x > Config::SCREEN_WIDTH)
	{
		getTransform()->position = glm::vec2(0.0f, getTransform()->position.y);
	}

	if (getTransform()->position.x < 0)
	{
		getTransform()->position = glm::vec2(800.0f, getTransform()->position.y);
	}

	if (getTransform()->position.y > Config::SCREEN_HEIGHT)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 0.0f);
	}

	if (getTransform()->position.y < 0)
	{
		getTransform()->position = glm::vec2(getTransform()->position.x, 600.0f);
	}

}

void Crate::m_reset()
{
	getRigidBody()->isColliding = false;
	const int halfWidth = getWidth() * 0.5f;
	const auto xComponent = rand() % (640 - getWidth()) + halfWidth + 1;
	const auto yComponent = -getHeight();
	getTransform()->position = glm::vec2(xComponent, yComponent);
}

