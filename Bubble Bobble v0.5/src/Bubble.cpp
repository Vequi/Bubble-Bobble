#include "Bubble.h"
#include "Sprite.h"
#include "TileMap.h"
#include "Enemy.h"


Bubble::Bubble(const Point& p, const Point& d, int width, int height, int frame_width, int frame_heigth, EnemyType type) :
	Shot(p, d, width, height, frame_width, frame_heigth)
{
	
	bubble_delay = BUBBLE_DELAY;
	map = nullptr;
	alive = true;
	moving = true;
}

Bubble::~Bubble()
{
}
AppStatus Bubble::Initialise()
{
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
	sprite->AddKeyFrame((int)BBAnim::LIGHTRED_STATIC, { 7*n, n, n, n });


	//sprite->SetAnimationDelay((int)BBAnim::RED_STATIC, ANIM_DELAY);
	//sprite->AddKeyFrame((int)BBAnim::RED_STATIC, { n, 3*n, n, n });

	sprite->SetAnimationDelay((int)BBAnim::RED_BLINK, ANIM_DELAY);
	sprite->AddKeyFrame((int)BBAnim::RED_BLINK, { 10*n, n, n, n });
	sprite->AddKeyFrame((int)BBAnim::RED_BLINK, { 7*n, n, n, n });
		
	sprite->SetAnimationDelay((int)BBAnim::POP, ANIM_DELAY);
	sprite->AddKeyFrame((int)BBAnim::POP, { 0, 8*n, n, n });
	sprite->AddKeyFrame((int)BBAnim::POP, { n, 8*n, n, n });


	SetAnimation((int)BBAnim::CAST);
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
void Bubble::Update() {
	MoveX();
	MoveY();
	BubbleCnt();
	Sprite* sprite = dynamic_cast<Sprite*>(render);
	sprite->Update();
}
void Bubble::MoveX ()
{
	AABB box;
	int prev_x = pos.x;
	if (pos.y <= BUBBLE_HEIGHT) {
		//left movement
		if (WINDOW_WIDTH / 2 - BUBBLE_OFFSET + BUBBLE_MARGIN_Y <= pos.x)
		{
			pos.x -= BUBBLE_DASHX;
			dir.x = -dir.x;
		}
		//right movement
		else if (WINDOW_WIDTH / 2 - BUBBLE_OFFSET - BUBBLE_MARGIN_Y >= pos.x)
		{
			pos.x += BUBBLE_DASHX;
			dir.x = -dir.x;
		}
		else 
		{
			if (IsMovingLeft()) {
				pos.x -= BUBBLE_DASHX;
			}
			else if (IsMovingRight()) {
				pos.x += BUBBLE_DASHX;
			}
		}
		
	}

}
void Bubble::MoveY()
{
	if (pos.y > BUBBLE_HEIGHT)
	{
		pos.y -= BUBBLE_DASHY;
	}
}
void Bubble::BubbleCnt()
{
	if (state != BBState::SHOT) {
		bubbleTimer += GetFrameTime();
		if (bubbleTimer >= POP_TIME + 1) {
			alive = false;
		}
		else if(bubbleTimer >= POP_TIME)
		{
			SetAnimation((int)BBAnim::POP);
			moving = false;
		}
		else if(bubbleTimer >= BLINK_TIME){
			state = BBState::BLINK_POP;
			SetAnimation((int)BBAnim::RED_BLINK);
		}
		else if (bubbleTimer >= RED_TIME) {
			state = BBState::RED_POP;
			SetAnimation((int)BBAnim::LIGHTRED_STATIC);
		}
	}
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