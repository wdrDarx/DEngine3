#include "EditorApp.h"
#include "ModulesSection.h"

EditorApp::EditorApp() : Application()
{
	
}

void EditorApp::Init()
{
	//create editor window
	m_EditorWindow = CreateNewWindow("DEditor", { 1280, 720 });

	CreateNewWindow("Test", { 1280, 720 });

	//create base dirs
	Paths::CreateBaseDirs();

	//load all modules
	GetModuleManager().LoadAllModules(Paths::GetModulesDirectory());

	//init imgui layer on the editor window
	GetEditorWindow()->SetCurrentContext();
	GetImGuiLayer().Init(m_EditorWindow.get());

	//Create the modules section
	CreateEditorSection<ModulesSection>(GetEditorWindow().get());
}

void EditorApp::OnUpdate(float DeltaSeconds)
{
	BeginFrame();

	Super::OnUpdate(DeltaSeconds);

	EndFrame();
}

void EditorApp::RenderUI()
{
	//render all sections
	for (auto& sec : GetEditorSections())
		sec->Render();

}

void EditorApp::BeginFrame()
{
	for (auto& window : GetWindows())
	{
		window->StartFrame();
	}
}

void EditorApp::EndFrame()
{
	for (auto& window : GetWindows())
	{
		if (window == GetEditorWindow())
		{		
			//if (m_ImGuiLayer.IsValid())
			{
				window->SetCurrentContext();
				m_ImGuiLayer.Begin();
				RenderUI();
				m_ImGuiLayer.End();
			}	
		}

		window->EndFrame();
	}
}

Ref<Window> EditorApp::CreateNewWindow(const std::string& name, const vec2d& size)
{
	Ref<Window> newWindow = MakeRef<Window>(name, size.x, size.y);
	m_Windows.push_back(newWindow);
	return newWindow;
}
