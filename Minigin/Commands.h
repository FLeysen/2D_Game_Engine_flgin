#pragma once
#include "Command.h"
#include <iostream>
#include "AudioLocator.h"

class FireCommand final : public Command
{
public:
	FireCommand(WORD mappedButton) : Command(mappedButton) {}
	void Execute() override { std::cout << "Fire command executed.\n"; AudioLocator::GetAudio()->playSound(330, 100); }
};

class DuckCommand final : public Command
{
public:
	DuckCommand(WORD mappedButton) : Command(mappedButton) {}
	void Execute() override { std::cout << "Duck command executed.\n"; AudioLocator::GetAudio()->playSound(262, 100); }
};

class JumpCommand final : public Command
{
public:
	JumpCommand(WORD mappedButton) : Command(mappedButton) {}
	void Execute() override { std::cout << "Jump command executed.\n"; AudioLocator::GetAudio()->playSound(392, 100); }
};

class FartCommand final : public Command
{
public:
	FartCommand(WORD mappedButton) : Command(mappedButton) {}
	void Execute() override { std::cout << "Fart command executed.\n"; AudioLocator::GetAudio()->playSound(196, 100); }
};