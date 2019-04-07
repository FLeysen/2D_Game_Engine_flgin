#pragma once
class Command
{
public:
	Command(WORD mappedButton) : m_MappedButton{ mappedButton } {}
	virtual ~Command() = default;
	virtual void Execute() = 0;
	void SetMapping(WORD newMapping) { m_MappedButton = newMapping; }
	WORD GetMapping() { return m_MappedButton; }

private:
	WORD m_MappedButton;
};