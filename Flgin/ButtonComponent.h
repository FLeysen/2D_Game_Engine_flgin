#pragma once
#include "BaseComponent.h"

namespace flgin
{
	class FunctionHolderBase;
	class RenderComponent;

	class ButtonComponent final : public BaseComponent
	{
	public:
		ButtonComponent(GameObject* pOwnerObject, FunctionHolderBase* pFunc);
		~ButtonComponent();
		void Update() override {};

	private:
		FunctionHolderBase* m_pFunc;
	};
}
