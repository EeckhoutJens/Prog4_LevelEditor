#pragma once
#include "Singleton.h"
#include "vector"
#include <memory>
namespace dae
{
	enum class SceneType
	{
		Standard,
		PengoLevel,
		MainMenu,
		GameOverMenu,
	};
	class Scene;
	class SceneManager final : public Singleton<SceneManager>
	{
	public:
		std::shared_ptr<Scene> CreateScene(const std::string& name, SceneType type);

		void Update(float deltaTime);
		void Render();

	private:
		std::vector<std::shared_ptr<Scene>> mScenes;
	};

}
