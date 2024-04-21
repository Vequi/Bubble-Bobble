#pragma once
#include "Entity.h"
#include "TileMap.h"

#define BUBBLE_FRAME_SIZE 16
#define BUBBLE_PHYSICAL_WIDTH 12
#define BUBBLE_PHYSICAL_HEIGHT 14

#define BUBBLE_SPEED 1.0f
#define BUBBLE_FORCE 10

#define BUBBLE_DELAY 2

#define BUBBLE_DASHX 5
#define BUBBLE_DASHY 1

//Bubble Timers
#define RED_TIME				7		
#define BLINK_TIME				10
#define POP_TIME				12

enum class BBState { SHOT, STATIC, RED_POP, BLINK_POP};
enum class BBDirection { GOING_L, GOING_R };

enum class BBAnim { 
	CAST, 
	IDLE, 
	LIGHTRED_STATIC, 
	RED_BLINK, 
	POP, 
	NUM_ANIMATIONS 
};

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
	void BubbleCnt();

	//Bubble destruction;
	bool alive;
	bool moving;
	float bubbleTimer;

	//Animation management
	void SetAnimation(int id);
	BBAnim GetAnimation();


	BBState state;
	BBDirection direction;
	TileMap* map;

	int bubble_delay;
};
