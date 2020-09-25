/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

FSM.cpp
Simple base class for a Finite State Machine.
*/

#include "FSM.h"

namespace nou
{
	FSM::FSM()
	{
		triggers = std::make_unique<FSM_Map>();
		variables = std::make_unique<FSM_Map>();
	}

	void FSM::ClearTriggers()
	{
		for (auto& [key, value] : *triggers)
		{
			value = false;
		}
	}

	void FSM::SetVariable(const std::string& name, bool value)
	{
		auto result = variables->insert(FSM_Var(name, value));

		if (!result.second)
			result.first->second = value;
	}

	void FSM::SetTrigger(const std::string& name)
	{
		auto result = triggers->insert(FSM_Var(name, true));

		if (!result.second)
			result.first->second = true;
	}

	bool FSM::GetTrigger(const std::string& name)
	{
		auto it = triggers->find(name);

		if (it == triggers->end())
			return false;

		return it->second;
	}

	bool FSM::GetVariable(const std::string& name)
	{
		auto it = variables->find(name);

		if (it == variables->end())
			return false;

		return it->second;
	}
}