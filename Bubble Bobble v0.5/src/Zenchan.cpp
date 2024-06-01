#include "Zenchan.h"
#include "Sprite.h"
#include "TileMap.h"

Zenchan::Zenchan(const Point& p, int width, int height, int frame_width, int frame_height) :
	Enemy(p, ZENCHAN_PHYSICAL_WIDTH, ZENCHAN_PHYSICAL_HEIGHT, ZENCHAN_FRAME_SIZE, ZENCHAN_FRAME_SIZE)
{
	state = ZenchanState::ROAMING;
	attack = 0;
	map = nullptr;
	step = 0;
	frame = 0;
}
Zenchan::~Zenchan()
{
}
AppStatus Zenchan::Initialise(Look look, const AABB& sight)
{
	int i;
	const int n = ZENCHAN_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	render = new Sprite(data.GetTexture(Resource::IMG_ZENCHAN));
	if (render == nullptr)
	{
		LOG("failed to allocate memory for zenchan sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)ZenchanAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)ZenchanAnim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::WALKING_RIGHT, { (float)i * n, 0, -n, n });

	sprite->SetAnimationDelay((int)ZenchanAnim::WALKING_LEFT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::WALKING_LEFT, { (float)i * n, 0, n, n });

	sprite->SetAnimationDelay((int)ZenchanAnim::WALKING_MADRIGHT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::WALKING_MADRIGHT, { (float)i * n, n, n, n });

	sprite->SetAnimationDelay((int)ZenchanAnim::WALKING_MADLEFT, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::WALKING_MADLEFT, { (float)i * n, n, -n, n });

	sprite->SetAnimationDelay((int)ZenchanAnim::DEAD, ANIM_DELAY);
	for (i = 0; i < 4; ++i)
		sprite->AddKeyFrame((int)ZenchanAnim::DEAD, { (float)i * n, 3 * n, n, n });

	this->look = look;
	if (look == Look::LEFT) {
		sprite->SetAnimation((int)ZenchanAnim::WALKING_LEFT);
	}
	else if (look == Look::RIGHT) {
		sprite->SetAnimation((int)ZenchanAnim::WALKING_RIGHT);
	}
	area = sight;
	return AppStatus::OK;
}

bool Zenchan::Update(const AABB& box)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	bool bubl = false;
	if (jumpCDT > 0) {
		jumpCDT -= GetFrameTime();
		isJumping = true;
	}
	else {
		isJumping = false;
	}
	MoveX();
	MoveY();
	StartFalling();
	Teleport();
	sprite->Update();

	return bubl;
}
void Zenchan::MoveX()
{
	AABB box;
	int prev_x = pos.x;
	box = GetHitbox();
	if (!hbox) {
		if (look == Look::RIGHT && state != ZenchanState::FALLING && map->TestCollisionGround(box, &pos.y)) {
			pos.x += ZENCHAN_SPEED;

			if (map->TestCollisionWallRight(box)) {
				pos.x = prev_x;
				look = Look::LEFT;
				SetAnimation((int)ZenchanAnim::WALKING_LEFT);
			}
			else if (map->TestCollisionWallLeft(box)) {
				pos.x = prev_x;
				look = Look::LEFT;
				SetAnimation((int)ZenchanAnim::WALKING_RIGHT);
			}

		}
		else if (look == Look::LEFT && state != ZenchanState::FALLING && map->TestCollisionGround(box, &pos.y)) {
			pos.x -= ZENCHAN_SPEED;

			if (map->TestCollisionWallLeft(box)) {
				pos.x = prev_x;
				look = Look::RIGHT;
				SetAnimation((int)ZenchanAnim::WALKING_RIGHT);
			}
			else if (map->TestCollisionWallRight(box)) {
				pos.x = prev_x;
				look = Look::RIGHT;
				SetAnimation((int)ZenchanAnim::WALKING_RIGHT);
			}
		}
	}

	if (wakywaky && !isJumping) {
		pat += GetFrameTime();
		if (pat < .1) {
			pos.y -= 1;
			hbox = true;
		}

		else if (pat > .8) {
			isJumping = true;
			hbox = false;
			wakywaky = false;
			pat = 0;
			jumpCDT = jumpCD;
		}
	}
}
EnemyType Zenchan::GetEnemyType() const
{
	return EnemyType::ZENCHAN;
}
void Zenchan::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
void Zenchan::StartFalling()
{
	dir.y = 1;
}
void Zenchan::Halt()
{
	dir = { 0, 0 };
	state = ZenchanState::ROAMING;
}
void Zenchan::MoveY()
{
	AABB box, prev_box;
	int prev_x = pos.x;
	int prev_y = pos.y;

	if (wakywaky == true)
	{
		pos.y -= 1;
		hbox = true;
	}

	else {
		hbox = false;
	}

	if (state != ZenchanState::JUMPING && !hbox)
	{
		pos.y += 1;

		box = GetHitbox();
		if (map->TestCollisionGround(box, &pos.y))
		{
			if (state == ZenchanState::FALLING) {
				Halt();
			}
			if (IsKeyPressed(KEY_X)) {
				dir.y = -1;
			}
		}
		else {
			if (state != ZenchanState::FALLING) {
				StartFalling();
			}
		}

	}

}


