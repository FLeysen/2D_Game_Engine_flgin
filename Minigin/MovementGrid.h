#pragma once
#include <vector>
#pragma warning(push)
#pragma warning (disable:4201)
#include <glm/vec2.hpp>
#pragma warning(pop)
namespace flgin
{
	class GridNode
	{
	public:
		GridNode();
		GridNode(glm::vec2 pos, const GridNode* leftNode = nullptr, const GridNode* rightNode = nullptr, const GridNode* upNode = nullptr, const GridNode* downNode = nullptr);
		glm::vec2 GetPosition();

	private:
		glm::vec2 m_Position;
		GridNode const* m_pLeftNode;
		GridNode const* m_pRightNode;
		GridNode const* m_pUpNode;
		GridNode const* m_pDownNode;
	};

	class MovementGrid
	{
	public:
		MovementGrid(unsigned int rows, unsigned int cols, float distBetween);
		~MovementGrid();

	private:
		GridNode* m_pNodes;
	};
}