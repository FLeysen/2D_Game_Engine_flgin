#pragma once
#include <vector>
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)
#include "BaseComponent.h"

namespace flgin
{
	class GridNode
	{
	public:
		GridNode();
		GridNode(glm::vec2 pos, const GridNode* leftNode = nullptr, const GridNode* rightNode = nullptr, const GridNode* upNode = nullptr, const GridNode* downNode = nullptr);
		glm::vec2 GetPosition() const;
		GridNode const* GetLeftNode() const { return m_pLeftNode; }
		GridNode const* GetRightNode() const { return m_pRightNode; }
		GridNode const* GetUpNode() const { return m_pUpNode; }
		GridNode const* GetDownNode() const { return m_pDownNode; }

	private:
		int m_Weight;
		glm::vec2 m_Position;
		GridNode const* m_pLeftNode;
		GridNode const* m_pRightNode;
		GridNode const* m_pUpNode;
		GridNode const* m_pDownNode;
	};

	class MovementGrid : public BaseComponent
	{
	public:
		MovementGrid(GameObject* pOwnerObject, unsigned int rows, unsigned int cols, float distBetween);
		~MovementGrid();
		void Update() override;
		GridNode* GetNodeNearestTo(float x, float y);
		GridNode* GetGrid();
		unsigned int GetGridSize();

	private:
		unsigned int m_Rows;
		unsigned int m_Cols;
		float m_DistBetween;
		GridNode* m_pNodes;
	};
}