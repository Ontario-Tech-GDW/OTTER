/*
NOU Framework - Created for INFR 2310 at Ontario Tech.
(c) Samantha Stahlke 2020

FSM.h
Simple base class for a Finite State Machine.
*/

#pragma once

#include <string>
#include <map>
#include <memory>

namespace nou
{
	class FSM
	{
	public:

		FSM();
		virtual ~FSM() = default;

		FSM(FSM&&) = default;
		FSM& operator=(FSM&&) = default;

		virtual void Update() = 0;

		void ClearTriggers();
		void SetVariable(const std::string& name, bool value);
		void SetTrigger(const std::string& name);

		bool GetTrigger(const std::string& name);
		bool GetVariable(const std::string& name);

	protected:

		typedef std::map<std::string, bool> FSM_Map;
		typedef FSM_Map::value_type FSM_Var;

		//You can really define anything you want in terms of triggers/variables.
		//By convention, we'll say that triggers are set to false whenever the state changes,
		//but variables are stored until their value is changed.
		std::unique_ptr<FSM_Map> triggers;
		std::unique_ptr<FSM_Map> variables;	
	};
}