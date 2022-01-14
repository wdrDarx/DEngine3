#pragma once
#include "Core/Core.h"
#include "Event/EventDispatcher.h"
#include "Event/Callback.h"
#include "Event/Event.h"

enum class MouseEventType
{
	BUTTON = 0,
	SCROLL,
	MOVED
};

enum class ScrollDir
{
	UP = 0,
	DOWN
};

enum class KeyState
{
	DOWN = 0,
	UP,
	CLICKED
};

struct EventMouseButton : public Event { int KeyCode; KeyState keyState; };
struct EventMouseScroll : public Event { ScrollDir scrollDir; };
struct EventMouseMove: public Event { vec2d MoveVector; };
struct EventKey : public Event { int KeyCode; KeyState keyState; };

struct GLFWwindow;
/*
	class for Input events. Can forward input events to other managers.
	One main manager needs to be tied to a window through the base private input calls
	that main manager then should relay all events to other managers (default framework relays events to the focused scene)
*/
class DENGINE_API InputManager
{
	public:
	friend class Window;

	InputManager();
	void ForwardTo(InputManager& other);
	void StopForwarding(InputManager& other);

	void BindOnKeyDown(Callback<EventKey>& callback);
	void BindOnKeyUp(Callback<EventKey>& callback);

	void BindOnMouseMove(Callback<EventMouseMove>& callback);
	void BindOnMouseScroll(Callback<EventMouseScroll>& callback);
	void BindOnMouseDown(Callback<EventMouseButton>& callback);
	void BindOnMouseUp(Callback<EventMouseButton>& callback);

	bool IsKeyDown(int KeyCode);
	void ClearInput();

	const vec2d& GetMouseDelta() const
	{
		return m_MouseDelta;
	}

	EventDispatcher& GetInternalDispatcher()
	{
		return m_InternalDispatcher;
	}

	public:
		void GlfwKey(GLFWwindow* window, int key, int scancode, int action, int mods);
		void GlfwMouseKey(GLFWwindow* window, int key, int scancode, int action, int mods);
		void GlfwMouseMove(GLFWwindow* window, const vec2d& vector);
		void GlfwMouseScroll(GLFWwindow* window, const ScrollDir& dir);

		std::vector<int> m_HeldKeys;
		vec2d m_MouseDelta = {0,0};

		EventDispatcher m_InternalDispatcher;

		Callback<EventKey> m_InternalKeyEvent;
		Callback<EventMouseButton> m_InternalMouseButtonEvent;
		Callback<EventMouseScroll> m_InternalMouseScrollEvent;
		Callback<EventMouseMove> m_InternalMouseMoveEvent;

		EventDispatcher m_KeyDownDispatchcer;
		EventDispatcher m_KeyUpDispatchcer;

		EventDispatcher m_MouseMoveDispatcher;
		EventDispatcher m_MouseScrollDispatcher;
		EventDispatcher m_MouseDownDispatcher;
		EventDispatcher m_MouseUpDispatcher;

};