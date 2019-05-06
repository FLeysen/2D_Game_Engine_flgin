#include "FlginPCH.h"
#include "GridRenderer.h"
#include "MovementGrid.h"
#include "RenderComponent.h"
#include "ResourceManager.h"

flgin::GridRenderer::GridRenderer(GameObject* pOwnerObject, Scene& scene, MovementGrid* pGrid)
	: BaseComponent(pOwnerObject)
{
	m_pRenderers.resize(pGrid->GetGridSize());
	GridNode* pGridNodes{ pGrid->GetGrid() };
	std::shared_ptr<Texture2D> spTex{ ResourceManager::GetInstance().LoadTexture("backGroundSprite.png") };
	glm::vec2 negativeOffset{ pGrid->GetGameObject()->GetPosition() };
	negativeOffset += pGrid->GetNodeSize() / 2;
	for (size_t i{}, size{ m_pRenderers.size() }; i < size; ++i)
	{
		m_pRenderers[i] = new RenderComponent{ pOwnerObject, scene };
		m_pRenderers[i]->SetTexture(spTex);
		m_pRenderers[i]->SetPositionOffset(pGridNodes[i].GetPosition().x - negativeOffset.x, pGridNodes[i].GetPosition().y - negativeOffset.y);
	}
}

flgin::GridRenderer::~GridRenderer()
{
	Logger& logger{ FLogger };
	for (RenderComponent* renderComponent : m_pRenderers)
		logger.SafeDelete(renderComponent);
}

void flgin::GridRenderer::Update()
{}
