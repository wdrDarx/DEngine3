#pragma once
#include "Core/Core.h"

//Simple thread system
class Thread
{
public:
	static inline uint MaxThreads()
	{
		return std::thread::hardware_concurrency();
	}

	Thread()
	{
		Start();
	}

	virtual ~Thread()
	{
		Shutdown();
	}

	void Start()
	{
		m_Thread = std::thread(&Thread::Poll, this);
	}

	void Poll()
	{
		{
			std::unique_lock<std::mutex> lock(m_Mutex);

			m_Condition.wait(lock, [this]()
			{
				return !m_Jobs.empty() && !m_Stop;
			});

			m_Job = m_Jobs.front();
			m_Jobs.erase(m_Jobs.begin());
		}

		m_Job(); // function<void()> type
	}

	void Execute(std::function<void()> New_Job)
	{
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_Jobs.push_back(New_Job);
		}

		m_Condition.notify_one();
	}

	template <typename Func, typename... Args>
	void ExecuteParams(Func f, Args... args)
	{
		Execute(std::bind(f, args...));
	}

	void Wait()
	{
		while (!m_Jobs.empty() && !m_Stop)
		{
			Poll();
		}
	}

	void Shutdown()
	{
		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			m_Stop = true;
		}

		m_Condition.notify_all();
		m_Thread.join();
	}

	std::thread m_Thread;
	std::mutex m_Mutex;
	std::condition_variable m_Condition;
	bool m_Stop = false;
	std::function<void()> m_Job;
	std::vector<std::function<void()>> m_Jobs;
};

//same as above but have to Poll() manually on tick (used as a "Main Thread")
class VirtualThread
{
public:

	VirtualThread()
	{
		
	}

	virtual ~VirtualThread()
	{
		Shutdown();
	}

	void Start()
	{
		
	}

	void Poll()
	{
		{
			std::unique_lock<std::mutex> lock(m_Mutex);

			if(!m_Jobs.empty() && !m_Stop)
			{ 
				m_Job = m_Jobs.front();
				m_Jobs.erase(m_Jobs.begin());
			}
		}

		if(m_Job)
		{ 
			m_Job(); // function<void()> type
			m_Job = nullptr;
		}
	}

	void Execute(std::function<void()> New_Job)
	{
		{
			std::unique_lock lock(m_Mutex);
			m_Jobs.push_back(New_Job);
		}
	}

	template <typename Func, typename... Args>
	void ExecuteParams(Func f, Args... args)
	{
		Execute(std::bind(f, args...));
	}

	void Wait()
	{
		while (!m_Jobs.empty() && !m_Stop)
		{
			Poll();
		}
	}

	void Shutdown()
	{
		m_Stop = true;
	}

	bool m_Stop = false;
	std::function<void()> m_Job;
	std::mutex m_Mutex;
	std::vector<std::function<void()>> m_Jobs;
};