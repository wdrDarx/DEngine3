#include "ImGuiLayer.h"
#include "Rendering/Window.h"
//#include "Rendering/Core/Renderer.h"
#include "Utils/Paths.h"
#include "Core/Core.h"
#include "GLFW/glfw3.h"
//#include "ImGuizmo/ImGuizmo.h"

ImGuiLayer::~ImGuiLayer()
{	
	Shutdown();
}

void ImGuiLayer::Init(Window* window)
{
	m_Window = window;
	if (!m_Window->isContextBound()) return;

	m_ImGuidContext = ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	ImGui::SetCurrentContext(m_ImGuidContext);

	io.ConfigWindowsMoveFromTitleBarOnly = true;

	ImGui_ImplGlfw_InitForOpenGL(window->GetGlfwWindow(), true);
	ImGui_ImplOpenGL3_Init("#version 460");

	io.FontDefault = io.Fonts->AddFontFromFileTTF(std::string(Paths::GetEngineDirectory() + "//Fonts//OpenSans-Regular.ttf").c_str(), 16.0f);
	io.Fonts->AddFontFromFileTTF(std::string(Paths::GetEngineDirectory() + "//Fonts//OpenSans-Bold.ttf").c_str(), 16.0f);

	ImGui::StyleColorsDark();
	SetDarkTheme();
	m_Valid = true;
}

void ImGuiLayer::Begin()
{
	m_Valid = false;
	if(!m_Window || !m_Window->isContextBound()) return;

	ImGui::SetCurrentContext(m_ImGuidContext);
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	m_Valid = true;
	//ImGuizmo::BeginFrame();

	//Docking
	static ImGuiID dockspaceID = 0;
	bool open = true;
	bool opt_fullscreen = true;
	bool opt_padding = false;
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;

	//ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDocking;
	if (opt_fullscreen)
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->GetWorkPos());
		ImGui::SetNextWindowSize(viewport->GetWorkSize());
		ImGui::SetNextWindowViewport(viewport->ID);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
		ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
		window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	}

 	if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
 		window_flags |= ImGuiWindowFlags_NoBackground;

	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("DockSpace Demo", &open, window_flags);
	ImGui::PopStyleVar();

	if (opt_fullscreen)
		ImGui::PopStyleVar(2);

	ImGuiIO& io = ImGui::GetIO();
	if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
	{
		ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
		ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		ImGui::End();
	}
}

void ImGuiLayer::End()
{
	m_Valid = false;
	if (!m_Window || !m_Window->isContextBound()) return;

	ImGui::SetCurrentContext(m_ImGuidContext);
	ImGuiIO& io = ImGui::GetIO();
	io.DisplaySize = ImVec2((float)m_Window->GetWidth(),  (float)m_Window->GetHeight());

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}

	m_Valid = true;
}

void ImGuiLayer::Shutdown()
{
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext(m_ImGuidContext);
	m_Valid = false;
}

void ImGuiLayer::SetDarkTheme()
{
	auto& colors = ImGui::GetStyle().Colors;
	//colors[ImGuiCol_WindowBg] = ImVec4{ 0.1f, 0.105f, 0.1f, 1.0f };

	// Headers
	colors[ImGuiCol_Header] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };
	colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_HeaderActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

	// Buttons
	colors[ImGuiCol_Button] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
	colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.4f, 0.405f, 0.41f, 1.0f };
	colors[ImGuiCol_ButtonActive] = ImVec4{ 0.25f, 0.2505f, 0.251f, 1.0f };

	// Frame BG
// 	colors[ImGuiCol_FrameBg] = ImVec4{ 0.1f, 0.105f, 0.151f, 1.0f };
// 	colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.2f, 0.205f, 0.251f, 1.0f };
// 	colors[ImGuiCol_FrameBgActive] = ImVec4{ 0.15f, 0.1505f, 0.201f, 1.0f };

	// Tabs
	colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
	colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
	colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

	// Title
	colors[ImGuiCol_TitleBg] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
	colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

}
