#include "Enemy.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include <raymath.h>

Sound enemySound[10];

Enemy::Enemy(const Point& p, EnemyState s, EnemyLook view) :
	Entity(p, ENEMY_PHYSICAL_WIDTH, ENEMY_PHYSICAL_HEIGHT, ENEMY_FRAME_SIZE, ENEMY_FRAME_SIZE)
{
	state = s;
	look = view;
	jump_delay = ENEMY_JUMP_DELAY;
	map = nullptr;

}
Enemy::~Enemy()
{
}
AppStatus Enemy::Initialise()
{
	int i;
	const int n = ENEMY_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ZENCHAN, "BubbleBobble_Art/Enemies/zenchanSprite.png") != AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_ZENCHAN));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for enemy zenchan sprite");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)EnemyAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)EnemyAnim::IDLE_RIGHT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::IDLE_RIGHT, { 0 * n, 0 * n, -n, n });
	sprite->AddKeyFrame((int)EnemyAnim::IDLE_RIGHT, { 2 * n, 0 * n, -n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::IDLE_LEFT, ANIM_DELAY);
	sprite->AddKeyFrame((int)EnemyAnim::IDLE_LEFT, { 0 * n, 0 * n, n, n });
	sprite->AddKeyFrame((int)EnemyAnim::IDLE_LEFT, { 2 * n, 0 * n, n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_RIGHT, ANIM_DELAY);
	for (i = 1; i < 5; ++i)
		sprite->AddKeyFrame((int)EnemyAnim::WALKING_RIGHT, { (float)i * n, 0 * n, -n, n });

	sprite->SetAnimationDelay((int)EnemyAnim::WALKING_LEFT, ANIM_DELAY);
	for (i = 1; i < 5; ++i)
		sprite->AddKeyFrame((int)EnemyAnim::WALKING_LEFT, { (float)i * n, 0 * n, n, n });


	sprite->SetAnimation((int)EnemyAnim::IDLE_RIGHT);

	return AppStatus::OK;
}

void Enemy::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
bool Enemy::IsLookingRight() const
{
	return look == EnemyLook::RIGHT;
}
bool Enemy::IsLookingLeft() const
{
	return look == EnemyLook::LEFT;
}
bool Enemy::IsAscending() const
{
	return dir.y < -ENEMY_LEVITATING_SPEED;
}
bool Enemy::IsLevitating() const
{
	return abs(dir.y) <= ENEMY_LEVITATING_SPEED;
}
bool Enemy::IsDescending() const
{
	return dir.y > ENEMY_LEVITATING_SPEED;
}
bool Enemy::IsInFirstHalfTile() const
{
	return pos.y % TILE_SIZE < TILE_SIZE / 2;
}
bool Enemy::IsInSecondHalfTile() const
{
	return pos.y % TILE_SIZE >= TILE_SIZE / 2;
}
void Enemy::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
EnemyAnim Enemy::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (EnemyAnim)sprite->GetAnimation();
}

