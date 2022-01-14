#include "InputManager.h"
#include "DEngine.h"

InputManager::InputManager()
{
	m_InternalKeyEvent.Assign([&](EventKey& event)
	{
		if(event.keyState == KeyState::DOWN)
		{ 
			m_KeyDownDispatchcer.Dispatch(event);

			//add to held keys array if not already there
			if(std::count(m_HeldKeys.begin(), m_HeldKeys.end(), event.KeyCode) < 1)
				m_HeldKeys.push_back(event.KeyCode);
		}
		else
		if(event.keyState == KeyState::UP)
		{ 
			m_KeyUpDispatchcer.Dispatch(event);

			//remove from held keys array if its there
			auto it = std::find(m_HeldKeys.begin(), m_HeldKeys.end(), event.KeyCode);
			if(it != m_HeldKeys.end())
				m_HeldKeys.erase(it);
		}
	});
	GetInternalDispatcher().Bind(m_InternalKeyEvent);

	m_InternalMouseButtonEvent.Assign([&](EventMouseButton& event)
	{

		if (event.keyState == KeyState::DOWN)
		{
			m_MouseDownDispatcher.Dispatch(event);

			//add to held keys array if not already there
			if (std::count(m_HeldKeys.begin(), m_HeldKeys.end(), event.KeyCode) < 1)
				m_HeldKeys.push_back(event.KeyCode);
		}
		else
			if (event.keyState == KeyState::UP)
			{
				m_MouseUpDispatcher.Dispatch(event);

				//remove from held keys array if its there
				auto it = std::find(m_HeldKeys.begin(), m_HeldKeys.end(), event.KeyCode);
				if (it != m_HeldKeys.end())
					m_HeldKeys.erase(it);
			}
	});
	GetInternalDispatcher().Bind(m_InternalMouseButtonEvent);

	m_InternalMouseMoveEvent.Assign([&](EventMouseMove& event)
	{
		m_MouseMoveDispatcher.Dispatch(event);
	});
	GetInternalDispatcher().Bind(m_InternalMouseMoveEvent);

	m_InternalMouseScrollEvent.Assign([&](EventMouseScroll& event)
	{
		m_MouseScrollDispatcher.Dispatch(event);
	});
	GetInternalDispatcher().Bind(m_InternalMouseScrollEvent);
}

void InputManager::ForwardTo(InputManager& other)
{
	if(!GetInternalDispatcher().IsBound(other.m_InternalKeyEvent))
		GetInternalDispatcher().Bind(other.m_InternalKeyEvent);

	if (!GetInternalDispatcher().IsBound(other.m_InternalMouseButtonEvent))
		GetInternalDispatcher().Bind(other.m_InternalMouseButtonEvent);

	if (!GetInternalDispatcher().IsBound(other.m_InternalMouseMoveEvent))
		GetInternalDispatcher().Bind(other.m_InternalMouseMoveEvent);

	if (!GetInternalDispatcher().IsBound(other.m_InternalMouseScrollEvent))
		GetInternalDispatcher().Bind(other.m_InternalMouseScrollEvent);
}

void InputManager::StopForwarding(InputManager& other)
{
	if (GetInternalDispatcher().IsBound(other.m_InternalKeyEvent))
	{ 
		GetInternalDispatcher().Unbind(other.m_InternalKeyEvent);
	}

	if (GetInternalDispatcher().IsBound(other.m_InternalMouseButtonEvent))
	{
		GetInternalDispatcher().Unbind(other.m_InternalMouseButtonEvent);
	}

	if (GetInternalDispatcher().IsBound(other.m_InternalMouseMoveEvent))
	{
		GetInternalDispatcher().Unbind(other.m_InternalMouseMoveEvent);
	}

	if (GetInternalDispatcher().IsBound(other.m_InternalMouseScrollEvent))
	{
		GetInternalDispatcher().Unbind(other.m_InternalMouseScrollEvent);
	}
}

void InputManager::BindOnKeyDown(Callback<EventKey>& callback)
{
	m_KeyDownDispatchcer.Bind(callback);
}

void InputManager::BindOnKeyUp(Callback<EventKey>& callback)
{
	m_KeyUpDispatchcer.Bind(callback);
}

void InputManager::BindOnMouseMove(Callback<EventMouseMove>& callback)
{
	m_MouseMoveDispatcher.Bind(callback);
}

void InputManager::BindOnMouseScroll(Callback<EventMouseScroll>& callback)
{
	m_MouseScrollDispatcher.Bind(callback);
}

void InputManager::BindOnMouseDown(Callback<EventMouseButton>& callback)
{
	m_MouseDownDispatcher.Bind(callback);
}

void InputManager::BindOnMouseUp(Callback<EventMouseButton>& callback)
{
	m_MouseUpDispatcher.Bind(callback);
}

bool InputManager::IsKeyDown(int KeyCode)
{
	return std::find(m_HeldKeys.begin(), m_HeldKeys.end(), KeyCode) != m_HeldKeys.end();
}

void InputManager::ClearInput()
{
	m_HeldKeys.clear();
}

void InputManager::GlfwKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	EventKey event;
	event.KeyCode = key;
	switch (action)
	{
		case GLFW_PRESS:
		{
			event.keyState = KeyState::DOWN;
			break;
		}

		case GLFW_RELEASE:
		{
			event.keyState = KeyState::UP;
			break;
		}
	}

	GetInternalDispatcher().Dispatch(event);
}

void InputManager::GlfwMouseKey(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	EventMouseButton event;
	event.KeyCode = key;
	switch (action)
	{
		case GLFW_PRESS:
		{
			event.keyState = KeyState::DOWN;
			break;
		}

		case GLFW_RELEASE:
		{
			event.keyState = KeyState::UP;
			break;
		}
	}

	GetInternalDispatcher().Dispatch(event);
}

void InputManager::GlfwMouseMove(GLFWwindow* window, const vec2d& vector)
{
	EventMouseMove event;
	event.MoveVector = vector;
	
	//assign the last move delta
	m_MouseDelta = vector;
	//LogTemp("Mouse Delta : " + Log::string(m_MouseDelta));
	GetInternalDispatcher().Dispatch(event);
}

void InputManager::GlfwMouseScroll(GLFWwindow* window, const ScrollDir& dir)
{
	EventMouseScroll event;
	event.scrollDir = dir;

	GetInternalDispatcher().Dispatch(event);
}

