#include "Window.h"
#include "Core/Profiling.h"

Window::Window(const std::string& name, int width, int height)
{
	PROFILE_FUNC()

	glfwInit();
	m_Window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);

	//create render api
	m_RenderAPI = MakeRef<RenderAPI>(m_Window);
	SetCurrentContext();

	//set viewport
	GetRenderAPI()->SetViewport({width, height});

	//point to this window
	glfwSetWindowUserPointer(GetRenderAPI()->GetCurrentContext(), (void*)this);

	//bind resize callback
	glfwSetWindowSizeCallback(GetRenderAPI()->GetCurrentContext(), [](GLFWwindow* window, int width, int height)
	{
		StaticCast<Window>(glfwGetWindowUserPointer(window))->OnWindowResize(window, width, height);
	});

	//bind key input callback to the input manager
	glfwSetKeyCallback(GetRenderAPI()->GetCurrentContext(), [](GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		//StaticCast<Window>(glfwGetWindowUserPointer(window))->GetInputManager().GlfwKey(window, key, scancode, action, mods);
	});	

	//bind on cursor enter / leave
	glfwSetCursorEnterCallback(GetRenderAPI()->GetCurrentContext(), [](GLFWwindow* window, int entered)
	{
		//WindowEvent event;
		//event.m_EventType = entered ? WindowEventType::CURSORENTER : WindowEventType::CURSORLEAVE;
		//StaticCast<Window>(glfwGetWindowUserPointer(window))->GetEventDispatcher().Dispatch(event);
	});

	//bind on mouse button
	glfwSetMouseButtonCallback(GetRenderAPI()->GetCurrentContext(), [](GLFWwindow * window, int button, int action, int mods)
	{
		//StaticCast<Window>(glfwGetWindowUserPointer(window))->GetInputManager().GlfwMouseKey(window, button, 0, action, mods);
	});

	//bind on mouse scroll
	glfwSetScrollCallback(GetRenderAPI()->GetCurrentContext(), [](GLFWwindow * window, double xoffset, double yoffset)
	{
		//StaticCast<Window>(glfwGetWindowUserPointer(window))->GetInputManager().GlfwMouseScroll(window,  yoffset > 0.f ? ScrollDir::UP : ScrollDir::DOWN);
	});

	//bind on cursor move
	glfwSetCursorPosCallback(GetRenderAPI()->GetCurrentContext(), [](GLFWwindow* window, double xpos, double ypos)
	{
// 		auto This = StaticCast<Window>(glfwGetWindowUserPointer(window));
// 		vec2d newPos = { xpos, This->GetHeight() - ypos }; //flip y axis
// 		vec2d LastPos = This->GetLastCursorPos();
// 
// 		vec2d DeltaPos = (newPos - LastPos);
// 
// 		//keep cursor in the middle if input mode is game
// 		if (This->GetInputMode() == InputMode::GAME)
// 		{
// 			vec2d ForcedPos = This->GetRenderAPI()->GetWindowSize();
// 			ForcedPos.x /= 2.f;
// 			ForcedPos.y /= 2.f;
// 
// 			This->SetCursorPos(ForcedPos);
// 			This->SetLastCursorPos(ForcedPos);
// 		}
// 		else
// 		{ 
// 			This->SetLastCursorPos(newPos);
// 		}
// 
// 		StaticCast<Window>(glfwGetWindowUserPointer(window))->GetInputManager().GlfwMouseMove(window, DeltaPos);		
	});
}

void Window::StartFrame()
{
	PROFILE_FUNC()

	if (m_Closed)
		return;

	//bind context if not bound
	if (!GetRenderAPI()->IsContextBound())
	{
		SetCurrentContext();
	}

	if (glfwWindowShouldClose(GetRenderAPI()->GetCurrentContext()))
	{
 		EventWindowClosed event;
		event.window = this;
  		m_EventDispatcher.Dispatch(event);

		glfwDestroyWindow(GetRenderAPI()->GetCurrentContext());
		m_Closed = true;
		ClearCurrentContext();
		return;
	}

	GetRenderAPI()->SetClearColor({0,0,0,0});
	GetRenderAPI()->Clear();
}

void Window::EndFrame()
{
	PROFILE_FUNC()

	if(m_Closed)
		return;

	//bind context if not bound
	if (!GetRenderAPI()->IsContextBound())
	{
		SetCurrentContext();
	}

	glfwSwapBuffers(GetRenderAPI()->GetCurrentContext());
	glfwPollEvents();
}

void Window::OnWindowResize(GLFWwindow* window, int width, int height)
{
	PROFILE_FUNC()

	//dispatch resize event
 	EventWindowResized event;
	event.NewSize = { width, height };
	event.window = this;
 	m_EventDispatcher.Dispatch(event);

	GetRenderAPI()->SetViewport({width, height});
}


void Window::BindOnWindowResized(Callback<EventWindowResized>& callback)
{
	m_EventDispatcher.Bind(callback);
}

void Window::BindOnWindowClosed(Callback<EventWindowClosed>& callback)
{
	m_EventDispatcher.Bind(callback);
}

void Window::SetCurrentContext()
{
	GetRenderAPI()->SetCurrentContext(m_Window);
}

void Window::ClearCurrentContext()
{
	GetRenderAPI()->ClearCurrentContext();
}

bool Window::isContextBound()
{
	return GetRenderAPI()->IsContextBound();
}

uint Window::GetWidth() const
{
	return GetRenderAPI()->GetWindowSize().x;
}

uint Window::GetHeight() const
{
	return GetRenderAPI()->GetWindowSize().y;
}

void Window::SetShowCursor(bool show)
{
	//InputUtils::SetShowCursor(GetRenderAPI(), show);
}

void Window::SetCursorPos(const vec2d& pos)
{
	//InputUtils::SetCursorPos(GetRenderAPI(), pos);
}

void Window::SetUseRawMouseInput(bool use)
{
	//InputUtils::SetUseRawMouseInput(GetRenderAPI(), use);
}
