#include "MiniginPCH.h"
#include "SceneManager.h"
#include "Scene.h"


void dae::SceneManager::Update(float deltaTime)
{
	for(auto scene : mScenes)
	{
		scene->Update(deltaTime);
	}
}

void dae::SceneManager::Render()
{
	for (const auto scene : mScenes)
	{
		scene->Render();
	}
}

std::shared_ptr<dae::Scene> dae::SceneManager::CreateScene(const std::string& name,SceneType type)
{
	std::shared_ptr<Scene> scene{};
	switch (type) 
	{
	case SceneType::Standard:
		scene = std::shared_ptr<Scene>(new Scene(name));
		break;


	default:
		scene = std::shared_ptr<Scene>(new Scene(name));
		break;
	}
	scene->Initialize();
	mScenes.push_back(scene);
	return scene;
}
