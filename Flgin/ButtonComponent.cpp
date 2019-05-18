#include "FlginPCH.h"
#include "ButtonComponent.h"

flgin::ButtonComponent::ButtonComponent(GameObject* pOwnerObject, FunctionHolderBase* pFunc)
	: BaseComponent(pOwnerObject)
	, m_pFunc{ pFunc }
{}

flgin::ButtonComponent::~ButtonComponent()
{

}