#pragma once
#include <functional>
#include <vector>
#include "Singleton.h"
template<typename returnType, typename... argList>
struct FunctionContainer
{
	std::function<returnType(argList...)> function;
	std::tuple<argList...> arguments;

	explicit FunctionContainer(std::function<returnType(argList...)> func, argList... argumentList) 
		: function{ func }, arguments{ std::tuple<argList...>{argumentList...} } {}
};

class Invoker final : public dae::Singleton<Invoker>
{
public:
	Invoker() = default;
	~Invoker() = default;
	Invoker(const Invoker& other) = delete;
	Invoker(Invoker&& other) = delete;
	Invoker& operator=(const Invoker& other) = delete;
	Invoker& operator=(Invoker&& other) = delete;

	template<typename returnType, typename... argList>
	void Invoke(const FunctionContainer<returnType(argList...)>& function, float timeToCall = 0.0f) {};

private:
	std::vector<Invocation> m_Invokes;
};

//-----------------------------------------------------------------
// DELEGATE CLASS (DELEGATE BASE)
//----------------------------------------------------------------
template<typename ret, typename... args>
class Invocation final
{
public:
	template<std::size_t... Is>
	ret CallFunction(std::function<ret(args...)>& func, const std::tuple<args...>& tuple, std::index_sequence<Is...>)
	{
		return func(std::get<Is>(tuple)...);
	}

	explicit Multicast() {};
	explicit Multicast(std::vector<FunctionContainer<void, args...>> dc)
	{
		for (auto c : dc)
			MulticastDataBase<void, args...>::m_functions.push_back(c);
	}

	void Invoke()
	{
		for (FunctionContainer<void, args...> f : MulticastDataBase<void, args...>::m_functions)
		{
			CallFunction(f.func, f.arguments, std::index_sequence_for<args...>());
		}
	}
};
