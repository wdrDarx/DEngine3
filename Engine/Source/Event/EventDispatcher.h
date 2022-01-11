#pragma once
#include "Core/Core.h"
#include "Core/Profiling.h"
#include "Utils/VectorUtils.h"
#include "Core/Log.h"

struct Event;

template<typename T>
struct Callback;


class EventDispatcher
{
public:
	
	template<typename T>
	void Dispatch(T& event)
	{
		FORCE_BASE_CLASS(T, Event)
		PROFILE_FUNC()

		//if event is handled then stop
		if (static_cast<Event*>(&event)->m_IsHandled)
			return;

		//using a queue because callbacks could be popped off in the for loop so we need a seperate one
		std::vector<std::function<void(T& event)>> CallbacksToCall;
		for (auto callback : m_BoundCallbacks)
		{		
			//try casting the event, if it works then call the callback			
 			T* castedEvent = dynamic_cast<T*>(callback->m_PlaceholderEvent);

			if (castedEvent)
			{
				//add callback to call queue
				if(callback->m_OnTrigger)
					CallbacksToCall.push_back(callback->m_OnTrigger);
			}
 		}

		//call the queue
		for (auto& callback : CallbacksToCall)
		{
			callback(event);
		}
	}

	template<typename T>
	bool IsBound(Callback<T>& callback)
	{
		return std::find(m_BoundCallbacks.begin(), m_BoundCallbacks.end(), (Callback<Event>*)(&callback)) != m_BoundCallbacks.end();
	}

	template<typename T>
	void Bind(Callback<T>& callback)
	{
		if(callback.m_EventDispatcher && callback.m_EventDispatcher != this)
			LOG_WARN("Callback Already bound to a dispatcher! this might cause a crash!");

		callback.m_EventDispatcher = this;
		Callback<T>* ptr = static_cast<Callback<T>*>(&callback);
		m_BoundCallbacks.push_back((Callback<Event>*)ptr);
	}

	template<typename T>
	void Unbind(Callback<T>& callback)
	{
		if(m_BoundCallbacks.size() < 1) return;

		auto it = std::find(m_BoundCallbacks.begin(), m_BoundCallbacks.end(), (Callback<Event>*)(&callback));
		if(it != m_BoundCallbacks.end())
			m_BoundCallbacks.erase(it);
	}

	void UnbindAll()
	{
		m_BoundCallbacks.clear();
	}

	const std::vector<Callback<Event>*>& GetBoundCallbacks() const
	{
		return m_BoundCallbacks;
	}

protected:
	std::vector<Callback<Event>*> m_BoundCallbacks;
};