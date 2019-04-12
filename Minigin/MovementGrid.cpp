#include "MiniginPCH.h"
#include "MovementGrid.h"
#include "GameObject.h"

flgin::MovementGrid::MovementGrid(GameObject* pOwnerObject, unsigned int rows, unsigned int cols, float distBetween)
	: BaseComponent(pOwnerObject)
	, m_pNodes{ new GridNode[rows * cols]{} }
	, m_Rows{ rows }
	, m_Cols{ cols }
	, m_DistBetween{ distBetween }
{
	unsigned int nodeId{ 0 };
	glm::vec2 position{ distBetween / 2, distBetween / 2 };
	GridNode* leftNode{ nullptr };
	GridNode* rightNode{ nullptr };
	GridNode* upNode{ nullptr };
	GridNode* downNode{ nullptr };

	for (unsigned int row{}; row < rows; ++row)
	{
		for (unsigned int col{}; col < cols; ++col)
		{
			nodeId = row * cols + col;

			if (col > 0) leftNode = &m_pNodes[nodeId - 1];
			else leftNode = nullptr;

			if (col < cols - 1) rightNode = &m_pNodes[nodeId + 1];
			else rightNode = nullptr;

			if (row > 0) upNode = &m_pNodes[nodeId - cols];
			else upNode = nullptr;

			if (row < rows - 1) downNode = &m_pNodes[nodeId + cols];
			else downNode = nullptr;

			m_pNodes[nodeId] = GridNode{ position, leftNode, rightNode, upNode, downNode };

			position.x += distBetween;
		}
		position.x = distBetween / 2;
		position.y += distBetween;
	}
}

flgin::GridNode* flgin::MovementGrid::GetNodeNearestTo(float x, float y)
{
	if (!m_pNodes) return nullptr;

	float xSteps{ (x - m_DistBetween / 2) / m_DistBetween };
	float ySteps{ (y - m_DistBetween / 2) / m_DistBetween };
	int col{ static_cast<int>(round(xSteps)) };
	int row{ static_cast<int>(round(ySteps)) };

	if (col < 0) col = 0;
	else if (col >= static_cast<int>(m_Cols)) col = m_Cols - 1;
	if (row < 0) row = 0;
	else if (row >= static_cast<int>(m_Rows)) row = m_Rows - 1;

	return &m_pNodes[col + row * m_Cols];
}

flgin::GridNode* flgin::MovementGrid::GetGrid()
{
	return m_pNodes;
}

unsigned int flgin::MovementGrid::GetGridSize()
{
	return m_Rows * m_Cols;
}

flgin::MovementGrid::~MovementGrid()
{
	Logger::GetInstance().SafeDeleteArray(m_pNodes);
}

void flgin::MovementGrid::Update()
{}

flgin::GridNode::GridNode()
	: m_pLeftNode{}
	, m_pRightNode{}
	, m_pDownNode{}
	, m_pUpNode{}
	, m_Position{}
	, m_Weight{}
{}

flgin::GridNode::GridNode(glm::vec2 pos, const GridNode* leftNode, const GridNode* rightNode, const GridNode* upNode, const GridNode* downNode)
	: m_pLeftNode{ leftNode }
	, m_pRightNode{ rightNode }
	, m_pUpNode{ upNode }
	, m_pDownNode{ downNode }
	, m_Position{ pos }
	, m_Weight{ 10 }
{}

glm::vec2 flgin::GridNode::GetPosition() const
{
	return m_Position;
}
