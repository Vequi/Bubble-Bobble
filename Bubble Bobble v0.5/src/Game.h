#pragma once
#include "Globals.h"
#include "Scene.h"

enum class GameState { INITIAL1, INITIAL2, MAIN_MENU, PLAYING, INSERT_COIN, PLAYER_SELC, SCROLL, SETTINGS, CREDITS };

class Game
{
public:
    Game();
    ~Game();

    AppStatus Initialise(float scale);
    AppStatus Update();
    void Render();
    void Cleanup();

private:
    AppStatus BeginPlay();
    void FinishPlay();

    void RenderScore();

    AppStatus LoadResources();
    void UnloadResources();

    GameState state;
    Scene *scene;
    const Texture2D* img_Initial1;
    const Texture2D* img_Initial2;
    const Texture2D *img_menu;
    const Texture2D* img_insertcoin;
    const Texture2D* img_player_selc;
    const Texture2D* img_lvl1;
    const Texture2D* img_lvl6;
    const Texture2D* img_lvl34;
    const Texture2D* img_lvl46;
    const Texture2D* img_lvl100;
    const Texture2D* img_scoreUI;
    float transitionTimer;
    int time=0;
    float timer=0;
    float total_time=2.0f;
    float time_elapsed=0.0f;
    int clickCount;
    const int maxClicks = 20;

    //To work with original game units and then scale the result
    RenderTexture2D target;
    Rectangle src, dst;
};