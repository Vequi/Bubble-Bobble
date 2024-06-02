#pragma once
#include "Entity.h"

#define OBJECT_PHYSICAL_SIZE	14
#define OBJECT_FRAME_SIZE		16

#define POINTS_CHERRY	100
#define POINTS_BANANA	200
#define POINTS_CAKE     300 
#define POINTS_BOTTLE   400
#define POINTS_ICECREAM   500
#define POINTS_REDGEM   600

enum class ObjectType { CHERRY, BANANA, CAKE, BOTTLE, ICECREAM, REDGEM };

class Object : public Entity
{
public:
	Object(const Point& p, ObjectType t);
	~Object();

	void DrawDebug(const Color& col) const;
	int Points() const;

private:
	ObjectType type;
};

