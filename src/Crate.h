#pragma once
#ifndef __SHIP__
#define __SHIP__

#include "DisplayObject.h"
#include "TextureManager.h"

class Crate final : public DisplayObject
{
public:
	Crate();
	~Crate();

	// Inherited via GameObject
	void draw() override;
	void update() override;
	void clean() override;
	
	void move();

	// getters
	float getAngle();
	bool getIsMoving();

	// setters
	void SetAngle(float angle);
	void setIsMoving(bool isMoving);

private:
	void m_checkBounds();
	void m_reset();

	float m_currentHeading;
	glm::vec2 m_currentDirection;
	glm::vec2 m_targetPosition;
	bool m_isMoving;
};


#endif /* defined (__SHIP__) */

