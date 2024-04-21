#include "Bubble.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Globals.h"
#include "raymath.h"

Bubble::Bubble(const Point& p, BBDirection dir) :
	Entity(p, BUBBLE_PHYSICAL_WIDTH, BUBBLE_PHYSICAL_HEIGHT, BUBBLE_FRAME_SIZE, BUBBLE_FRAME_SIZE)
{
	direction = dir;
	bubble_delay = BUBBLE_DELAY;
	map = nullptr;
}

Bubble::~Bubble()
{
}
AppStatus Bubble::Initialise()
{
	int i;
	const int n = BUBBLE_FRAME_SIZE;

	ResourceManager& data = ResourceManager::Instance();
	if(data.LoadTexture(Resource::IMG_BUBBLES, "BubbleBobble_Art/Bubbles&Items/bubbles.png")!=AppStatus::OK)
	{
		return AppStatus::ERROR;
	}

	render = new Sprite(data.GetTexture(Resource::IMG_BUBBLES));
	if (render == nullptr)
	{
		LOG("Failed to allocate memory for render");
		return AppStatus::ERROR;
	}

	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetNumberAnimations((int)BBAnim::NUM_ANIMATIONS);

	sprite->SetAnimationDelay((int)BBAnim::CAST, ANIM_DELAY);
	sprite->AddKeyFrame((int)BBAnim::CAST, { 0, 0, n, n });
	sprite->AddKeyFrame((int)BBAnim::CAST, { n, 0, n, n });
	sprite->AddKeyFrame((int)BBAnim::CAST, { 3*n, 0, n, n });
	sprite->AddKeyFrame((int)BBAnim::CAST, { n, n, n, n });

	sprite->SetAnimationDelay((int)BBAnim::IDLE, ANIM_DELAY);
	sprite->AddKeyFrame((int)BBAnim::IDLE, { n, n, n, n });

	sprite->SetAnimationDelay((int)BBAnim::LIGHTRED_STATIC, ANIM_DELAY);
	sprite->AddKeyFrame((int)BBAnim::LIGHTRED_STATIC, { n, 2*n, n, n });

	sprite->SetAnimationDelay((int)BBAnim::RED_STATIC, ANIM_DELAY);
	sprite->AddKeyFrame((int)BBAnim::RED_STATIC, { n, 3*n, n, n });

	sprite->SetAnimationDelay((int)BBAnim::RED_BLINK, ANIM_DELAY);
	for (i = 2; i < 4; i++)
		sprite->AddKeyFrame((int)BBAnim::RED_BLINK, { n, (float)i*n, n, n });
		
	sprite->SetAnimationDelay((int)BBAnim::POP, ANIM_DELAY);
	for(i = 0; i < 2; i++)
		sprite->AddKeyFrame((int)BBAnim::POP, { (float)i*n, 10*n, n, n });

}
void Bubble::SetTileMap(TileMap* tilemap) 
{
	map = tilemap;
}
bool Bubble::IsAlive()const {
	if (alive == true) {
		return true;
	}
	else
	{
		return false;
	}
}
void Bubble::SetAnimation(int id)
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->SetAnimation(id);
}
BBAnim Bubble::GetAnimation()
{
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	return (BBAnim)sprite->GetAnimation();
}
void Bubble::DrawDebug(const Color & col) const
{
	Entity::DrawHitbox(pos.x, pos.y, width, height, col);
}
void Bubble::Release()
{
	ResourceManager& data = ResourceManager::Instance();
	data.ReleaseTexture(Resource::IMG_BUBBLES);

	render->Release();
}