#include "MiniginPCH.h"
#include "GridRenderer.h"
#include "MovementGrid.h"
#include "RenderComponent.h"
#include "ResourceManager.h"

flgin::GridRenderer::GridRenderer(GameObject* pOwnerObject, Scene& scene, MovementGrid* pGrid)
	: BaseComponent(pOwnerObject)
{
	m_pRenderers.resize(pGrid->GetGridSize());
	GridNode* pGridNodes{ pGrid->GetGrid() };
	for (size_t i{}, size{ m_pRenderers.size() }; i < size; ++i)
	{
		m_pRenderers[i] = new RenderComponent{ pOwnerObject, scene };
		m_pRenderers[i]->SetTexture(ResourceManager::GetInstance().LoadTexture("grid.png"));
		m_pRenderers[i]->SetPositionOffset(pGridNodes[i].GetPosition().x, pGridNodes[i].GetPosition().y);
	}
}

flgin::GridRenderer::~GridRenderer()
{
	Logger& logger{ Logger::GetInstance() };
	for (RenderComponent* renderComponent : m_pRenderers)
		logger.SafeDelete(renderComponent);
}

void flgin::GridRenderer::Update()
{}
