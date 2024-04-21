#pragma once
#include <raylib.h>
#include "Sprite.h"
#include "Point.h"
#include "AABB.h"
#include "Globals.h"
#include <unordered_map>

enum class Tile {

	// -1: area covered by an entity
	EMPTY = -1,
	//  0: air tile
	AIR = 0,

	// 0 < id < 54: static tiles

	L1BLOCK = 1, L1BLOCK_NUMT, L1BLOCK_NUMB,

	L6BLOCKTL, L6BLOCKTR, L6BLOCKBL, L6BLOCKBR , L6BLOCK_NUMT, L6BLOCK_NUMB, L6BLOCK_ALT,

	L34BLOCK, L34BLOCK_NUMT1, L34BLOCK_NUMT2, L34BLOCK_NUMB1, L34BLOCK_NUMB2,

	L46BLOCK, L46BLOCK_NUMT1, L46BLOCK_NUMT2, L46BLOCK_NUMB1, L46BLOCK_NUMB2,

	L100BLOCK, L100BLOCK_NUMT1, L100BLOCK_NUMT2, L100BLOCK_NUMB1, L100BLOCK_NUMB2,

	L1SHADE_RIGHT,  L1SHADE_CORNER, L1SHADE_B, L1SHADE_BD, L1SHADE_CORNERB,
	L1SHADE_3D,
	
	L6SHADE_RIGHT, L6SHADE_CORNER, L6SHADE_B, L6SHADE_BD, L6SHADE_CORNERB,
	L6SHADE_3D,

	L34SHADE_RIGHT, L34SHADE_CORNER, L34SHADE_B, L34SHADE_BD, L34SHADE_CORNERB,
	L34SHADE_3D,

	L46SHADE_RIGHT, L46SHADE_CORNER, L46SHADE_B, L46SHADE_BD, L46SHADE_CORNERB,
	L46SHADE_3D,

	L100SHADE_RIGHT, L100SHADE_CORNER, L100SHADE_B, L100SHADE_BD, L100SHADE_CORNERB,
	L100SHADE_3D,

	BLOCK_SQUARE1_TL = 150, BLOCK_SQUARE1_TR, BLOCK_SQUARE1_BL, BLOCK_SQUARE1_BR,
	BLOCK_SQUARE2_TL, BLOCK_SQUARE2_TR, BLOCK_SQUARE2_BL, BLOCK_SQUARE2_BR,
	BLOCK_VERT2_T, BLOCK_VERT2_B, BLOCK_HORIZ2_L, BLOCK_HORIZ2_R, BLOCK_BLUE,
	BLOCK_HORIZ3_L, BLOCK_HORIZ3_M, BLOCK_HORIZ3_R,
	BLOCK_BEAM_L, BLOCK_BEAM_R,
	
	LADDER_L = 120, LADDER_R, LADDER_TOP_L, LADDER_TOP_R,
	LOCK_RED = 125, LOCK_YELLOW, 
	LASER_L = 128, LASER_R,

	// 55 <= id < 100: special tiles
	DOOR = 55,
	KEY_RED = 60, YELLOW_KEY, ITEM_CHERRY, ITEM_BANANA, 
	LASER = 70, LASER_FRAME0, LASER_FRAME1, LASER_FRAME2,

	// id >= 100: entities' initial locations
	PLAYER = 100, ZENCHAN,

	//Intervals
	STATIC_FIRST = L1BLOCK,
	STATIC_LAST = L100SHADE_3D,
	SOLID_FIRST = L1BLOCK,
	SOLID_LAST = L100BLOCK_NUMB2,
	SPECIAL_FIRST = DOOR,
	SPECIAL_LAST = LASER,
	ENTITY_FIRST = PLAYER, 
	ENTITY_LAST = ZENCHAN
};

class TileMap
{
public:
	TileMap();
	~TileMap();

	AppStatus Initialise();
	AppStatus Load(int data[], int w, int h);
	void Update();
	void Render();
	void Release();

	//Test for collisions with walls
	bool TestCollisionWallLeft(const AABB& box) const;
	bool TestCollisionWallRight(const AABB& box) const;
	
	//Test collision with the ground and update 'py' with the maximum y-position to prevent
	//penetration of the grounded tile, that is, the pixel y-position above the grounded tile.
	//Grounded tile = solid tile (blocks) or ladder tops.
	bool TestCollisionGround(const AABB& box, int *py) const;
	
	//Test if there is a ground tile one pixel below the given box
	bool TestFalling(const AABB& box) const;
	
	//Test if box is on ladder and update 'px' with the x-center position of the ladder
	bool TestOnLadder(const AABB& box, int* px) const;
	
	//Test if box is on ladder top and update 'px' with the x-center position of the ladder
	bool TestOnLadderTop(const AABB& box, int* px) const;

private:
	void InitTileDictionary();

	Tile GetTileIndex(int x, int y) const;
	bool IsTileSolid(Tile tile) const;
	bool IsTileLadderTop(Tile tile) const;
	bool IsTileLadder(Tile tile) const;
	bool CollisionX(const Point& p, int distance) const;
	bool CollisionY(const Point& p, int distance) const;
	int GetLadderCenterPos(int pixel_x, int pixel_y) const;

	//Tile map
	Tile *map;

	//Size of the tile map
	int size, width, height;
	
	//Dictionary of tile frames
	std::unordered_map<int, Rectangle> dict_rect;

	Sprite *laser;
	
	//Tile sheet
	const Texture2D *img_tiles;
	const Texture2D *img_objects;
};

