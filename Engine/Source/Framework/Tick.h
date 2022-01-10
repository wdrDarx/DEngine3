#pragma once
#include "Core/Core.h"

enum  TickGroup
{
	GAME = BIT(0),
	EDITOR = BIT(1),
	PAUSED = BIT(2)
};

struct Tick
{
	public:
		Tick(const TickGroup& tg, float dt) : m_TickGroup(tg), DeltaTime(dt)
		{

		}

		Tick()
		{

		}

		const inline TickGroup& GetTickGroup() const
		{
			return m_TickGroup;
		}

		float DeltaTime;
	private:
		TickGroup m_TickGroup;
};