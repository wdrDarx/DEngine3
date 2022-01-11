#pragma once
#include "Core/Core.h"

//inheritable class to represent an event
struct Event
{
public:

	virtual ~Event()
	{

	}

	void MarkHandled()
	{
		m_IsHandled = true;
	}

	//a receiver can mark this to prevent the rest of recievers from getting the event
	bool m_IsHandled = false;
};