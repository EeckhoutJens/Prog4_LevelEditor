#include "MiniginPCH.h"
#include "BaseComponent.h"
#include "GameObject.h"


NFE::BaseComponent::BaseComponent(dae::GameObject* parent)
{
	m_pParent = parent;
}

const dae::GameObject& NFE::BaseComponent::getParent() const
{
	return *m_pParent;
}


