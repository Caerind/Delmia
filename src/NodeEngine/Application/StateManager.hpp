#ifndef AH_STATEMANAGER_HPP
#define AH_STATEMANAGER_HPP

#include <functional>
#include <map>
#include <vector>
#include <string>
#include <cassert>
#include <iostream>

#include "State.hpp"
#include "../Utils/String.hpp"

namespace ah
{

class StateManager
{
    public:
		StateManager();

		#ifdef N_DESKTOP_PLATFORM
		template<typename T>
		void registerState();

		template<typename T>
		void pushState();
		#endif

		template<typename T>
		void registerState(std::string const& id);

        void handleEvent(sf::Event const& event);
		void update(sf::Time dt);
		void render(sf::RenderTarget& target, sf::RenderStates states);

		void pushState(std::string const& id);
		void popState();
		void clearStates();

		bool empty() const;
		std::size_t size() const;

	protected:
		enum Action
		{
			Push,
			Pop,
			Clear
		};

		struct PendingChange
		{
			explicit PendingChange(Action action, std::string const& id = "");
			Action action;
			std::string id;
		};

		void applyPendingChanges();
		State::Ptr createState(std::string const& id);

	protected:
        std::string mLastActiveStateType;

		std::vector<State::Ptr> mStates;
		std::vector<PendingChange> mPendingList;

		std::map<std::string, std::function<State::Ptr()>> mFactories;
};

#ifdef N_DESKTOP_PLATFORM
template<typename T>
void StateManager::registerState()
{
    mFactories[NString::type<T>()] = [this]()
    {
        return State::Ptr(new T(*this));
    };
}

template<typename T>
void StateManager::pushState()
{
    pushState(NString::type<T>());
}
#endif

template<typename T>
void StateManager::registerState(std::string const& id)
{
	mFactories[id] = [this]()
	{
		return State::Ptr(new T(*this));
	};
}

} // namespace ah

#endif // AH_STATEMANAGER_HPP
