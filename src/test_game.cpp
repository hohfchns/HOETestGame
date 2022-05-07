#include <iostream>
#include "player.h"
#include "block.h"
#include "level_loader.h"
#include "game.h"


std::vector<Block> g_Blocks;

Player g_Player = Player();

Game& g_Game = Game::GetInstance();


void AddBlocks()
{
    LevelLoader loader = LevelLoader(24);
    std::string path = std::string(SDL_GetBasePath()) + "resources/level.hfl";
    g_Blocks = loader.loadBlocks(path, '#');

    HFMath::Vector2 offset = HFMath::Vector2(-8, 0);

    for (int i = 0; i < g_Blocks.size(); i++)
    {
        g_Blocks[i].m_Transform.SetGlobalPosition(g_Blocks[i].m_Transform.GetGlobalPosition() + offset);
        g_Game.AddRenderComponent(&g_Blocks[i]);
    }
}


void HOEINIT_AddComponents()
{
    g_Game.AddComponent(&g_Player);
    AddBlocks();
}