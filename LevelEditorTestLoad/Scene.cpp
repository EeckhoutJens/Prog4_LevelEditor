#include "MiniginPCH.h"
#include "Scene.h"
#include "GameObject.h"
#include "TextObject.h"
#include "TextureComponent.h"
#include "ResourceManager.h"
#include "TextObject.h"

unsigned int dae::Scene::idCounter = 0;

dae::Scene::Scene(const std::string& name) : mName(name)
{
}

void dae::Scene::Add(const std::shared_ptr<SceneObject>& object)
{
	mObjects.push_back(object);
}

void dae::Scene::Initialize()
{

}

void dae::Scene::Update(float deltaTime)
{
	for(auto gameObject : mObjects)
	{
		gameObject->Update(deltaTime);
	}
}

void dae::Scene::Render() const
{
	for (const auto gameObject : mObjects)
	{
		gameObject->Render();
	}
}

