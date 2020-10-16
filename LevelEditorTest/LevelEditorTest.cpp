// LevelEditorTest.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include "pch.h"
#include <SDL.h>
#include <iostream>
#include "Renderer.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include <chrono>
#include "Scene.h"
#include "Grid.h"
#include "BinaryWriter.h"
#include "vld.h"
#include "Level.h"

struct BlockToPlace
{
	BlockToPlace(std::string name, GameObjectType type)
	{
		fileName = name;
		objType = type;
	};
	std::string fileName;
	GameObjectType objType;
};

SDL_Window* window;
Level* level;
SDL_Point mouse;
BinaryWriter writer;
std::shared_ptr<dae::Scene> pScene;
int gridSize{ 22 };
int blockArrInd{ 0 };
int enemyArrInd{0};
bool MouseWasPressed{};
bool RightArrowWasPressed{};
bool LeftArrowWasPressed{};
bool SpaceWasPressed{};
bool PlaceEnemies{ false };
GameObjectType arrPossibleType[2]
{
	GameObjectType{GameObjectType::ZenChan},
	GameObjectType{GameObjectType::Maita}
};
GameObjectType currType = arrPossibleType[enemyArrInd];

BlockToPlace arrPossibleBlocks[3]
{
	BlockToPlace{"PlatformBlock.png",GameObjectType::Platformblock},
	BlockToPlace{"WallBlock.png",GameObjectType::Wallblock},
	BlockToPlace{"AirBlock.png",GameObjectType::Airblock},
};
BlockToPlace currBlock = arrPossibleBlocks[blockArrInd];

void InitializeSDL()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0)
	{
		throw std::runtime_error(std::string("SDL_Init Error: ") + SDL_GetError());
	}
	
	window = SDL_CreateWindow(
		"LevelEditor",
		SDL_WINDOWPOS_UNDEFINED,
		SDL_WINDOWPOS_UNDEFINED,
		750,
		550,
		SDL_WINDOW_OPENGL
	);

	if (window == nullptr)
	{
		throw std::runtime_error(std::string("SDL_CreateWindow Error: ") + SDL_GetError());
	}
	dae::Renderer::GetInstance().Init(window);
}

void LoadGame()
{
	//SET UP BASE SCENE
    pScene = dae::SceneManager::GetInstance().CreateScene("Editor", dae::SceneType::Standard);
	level = new Level(550, 550, gridSize, gridSize);
	auto vecGO = level->GetGrid()->GetGameObjects();
	for (size_t i{}; i < vecGO.size(); i++)
	{
		(*pScene).Add(vecGO[i]);
	}
}

void CleanUp()
{
	dae::Renderer::GetInstance().Destroy();
	SDL_DestroyWindow(window);
	window = nullptr;
	SDL_Quit();
	delete level;
	level = nullptr;
}

void SaveFile()
{
	std::string name;
	std::cout << "Please enter a name for this file: ";
	std::cin >> name;
	level->WriteToFile(writer, name.c_str());
	std::cout << "File has been written";
}



