#pragma once

#include "raylib.h"
#include "Globals.h"
#include <unordered_map>
#include <string>

enum class Resource {
    IMG_INITIAL1,
    IMG_INITIAL2,
    IMG_MENU,
    IMG_INSERTCOIN,
    IMG_PLAYER_SELC,
    IMG_LVL1,
    IMG_LVL6,
    IMG_LVL34,
    IMG_LVL46,
    IMG_LVL100,
    IMG_SCORE,
    IMG_PLAYER, 
    IMG_PLAYER2,
    IMG_BUBBLES,
    IMG_TILES,
    IMG_OBJECTS,
    IMG_POWERUPS
};

class ResourceManager {
public:
    //Singleton instance retrieval
    static ResourceManager& Instance()
    {
        static ResourceManager instance; //Guaranteed to be initialized only once
        return instance;
    }

    //Load and unload texture
    AppStatus LoadTexture(Resource id, const std::string& file_path);
    void ReleaseTexture(Resource id);

    //Get texture by key
    const Texture2D* GetTexture(Resource id) const;

    //Release resources
    void Release();

    //Ensure Singleton cannot be copied or assigned
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

private:
    //Private constructor to prevent instantiation
    ResourceManager();
    //Destructor
    ~ResourceManager();

    //Dictionary to store loaded textures
    std::unordered_map<Resource, Texture2D> textures;
};