#include "Enemy.h"
#include "Sprite.h"

Enemy::Enemy(const Point& p, int width, int height, int frame_width, int frame_height) :
	Entity(p, width, height, frame_width, frame_height)
{
	area = {};
	look = Look::LEFT;
	map = nullptr;
}
Enemy::~Enemy()
{
}
void Enemy::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void Enemy::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
bool Enemy::IsVisible(const AABB& hitbox)
{
	if (area.TestAABB(hitbox))
	{
		if (look == Look::LEFT) {
			if (pos.x > hitbox.pos.x + hitbox.width)	return true;
		} else if (look == Look::RIGHT) {
			if (pos.x + width < hitbox.pos.x)	return true;
		}
	}
	return false;
}