bool ProcessSDLInput()
{
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			return false;
		}
		if ((GetKeyState(VK_SPACE) & 0x8000) && !SpaceWasPressed)
		{
			SpaceWasPressed = true;
			SaveFile();
		};

		if ((GetKeyState(VK_UP) & 0x8000) && !PlaceEnemies)
		{
			std::cout << "\nENEMY PLACEMENT ACTIVE\n";
			PlaceEnemies = true;
		}

		if ((GetKeyState(VK_DOWN) & 0x8000) && PlaceEnemies)
		{
			std::cout << "\nBLOCK PLACEMENT ACTIVE\n";
			PlaceEnemies = false;
		}

		    //UPDATE BLOCK/ENEMY YOU CAN PLACE
			if ((GetKeyState(VK_RIGHT) & 0x8000) && !RightArrowWasPressed)
			{
				
				RightArrowWasPressed = true;
				if (!PlaceEnemies)
				{
					++blockArrInd;
					if (blockArrInd >= 3)
					{
						blockArrInd = 0;
					}
					currBlock = arrPossibleBlocks[blockArrInd];
				}
				else
				{
					++enemyArrInd;
					if (enemyArrInd >=2)
					{
						enemyArrInd = 0;
					}
					currType = arrPossibleType[enemyArrInd];
				}

			};

			if ((GetKeyState(VK_LEFT) & 0x8000) && !LeftArrowWasPressed)
			{
				LeftArrowWasPressed = true;
				if (!PlaceEnemies)
				{
					--blockArrInd;
					if (blockArrInd < 0)
					{
						blockArrInd = 2;
					}
					currBlock = arrPossibleBlocks[blockArrInd];
				}
				else
				{
					--enemyArrInd;
					if (enemyArrInd < 0)
					{
						enemyArrInd = 1;
					}
					currType = arrPossibleType[enemyArrInd];
				}

			};

			if (RightArrowWasPressed && !(GetKeyState(VK_RIGHT) & 0x8000))
			{
				RightArrowWasPressed = false;
			}

			if (LeftArrowWasPressed && !(GetKeyState(VK_LEFT) & 0x8000))
			{
				LeftArrowWasPressed = false;
			}
		
		if (e.type == SDL_MOUSEBUTTONDOWN)
		{
			MouseWasPressed = true;
		}
		if (e.type == SDL_MOUSEBUTTONUP)
		{
			MouseWasPressed = false;
		}
		if (e.type == SDL_MOUSEMOTION)
		{
			SDL_GetMouseState(&mouse.x, &mouse.y);
		}
	}
return  true;
}


int main(int, char* [])
{
	
	InitializeSDL();

	// tell the resource manager where he can find the game data
	dae::ResourceManager::GetInstance().Init("Resources/");

	LoadGame();
	{
		auto t = std::chrono::high_resolution_clock::now();
		auto& renderer = dae::Renderer::GetInstance();
		auto& sceneManager = dae::SceneManager::GetInstance();
		bool doContinue = true;
		std::cout << "USE THE LEFT AND RIGHT ARROW TO CYCLE BETWEEN THE DIFFERENT BLOCKS" << std::endl;

		while (doContinue)
		{
			auto current = std::chrono::high_resolution_clock::now();
			float deltaTime(std::chrono::duration<float>(current - t).count());
			doContinue = ProcessSDLInput();
			sceneManager.Update(deltaTime);
			SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 0x00, 0x00, 0x00, 0x00);
			SDL_RenderClear(dae::Renderer::GetInstance().GetSDLRenderer());
			renderer.Render();
			auto rects = level->GetGrid()->GetGridRects();
			int RectIndex = level->GetGrid()->PositionToIndexRect(mouse);
			SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 0xFF, 0x00, 0x00, 0xFF);
			SDL_RenderDrawRects(dae::Renderer::GetInstance().GetSDLRenderer(),rects,int(gridSize*gridSize));
			SDL_SetRenderDrawColor(dae::Renderer::GetInstance().GetSDLRenderer(), 0x00, 0xFF, 0x00, 0xFF);
			if (RectIndex >=0 && RectIndex < gridSize*gridSize)
			{
				SDL_RenderDrawRect(dae::Renderer::GetInstance().GetSDLRenderer(), &rects[RectIndex]);
				if (MouseWasPressed && !PlaceEnemies)
				{
					auto blocks = level->GetGrid()->GetBlocks();
					for (size_t i{}; i < gridSize * gridSize; i++)
					{
						if (blocks[i].GetRect().x == rects[RectIndex].x &&
							blocks[i].GetRect().y == rects[RectIndex].y)
						{
							blocks[i].ChangeTextureAndType(currBlock.fileName, currBlock.objType);
						}
					}
				}
				else if(MouseWasPressed && PlaceEnemies)
				{
					auto newEnemy = level->CreateNewEnemy(mouse.x, mouse.y - (550 / 22), currType);
					pScene->Add(newEnemy);
					MouseWasPressed = false;
				}
			}
			SDL_RenderPresent(dae::Renderer::GetInstance().GetSDLRenderer());
			t = current;
		}
	}

	CleanUp();
	return 0;
}