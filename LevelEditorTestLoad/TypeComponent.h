#pragma once
#include "BaseComponent.h"

enum class GameObjectType
{
	Airblock,
	Platformblock,
	Wallblock,
	ZenChan,
	Maita
};
namespace NFE
{
	class TypeComponent :
	public BaseComponent
	{
	public:
		TypeComponent(dae::GameObject *parent, GameObjectType type);
		~TypeComponent() = default;
		GameObjectType GetType();
		void SetType(GameObjectType newType);
		void Update(const float&, float, float, float, float) override {;};
		void Render() override{;};
		void WriteToFile(BinaryWriter& writer, const char* filename) override;
	private:
		GameObjectType m_Type;
	};
}


