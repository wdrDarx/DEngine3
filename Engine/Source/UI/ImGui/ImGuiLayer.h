#pragma once

#define IMGUI_OPENGL_LOADER_GLEW
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "misc/cpp/imgui_stdlib.h"
#include "Core/Core.h"


class Window;
class ImGuiLayer
{	
public:	
	
	~ImGuiLayer();

	void Init(Window* window);
	void Begin();
	void End();
	void Shutdown();

	//used to tell if calling imgui funcs is safe
	bool IsValid() const
	{
		return m_Valid;
	}

	void SetDarkTheme();


private:
	ImGuiContext* m_ImGuidContext;
	Window* m_Window;

	//used to tell if calling imgui funcs is safe
	bool m_Valid = false;
};

