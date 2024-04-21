#pragma once
#include "Entity.h"
#include "TileMap.h"

#define BUBBLE_FRAME_SIZE 16
#define BUBBLE_PHYSICAL_WIDTH 12
#define BUBBLE_PHYSICAL_HEIGHT 14

#define BUBBLE_SPEED 1.0f
#define BUBBLE_FORCE 10


enum class BBState { SHOT, NORMAL};
enum class BBDirection { GOING_L, GOING_R };
enum class BBAnim { CAST, IDLE, RED_STATIC, RED_BLINK, POP, NUM_ANIMATIONS };

class Bubble : public Entity {
public:
	Bubble(const Point& p, BBDirection direction);
	~Bubble();

	AppStatus Initialise();
	void SetTileMap(TileMap* tilemap);

	void Update();
	void DrawDebug(const Color& col) const;
	void Release();

	bool IsAlive() const;

private:

	//Bubble Mechanics
	void MoveX();
	void MoveY();
	void BubbleCounter();

	//Bubble destruction;
	bool alive;
	bool move;

	//BubbleTimer
	float bubbleTimer;

	//Animations
	void SetAnimation(int id);
	BBAnim GetAnimation();


	BBState state;
	BBDirection direction;
	TileMap* map;