#pragma once
#include "Rendering/RenderAPI.h"
#include "Event/Callback.h"
#include "Event/Event.h"
#include "Event/EventDispatcher.h"
#include "Framework/Classes/InputManager.h"

class Window;
struct EventWindowClosed : public Event
{
	Window* window;
};

struct EventWindowResized : public Event
{
	Window* window;
	vec2d NewSize;
};

struct EventWindowCursorEnter : public Event {};
struct EventWindowCursorLeave : public Event {};

//houses a window with a rendering context and input manager specific to this window
class DENGINE_API Window
{
public:
	Window(const std::string& name, int width, int height );

	//call to start rendering a frame
	void StartFrame();

	//call to finish the frame
	void EndFrame();

	void SetVsync(bool vsync)
	{
		m_Vsync = vsync;
		glfwSwapInterval(vsync);
	}

	const Ref<RenderAPI>& GetRenderAPI() const
	{
		return m_RenderAPI;
	}

	void OnWindowResize(GLFWwindow* window, int width, int height);

	void BindOnWindowResized(Callback<EventWindowResized>& callback);
	void BindOnWindowClosed(Callback<EventWindowClosed>& callback);

	bool IsClosed() const
	{
		return m_Closed;
	}

	void SetCurrentContext();

	void ClearCurrentContext();

	bool isContextBound();

	uint GetWidth() const;

	uint GetHeight() const;

	GLFWwindow* GetGlfwWindow() const
	{
		return m_Window;
	}

	InputManager& GetInputManager()
	{
		return m_InputManager;
	}

	//this gives the cursor pos in window space which might not be accurate
	const vec2d& GetLastCursorPos() const
	{
		return m_LastCursorPos;
	}

	void SetLastCursorPos(const vec2d& pos)
	{
		m_LastCursorPos = pos;
	}

	void SetShowCursor(bool show);

	void SetCursorPos(const vec2d& pos);

	void SetUseRawMouseInput(bool use);

	void SetInputMode(const InputMode& mode)
	{
		m_InputMode = mode;
		SetUseRawMouseInput(mode == InputMode::GAME);
		SetShowCursor(mode == InputMode::UI);
	}

	const InputMode& GetInputMode() const
	{
		return m_InputMode;
	}

private:

	//used to get the mouse move delta vector
	vec2d m_LastCursorPos;

	//used to mouse raw input stuff
	InputMode m_InputMode = InputMode::UI;

	//calls window events
	EventDispatcher m_EventDispatcher;

	//main input manager recieveing any window inputs and relaying them
	InputManager m_InputManager;

	GLFWwindow* m_Window;
	Ref<RenderAPI> m_RenderAPI;

	bool m_Vsync = false;
	bool m_Closed = false;
};


