#pragma once
#include <functional>
#include "Singleton.h"
#include "Time.h"
#include "Logger.h"

#ifndef MAX_INVOCATIONS
#define MAX_INVOCATIONS 10
#endif

namespace flgin
{
	class FunctionHolderBase
	{
	public:
		FunctionHolderBase(float invocationTime) : m_ElapsedTime{ 0.0f }, m_InvocationTime{ invocationTime }, m_IsRepeating{ false }{}
		virtual ~FunctionHolderBase() = default;
		bool Update()
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
		void SetRepeating(bool repeating) { m_IsRepeating = repeating; }

	protected:
		virtual void Invoke() = 0;

		bool m_IsRepeating;
		float m_ElapsedTime;
		float m_InvocationTime;
	};

	template<typename returnType, typename... argList>
	class FunctionHolder : public FunctionHolderBase
	{
	public:
		explicit FunctionHolder(float invocationTime, std::function<returnType(argList...)> function, argList... arguments)
			: FunctionHolderBase(invocationTime), m_Function{ function }, m_Arguments{ arguments... } {}

	private:
		void Invoke() 
		{
			CallFunction(m_Function, m_Arguments, std::index_sequence_for<argList...>());
		}
		template<std::size_t... Is>
		returnType CallFunction(std::function<returnType(argList...)>& func, const std::tuple<argList...>& tuple, std::index_sequence<Is...>)
		{
			return func(std::get<Is>(tuple)...);
		}

		std::function<returnType(argList...)> m_Function;
		std::tuple<argList...> m_Arguments;
	};

	class Invoker final : public Singleton<Invoker>
	{
	public:
		Invoker() = default;
		~Invoker() 
		{ 
			Logger& logger{ Logger::GetInstance() };
			for (int i{}; i < m_FunctionsHeld; ++i)
				logger.SafeDelete(m_pFunctionHolders[i]);
		};
		Invoker(const Invoker& other) = delete;
		Invoker(Invoker&& other) = delete;
		Invoker& operator=(const Invoker& other) = delete;
		Invoker& operator=(Invoker&& other) = delete;

		void Update()
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
		void AddInvoke(FunctionHolderBase* pInvokeToAdd)
		{
			if (m_FunctionsHeld == MAX_INVOCATIONS)
			{
				Logger::GetInstance().Log(StatusCode{StatusCode::Status::FAIL, "Invoker is full, could not add invoke."});
				Logger::GetInstance().SafeDelete(pInvokeToAdd);
				return;
			}
			m_pFunctionHolders[m_FunctionsHeld] = pInvokeToAdd;
			++m_FunctionsHeld;
		}

		void CancelInvoke(FunctionHolderBase*)
		{
		}

	private:
		FunctionHolderBase* m_pFunctionHolders[MAX_INVOCATIONS]{};
		int m_FunctionsHeld{ 0 };
	};
}