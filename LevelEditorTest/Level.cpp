#include "Level.h"
#include "GameObject.h"
#include "TextureComponent.h"
#include "TypeComponent.h"

Level::Level(float Width, float Height, int rows, int cols)
{
	m_pGrid = new Grid(Width, Height, rows, cols);
}

Level::~Level()
{
	delete m_pGrid;
	m_pGrid = nullptr;
}

std::shared_ptr<dae::GameObject> Level::CreateNewEnemy(float posX, float posY, GameObjectType type)
{
	auto enemyToAdd = std::make_shared<dae::GameObject>();
	enemyToAdd->SetPosition(posX, posY);
	NFE::TextureComponent* textComp = nullptr;
	if (type == GameObjectType::ZenChan)
		textComp = new NFE::TextureComponent{ enemyToAdd.get(),"ZenChan.png" };


	else if (type == GameObjectType::Maita)
		textComp = new NFE::TextureComponent{ enemyToAdd.get(),"Maita.png" };

	textComp->Update(0, 0, 0, 44, 44);
	auto typeComp = new NFE::TypeComponent{ enemyToAdd.get(), type };
	enemyToAdd->AddComponentToVector(textComp);
	enemyToAdd->AddComponentToVector(typeComp);
	m_pEnemies.push_back(enemyToAdd);

	return enemyToAdd;
}

void Level::WriteToFile(BinaryWriter& writer, const char* filename)
{
	m_pGrid->WriteToFile(writer, filename);
	int size = m_pEnemies.size();
	writer.Write(size,filename);
	for (size_t i{}; i < m_pEnemies.size(); ++i)
	{
		m_pEnemies[i]->WriteToFile(writer, filename);
	}


}
