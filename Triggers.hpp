#pragma once

#include <map>
#include <string>
#include <vector>
#include <iostream>
#include <kaguya/kaguya.hpp>

#include "TimeManager.hpp"

class Trigger
{
	private:
		std::string group;
		std::string triggerName;
		std::string triggerNamespace;
		kaguya::LuaTable* triggerParameters;
		bool enabled = false;
		bool toEnable = false;
		bool toDisable = false;
		bool permanent;
		void clearParameters();
		template <typename P>
		void pushParameter(std::string name, P parameter);
		friend class TriggerGroup;
		friend class TriggerDatabase;
	public:
		Trigger(std::string group, std::string triggerName, bool startState = false, bool permanent = false);
		Trigger(std::string nsp, std::string group, std::string triggerName, bool startState = false, bool permanent = false);
		bool getState();
		bool isPermanent();
		std::string getGroup();
		std::string getName();
		std::string getNamespace();
		kaguya::LuaTable* getParameters();
};

class TriggerDelay
{
	public:
		TriggerDelay(Trigger* trg, int del, bool sta);
		Trigger* trigger;
		int delay;
		bool state;
		int delaytarget;
};

class TriggerGroup
{
	private:
		std::string triggerGroupName;
		std::string fromNsp;
		std::map<std::string, Trigger*> triggerMap;
		std::vector<TriggerDelay*> delayedTriggers;
		friend class TriggerDatabase;
		
	public:
		TriggerGroup(std::string triggerGroupName);
		TriggerGroup(std::string triggerGroupNamespace, std::string triggerGroupName);
		Trigger* getTrigger(std::string triggerName);
		TriggerGroup* addTrigger(std::string triggerName);
		void delayTriggerState(std::string triggerName, int delay, bool state);
		void enableTrigger(std::string triggerName);
		void disableTrigger(std::string triggerName);
		void setTriggerState(std::string triggerName, bool state);
		bool getState(std::string triggerName);
		void setPermanent(std::string triggerName, bool permanent);
		template <typename P>
		void pushParameter(std::string triggerName, std::string parameterName, P parameter);
		std::vector<std::string> getAllTriggersName();
		std::vector<Trigger*> getAllTriggers();
		std::string getNamespace();
};

class TriggerDatabase
{
	private:
		std::map<std::string, Trigger*> coreTriggers;
		std::map<std::string, std::map<std::string, TriggerGroup*>> customTriggers;
		std::vector<std::string> toEnable;
		std::vector<std::string> toDisable;
		Chronometer databaseChrono;
		std::vector<TriggerDelay*> delayedTriggers;
	public:
		TriggerDatabase();
		Trigger* getTrigger(std::string triggerName);
		Trigger* getCustomTrigger(std::string groupNamespace, std::string triggerGroupName, std::string triggerName);
		void setTriggerState(std::string name, bool state);
		void createCustomNamespace(std::string groupNamespace);
		TriggerGroup* createTriggerGroup(std::string groupNamespace, std::string triggerGroupName);
		std::vector<std::string> getAllTriggersNameFromCustomGroup(std::string groupNamespace, std::string triggerGroupName);
		void update();
};

extern TriggerDatabase triggerDatabaseCore;

template<typename P>
inline void Trigger::pushParameter(std::string name, P parameter)
{
	(*triggerParameters)[name] = &parameter;
	std::cout << "Didn't word.." << std::endl;
}

template<typename P>
inline void TriggerGroup::pushParameter(std::string triggerName, std::string parameterName, P parameter)
{
	getTrigger(triggerName)->pushParameter(parameterName, parameter);
}