void Enemy::Update()
{
	//Player doesn't use the "Entity::Update() { pos += dir; }" default behaviour.
	//Instead, uses an independent behaviour for each axis.
	MoveX();
	MoveY();

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();

	Teleport();
}
void Enemy::MoveX()
{
	AABB box;
	int prev_x = pos.x;
	box = GetHitbox();

	//We can only go up and down while climbing
	if (look == EnemyLook::RIGHT)
	{
		pos.x += ENEMY_SPEED;
		if (map->TestCollisionWallRight(box))
		{
			pos.x = prev_x;
			look = EnemyLook::LEFT;
			SetAnimation((int)EnemyAnim::WALKING_LEFT);
		}

	}
	else if (look == EnemyLook::LEFT)
	{
		pos.x += -ENEMY_SPEED;
		if (map->TestCollisionWallLeft(box))
		{
			pos.x = prev_x;
			look = EnemyLook::RIGHT;
			SetAnimation((int)EnemyAnim::WALKING_RIGHT);
		}

	}
}
void Enemy::MoveY()
{
	/*AABB box;

	if (state == EnemyState::JUMPING)
	{
		LogicJumping();
	}
	else if (state == EnemyState::CLIMBING)
	{
		LogicClimbing();
	}
	else //idle, walking, falling
	{
		pos.y += ENEMY_SPEED;
		box = GetHitbox();
		if (map->TestCollisionGround(box, &pos.y))
		{
			if (state == EnemyState::FALLING) Stop();

			if (IsKeyDown(KEY_DOWN))
			{
				box = GetHitbox();
				if (map->TestOnLadder(box, &pos.x))
					StartClimbingUp();
			}
			else if (IsKeyDown(KEY_S))
			{
				//To climb up the ladder, we need to check the control point (x, y)
				//To climb down the ladder, we need to check pixel below (x, y+1) instead
				box = GetHitbox();
				box.pos.y++;
				if (map->TestOnLadderTop(box, &pos.x))
				{
					StartClimbingDown();
					pos.y += ENEMY_LADDER_SPEED;
				}

			}
			else if (IsKeyPressed(KEY_W))
			{
				StartJumping();
			}
		}
		else
		{
			if (state != EnemyState::FALLING) StartFalling();
		}
	}
}
void Enemy::LogicJumping()
{
	AABB box, prev_box;
	int prev_y;

	jump_delay--;
	if (jump_delay == 0)
	{
		prev_y = pos.y;
		prev_box = GetHitbox();

		pos.y += dir.y;
		dir.y += GRAVITY_FORCE;
		jump_delay = ENEMY_JUMP_DELAY;

		//Is the jump finished?
		if (dir.y > ENEMY_JUMP_FORCE)
		{
			dir.y = ENEMY_SPEED;
			StartFalling();
		}
		else
		{
			//Jumping is represented with 3 different states
			if (IsAscending())
			{
				if (IsLookingRight())	SetAnimation((int)EnemyAnim::JUMPING_RIGHT);
				else					SetAnimation((int)EnemyAnim::JUMPING_LEFT);
			}
			else if (IsLevitating())
			{
				if (IsLookingRight())	SetAnimation((int)EnemyAnim::LEVITATING_RIGHT);
				else					SetAnimation((int)EnemyAnim::LEVITATING_LEFT);
			}
			else if (IsDescending())
			{
				if (IsLookingRight())	SetAnimation((int)EnemyAnim::FALLING_RIGHT);
				else					SetAnimation((int)EnemyAnim::FALLING_LEFT);
			}
		}
		//We check ground collision when jumping down
		if (dir.y >= 0)
		{
			box = GetHitbox();

			//A ground collision occurs if we were not in a collision state previously.
			//This prevents scenarios where, after levitating due to a previous jump, we found
			//ourselves inside a tile, and the entity would otherwise be placed above the tile,
			//crossing it.
			if (!map->TestCollisionGround(prev_box, &prev_y) &&
				map->TestCollisionGround(box, &pos.y))
			{
				Stop();
			}
		}
	}
}
void Enemy::LogicClimbing()
{
	AABB box;
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	int tmp;

	if (IsKeyDown(KEY_DOWN))
	{
		pos.y -= ENEMY_LADDER_SPEED;
		sprite->NextFrame();
	}
	else if (IsKeyDown(KEY_S))
	{
		pos.y += ENEMY_LADDER_SPEED;
		sprite->PrevFrame();
	}

	//It is important to first check LadderTop due to its condition as a collision ground.
	//By doing so, we ensure that we don't stop climbing down immediately after starting the descent.
	box = GetHitbox();
	if (map->TestOnLadderTop(box, &tmp))
	{
		if (IsInSecondHalfTile())		SetAnimation((int)EnemyAnim::CLIMBING_PRE_TOP);
		else if (IsInFirstHalfTile())	SetAnimation((int)EnemyAnim::CLIMBING_TOP);
		else					LOG("Internal error, tile should be a LADDER_TOP, coord: (%d,%d)", box.pos.x, box.pos.y);
	}
	else if (map->TestCollisionGround(box, &pos.y))
	{
		//Case leaving the ladder descending.
		Stop();
		sprite->SetAutomaticMode();
	}
	else if (!map->TestOnLadder(box, &tmp))
	{
		//Case leaving the ladder ascending.
		//If we are not in a LadderTop, colliding ground or in the Ladder it means we are leaving
		//ther ladder ascending.
		Stop();
		sprite->SetAutomaticMode();
	}
	else
	{
		if (GetAnimation() != EnemyAnim::CLIMBING)	SetAnimation((int)EnemyAnim::CLIMBING);
	}*/
}
void Enemy::DrawDebug(const Color& col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);

}
void Enemy::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_ZENCHAN);

	render->Release();
}