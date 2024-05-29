#pragma once
#include "Entity.h"


#define ZENCHAN_FRAME_SIZE		16
#define ZENCHAN_PHYSICAL_WIDTH	12
#define ZENCHAN_PHYSICAL_HEIGHT	14


#define MAITA_FRAME_SIZE		16
#define MAITA_PHYSICAL_WIDTH	12
#define MAITA_PHYSICAL_HEIGHT	14

enum class EnemyType { ZENCHAN, MAITA };

class Enemy : public Entity
{
public:
	Enemy(const Point& p, int width, int height, int frame_width, int frame_height);
	virtual ~Enemy();

	//Draw the maximum visibility area of the enemy
	void DrawVisibilityArea(const Color& col) const;

	//Pure virtual functions, any class inheriting from this class must provide its own implementations

	//Initialize the enemy with the specified look and area
	virtual AppStatus Initialise(Look look, const AABB& area) = 0;

	//Update the enemy according to its logic, return true if the enemy must shoot
	virtual bool Update(const AABB& box) = 0;

	//Retrieve the position and direction of the shot to be thrown
	virtual void GetShootingPosDir(Point* pos, Point* dir) const = 0;

	//Get the type of enemy
	virtual EnemyType GetEnemyType() const = 0;

protected:
	//Return true if the given hitbox is within the visibility area and the enemy is facing it
	bool IsVisible(const AABB& hitbox);

	Look look;
	AABB visibility_area;
};

