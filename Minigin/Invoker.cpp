#include "MiniginPCH.h"
#include "Time.h"
#include "Invoker.h"

flgin::FunctionHolderBase::FunctionHolderBase(float invocationTime, void* origin)
	: m_ElapsedTime{ 0.0f }
	, m_InvocationTime{ invocationTime }
	, m_IsRepeating{ false }
	, m_pOrigin{ origin }
{}

bool flgin::FunctionHolderBase::Update()
{
	m_ElapsedTime += Time::GetInstance().GetDeltaTime();
	if (m_ElapsedTime > m_InvocationTime)
	{
		Invoke();
		if (m_IsRepeating)
		{
			m_ElapsedTime -= m_InvocationTime;
			return false;
		}
		return true;
	}
	return false;
}

bool flgin::FunctionHolderBase::DoesOriginateFrom(void* owner)
{
	return owner == m_pOrigin;
}

void flgin::FunctionHolderBase::SetRepeating(bool repeating)
{
	m_IsRepeating = repeating;
}

flgin::Invoker::~Invoker()
{
	Logger& logger{ Logger::GetInstance() };
	for (int i{}; i < m_FunctionsHeld; ++i)
		logger.SafeDelete(m_pFunctionHolders[i]);
}

void flgin::Invoker::Update()
{
	for (int i{}; i < m_FunctionsHeld; ++i)
	{
		if (m_pFunctionHolders[i]->Update())
		{
			--m_FunctionsHeld;
			Logger::GetInstance().SafeDelete(m_pFunctionHolders[i]);
			m_pFunctionHolders[i] = m_pFunctionHolders[m_FunctionsHeld];
			--i;
		}
	}
}

void flgin::Invoker::AddInvoke(FunctionHolderBase * pInvokeToAdd)
{
	if (m_FunctionsHeld == MAX_INVOCATIONS)
	{
		Logger::GetInstance().Log(StatusCode{ StatusCode::Status::FAIL, "Invoker is full, could not add invoke." });
		Logger::GetInstance().SafeDelete(pInvokeToAdd);
		return;
	}
	m_pFunctionHolders[m_FunctionsHeld] = pInvokeToAdd;
	++m_FunctionsHeld;
}

void flgin::Invoker::CancelOwnedInvokes(void* owner)
{
	for (int i{}; i < m_FunctionsHeld; ++i)
	{
		if (m_pFunctionHolders[i]->DoesOriginateFrom(owner))
		{
			--m_FunctionsHeld;
			Logger::GetInstance().SafeDelete(m_pFunctionHolders[i]);
			m_pFunctionHolders[i] = m_pFunctionHolders[m_FunctionsHeld];
			--i;
		}
	}
}

void flgin::Invoker::CancelAllInvokes()
{
	for (int i{}; i < m_FunctionsHeld; ++i)
			Logger::GetInstance().SafeDelete(m_pFunctionHolders[i]);
	m_FunctionsHeld = 0;
}