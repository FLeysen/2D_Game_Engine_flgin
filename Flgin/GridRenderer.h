#pragma once
#include "BaseComponent.h"

namespace flgin
{
	class Scene;
	class RenderComponent;
	class MovementGrid;
	class GridRenderer : public BaseComponent
	{
	public:
		GridRenderer(GameObject* pOwnerObject, Scene* pScene, MovementGrid* pGrid);
		~GridRenderer();
		void Update() override;

	private:
		std::vector<RenderComponent*> m_pRenderers;
	};
}