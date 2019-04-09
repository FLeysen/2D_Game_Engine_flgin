#include "MiniginPCH.h"
#include "MovementGrid.h"

flgin::MovementGrid::MovementGrid(unsigned int rows, unsigned int cols, float distBetween)
	: m_pNodes{ new GridNode[rows * cols] }
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

flgin::MovementGrid::~MovementGrid()
{
	Logger::GetInstance().SafeDelete(m_pNodes);
}

flgin::GridNode::GridNode()
	: m_pLeftNode{}
	, m_pRightNode{}
	, m_pDownNode{}
	, m_pUpNode{}
	, m_Position{}
{}

flgin::GridNode::GridNode(glm::vec2 pos, const GridNode* leftNode, const GridNode* rightNode, const GridNode* upNode, const GridNode* downNode)
	: m_pLeftNode{ leftNode }
	, m_pRightNode{ rightNode }
	, m_pUpNode{ upNode }
	, m_pDownNode{ downNode }
	, m_Position{ pos }
{}

glm::vec2 flgin::GridNode::GetPosition()
{
	return m_Position;
}
