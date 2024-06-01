#include "EnemyManager.h"
#include "Zenchan.h"
#include "Maita.h"

EnemyManager::EnemyManager()
{
	shots = nullptr;
	map	= nullptr;
}
EnemyManager::~EnemyManager()
{
	Release();
}
AppStatus EnemyManager::Initialise()
{
	ResourceManager& data = ResourceManager::Instance();
	if (data.LoadTexture(Resource::IMG_ZENCHAN, "BubbleBobble_Art/Enemies/zenchanSprite.png") != AppStatus::OK)
	{
		LOG("Failed to load enemies sprite texture");
		return AppStatus::ERROR;
	}

	return AppStatus::OK;
}
void EnemyManager::SetShotManager(ShotManager* shots)
{
	this->shots = shots;
}
void EnemyManager::SetTileMap(TileMap* tilemap)
{
	map = tilemap;
}
void EnemyManager::Add(const Point& pos, EnemyType type, const AABB& area, Look look)
{
	Enemy* enemy;

	if (type == EnemyType::ZENCHAN)
	{
		enemy = new Zenchan(pos, ZENCHAN_PHYSICAL_WIDTH, ZENCHAN_PHYSICAL_HEIGHT, ZENCHAN_FRAME_SIZE, ZENCHAN_FRAME_SIZE);
	}
	//else if (type == EnemyType::MAITA)
	//{
	//	enemy = new Maita(pos, MAITA_PHYSICAL_WIDTH, ZENCHAN_PHYSICAL_HEIGHT, ZENCHAN_FRAME_SIZE, ZENCHAN_FRAME_SIZE);
	//}
	else
	{
		LOG("Internal error: trying to add a new enemy with invalid type");
		return;
	}

	enemy->Initialise(look, area);
	enemy->SetTileMap(map);
	enemies.push_back(enemy);
}
AABB EnemyManager::GetEnemyHitBox(const Point& pos, EnemyType type) const
{
	int width, height;
	if (type == EnemyType::ZENCHAN)
	{
		width = ZENCHAN_PHYSICAL_WIDTH;
		height = ZENCHAN_PHYSICAL_HEIGHT;
	}
	//else if (type == EnemyType::MAITA)
	//{
	//	width = MAITA_PHYSICAL_WIDTH;
	//	height = MAITA_PHYSICAL_HEIGHT;
	//}
	else
	{
		LOG("Internal error while computing hitbox for an invalid enemy type");
		return {};
	}
	Point p(pos.x, pos.y - (height - 1));
	AABB hitbox(p, width, height);
	return hitbox;
}
void EnemyManager::Update(const AABB& player_hitbox, bool& hit)
{
	bool shoot;
	Point p, d;
	AABB box;
	hit = false;

	for (Enemy* enemy : enemies)
	{
		if (enemy->IsAlive()) {
			shoot = enemy->Update(player_hitbox);
			if (shoot)
			{
				enemy->GetShootingPosDir(&p, &d);
				shots->Add(p, d, ShotKind::BUBBLE);
			}
			box = enemy->GetHitbox();
			if (!hit) hit = box.TestAABB(player_hitbox);
		}
	}
}
void EnemyManager::Draw() const
{
	for (const Enemy* enemy : enemies)
		enemy->Draw();
}
void EnemyManager::DrawDebug() const
{
	for (const Enemy* enemy : enemies)
	{
		enemy->DrawHitbox(RED);
	}
}
void EnemyManager::Release()
{
	for (Enemy* enemy : enemies)
		delete enemy;
	enemies.clear();
}