#pragma once
#include "Entity.h"
#include "TileMap.h"
#include "Enemy.h"


#define SHOT_PHYSICAL_WIDTH		12
#define SHOT_PHYSICAL_HEIGHT	14
#define SHOT_FRAME_WIDTH		16
#define SHOT_FRAME_HEIGHT		16

enum class ShotKind { BUBBLE, LIGHTNING };

class Shot : public Entity {

public:
	Shot();
	Shot(const Point& p, const Point& d, int width, int heigth, int frame_width, int frame_height);
	virtual ~Shot();

	void Init(const Point& p, const Point& d);
	bool IsMovingLeft() const;
	bool IsMovingRight() const;
	void DrawDebug(const Color& col) const;
	void SetTileMap(TileMap* tilemap);
	
protected:

	TileMap* map;
	void SetAnimation(int id);

};