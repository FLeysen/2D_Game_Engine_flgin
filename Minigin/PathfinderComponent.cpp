#include "MiniginPCH.h"
#include "PathfinderComponent.h"
#include <algorithm>

flgin::PathfinderComponent::PathfinderComponent(GameObject* const pOwnerObject, float maxVelocity, MovementGrid* pAttachedGrid, float acceptableTurnDist)
	: GridMovementComponent(pOwnerObject, maxVelocity, pAttachedGrid, acceptableTurnDist)
{
}

void flgin::PathfinderComponent::Update()
{
	if (m_IsActive)
	{
		if (m_TargetChanged) FindPath();
		m_TargetChanged = false;
		m_WasStopped = false;

		SetVelocities();
		GridMovementComponent::Update();
	}
}

void flgin::PathfinderComponent::SetTarget(float x, float y)
{
	m_TargetChanged = true;
	m_Target.x = x;
	m_Target.y = y;
}

void flgin::PathfinderComponent::SetActive(bool isActive)
{
	m_IsActive = isActive;
}

void flgin::PathfinderComponent::FindPath()
{
	m_Path.clear();
	std::vector<GridConnection> openList{};
	std::vector<GridConnection> closedList{};
	GridConnection currentConnection{};

	glm::vec2 pos{ m_pOwnerObject->GetPosition() };
	GridNode const* pNearestNode{ m_pGrid->GetNodeNearestTo(pos.x, pos.y) };
	GridNode const* pNodeNearestToEnd{ m_pGrid->GetNodeNearestTo(m_Target.x, m_Target.y) };
	GridNode const* pTempNode{ nullptr };

	if (pNearestNode->GetPosition() != pos)
	{
		GridNode const* pNearestOtherNode{ nullptr };

		if (pNearestNode->GetPosition().x > pos.x)
		{
			pNearestOtherNode = pNearestNode->GetLeftNode();
			pTempNode = new GridNode{ pos, pNearestOtherNode, pNearestNode };
		}
		else if (pNearestNode->GetPosition().x < pos.x)
		{
			pNearestOtherNode = pNearestNode->GetRightNode();
			pTempNode = new GridNode{ pos, pNearestNode, pNearestOtherNode };
		}

		else if (pNearestNode->GetPosition().y > pos.y)
		{
			pNearestOtherNode = pNearestNode->GetDownNode();
			pTempNode = new GridNode{ pos, nullptr, nullptr, pNearestNode, pNearestOtherNode };
		}
		else
		{
			pNearestOtherNode = pNearestNode->GetUpNode();
			pTempNode = new GridNode{ pos, nullptr, nullptr, pNearestOtherNode, pNearestNode };
		}
		openList.push_back(GridConnection{ pTempNode, pNearestNode });
		openList.push_back(GridConnection{ pTempNode, pNearestOtherNode });
	}
	else
	{
		openList = pNearestNode->GetConnections();
	}

	while (!openList.empty())
	{
		currentConnection = openList[0];
		for (size_t i{ 1 }, listSize{ openList.size() }; i < listSize; ++i)
		{
			if (openList[i].GetWeight() < currentConnection.GetWeight())
				currentConnection = openList[i];
		}

		openList.erase(std::remove(openList.begin(), openList.end(), currentConnection), openList.end());
		closedList.push_back(currentConnection);

		std::vector<GridConnection> currConnections{ currentConnection.GetEndNode()->GetConnections() };
		std::vector<GridConnection>::iterator result{ std::find_if(currConnections.begin(), currConnections.end(),
			[pNodeNearestToEnd](GridConnection& connection) { return connection.GetEndNode() == pNodeNearestToEnd; }) };

		if (result != currConnections.end())
		{
			(*result).SetHeadConnection(&currentConnection);
			currentConnection = *result;
			openList.clear();
			break;
		}

		for (GridConnection& connection : currConnections)
		{
			if (std::find(closedList.cbegin(), closedList.cend(), connection) != closedList.cend()) continue;
			else if (std::find(openList.cbegin(), openList.cend(), connection) != openList.cend()) continue;

			connection.SetHeadConnection(&currentConnection);
			openList.push_back(connection);
		}
	}

	if (pTempNode) pNearestNode = pTempNode;
	while (currentConnection.GetStartNode() != pNearestNode)
	{
		m_Path.push_back(currentConnection.GetEndNode());
		currentConnection = *currentConnection.GetHeadConnection();
	}
	m_Path.push_back(currentConnection.GetEndNode());
	std::reverse(m_Path.begin(), m_Path.end());

	Logger::GetInstance().SafeDelete(pTempNode);
}

void flgin::PathfinderComponent::SetVelocities()
{

}