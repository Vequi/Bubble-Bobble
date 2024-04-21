#pragma once
#include "Entity.h"
#include "TileMap.h"


//Representation model size: 32x32
#define ENEMY_FRAME_SIZE		16

//Logical model size: 12x28
#define ENEMY_PHYSICAL_WIDTH	14
#define ENEMY_PHYSICAL_HEIGHT	14

//Horizontal speed and vertical speed while falling down
#define ENEMY_SPEED				1

//Frame animation delay while on a ladder
#define ANIM_LADDER_DELAY		(2*ANIM_DELAY)

//When jumping, initial jump speed and maximum falling speed
#define ENEMY_JUMP_FORCE		10

//Frame delay for updating the jump velocity
#define ENEMY_JUMP_DELAY		2

//Enemy is levitating when abs(speed) <= this value
#define ENEMY_LEVITATING_SPEED	2

//Gravity affects jumping velocity when jump_delay is 0
#define GRAVITY_FORCE			1

//Logic states
enum class EnemyState { IDLE, WALKING };
enum class EnemyLook { RIGHT, LEFT };

//Rendering states
enum class EnemyAnim {
    IDLE_LEFT, IDLE_RIGHT,
    WALKING_LEFT, WALKING_RIGHT,
    DEAD, BUBBLE,
    NUM_ANIMATIONS
};

class Enemy : public Entity
{
public:
    Enemy(const Point& p, EnemyState s, EnemyLook view);
    ~Enemy();

    AppStatus Initialise();
    void SetTileMap(TileMap* tilemap);

    void Update();
    void DrawDebug(const Color& col) const;
    void Release();

private:
    bool IsLookingRight() const;
    bool IsLookingLeft() const;

    //Enemy mechanics
    void MoveX();
    void MoveY();
    //void LogicJumping();
   // void LogicClimbing();

    //Animation management
    void SetAnimation(int id);
    EnemyAnim GetAnimation();
    /*void Stop();
    void StartWalkingLeft();
    void StartWalkingRight();
    void StartFalling();
    void StartJumping();
    void StartClimbingUp();
    void StartClimbingDown();
    void ChangeAnimRight();
    void ChangeAnimLeft();*/

    //Jump steps
    bool IsAscending() const;
    bool IsLevitating() const;
    bool IsDescending() const;

    //Ladder get in/out steps
    bool IsInFirstHalfTile() const;
    bool IsInSecondHalfTile() const;

    EnemyState state;
    EnemyLook look;
    int jump_delay;

    TileMap* map;

   
};

