#include "Bubble.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include "raymath.h"

Bubble::Bubble(const Point& p, BBDirection dir) :
	Entity(p, BUBBLE_PHYSICAL_WIDTH, BUBBLE_PHYSICAL_HEIGHT, BUBBLE_FRAME_SIZE, BUBBLE_FRAME_SIZE)
{

}

Bubble::~Bubble()
{
}
AppStatus Bubble::Initialise()
{

}