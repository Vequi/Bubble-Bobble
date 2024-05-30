#include "Shot.h"
#include "StaticImage.h"
#include "Enemy.h"

Shot::Shot() : Entity() {
	map = nullptr;
}
Shot::Shot(const Point& p, const Point& d, int width, int heigth, int frame_width, int frame_height) :
	Entity(p, d, width, heigth, frame_width, frame_height)
{
	map = nullptr;
}
Shot::~Shot()
{
}
void Shot::Init(const Point& p, const Point& d)
{
	Set(p, d, SHOT_PHYSICAL_WIDTH, SHOT_PHYSICAL_HEIGHT, SHOT_FRAME_WIDTH, SHOT_FRAME_HEIGHT);
}
void Shot::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
void Shot::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Shot::IsMovingLeft() const
{
	return dir.x < 0;
}
bool Shot::IsMovingRight() const
{
	return dir.x > 0;
}
void Shot::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}