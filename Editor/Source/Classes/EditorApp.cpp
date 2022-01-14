#include "EditorApp.h"

//Sections
#include "ModulesSection.h"
#include "RegistrySection.h"
#include "AppObjectsSection.h"
#include "PropertiesSection.h"
#include "PerformanceSection.h"
#include "SceneObjectsSection.h"

//windows
#include "MainEditorWindow.h"


EditorApp::EditorApp() : Application()
{
	
}

void EditorApp::Init()
{
	REGISTER_OBJECT(PropertyDrawTypes);

	//create editor window
	m_EditorWindow = CreateNewEditorWindow<MainEditorWindow>()->GetWindow();
	m_EditorWindow->SetVsync(false);

	//CreateNewWindow("Test", { 1280, 720 })->SetVsync(false);

	//create base dirs
	Paths::CreateBaseDirs();

	//init imgui layer on the editor window
	GetEditorWindow()->SetCurrentContext();
	GetImGuiLayer().Init(m_EditorWindow.get());

	//Create PropertyDrawTypes object
	CreateAppObject<PropertyDrawTypes>();

	//Create sections
	CreateEditorSection<ModulesSection>(GetEditorWindow().get());
	CreateEditorSection<RegistrySection>(GetEditorWindow().get());
	CreateEditorSection<AppObjectsSection>(GetEditorWindow().get());
	CreateEditorSection<PropertiesSection>(GetEditorWindow().get());
	CreateEditorSection<PerformanceSection>(GetEditorWindow().get());
	CreateEditorSection<SceneObjectsSection>(GetEditorWindow().get());

	//Create the editor scene
	m_EditorScene = CreateAppObject<Scene>();
	m_EditorScene->SetPipeline<DefaultPipeline>(m_EditorWindow->GetRenderAPI()); //set default pipeline

	//bind to shutdown if the main window is closed
	GetEditorWindow()->BindOnWindowClosed(m_EditorWindowCloseCallback);

	//load all modules
	GetModuleManager().LoadAllModules(Paths::GetModulesDirectory());
}

void EditorApp::OnUpdate(float DeltaSeconds)
{
	BeginFrame();

	Super::OnUpdate(DeltaSeconds);

	EndFrame();
}

void EditorApp::RenderUI()
{
	//set the properties pannel selected object as the selected Object
	if (auto ptr = GetEditorSection<SceneObjectsSection>()->m_SelectedComponent.Get())
	{
		GetEditorSection<PropertiesSection>()->m_ObjectToDraw.Get() = ptr;
	}
	else
		if (auto ptr = GetEditorSection<SceneObjectsSection>()->m_SelectedObject.Get())
		{
			GetEditorSection<PropertiesSection>()->m_ObjectToDraw.Get() = ptr;
		}
		else
			if (auto ptr = GetEditorSection<AppObjectsSection>()->m_SelectedObject.Get())
			{
				GetEditorSection<PropertiesSection>()->m_ObjectToDraw.Get() = ptr;
			}


	//Create editor dockspace
	bool open = true;
	ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_PassthruCentralNode;
	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoScrollbar;
	ImGui::SetNextWindowBgAlpha(0.f);
	window_flags |=  ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
	window_flags |= ImGuiWindowFlags_NoBackground;
			
	ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Editor", &open, window_flags);
	DrawEditorMenuBar();
	ImGui::PopStyleVar();
	ImGuiID dockspace_id = ImGui::GetID("EditorDockspace");
	ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
	ImGui::PopStyleVar(2);
	for (auto& sec : GetEditorSections()) //Draw all editor sections
	{
		sec->Render();
	}
	ImGui::End();

	//end frame for all viewports (calls end frame on the scene)
	for (auto& viewport : m_Viewports)
	{
		viewport->m_SelectedComponent.Get() = GetEditorSection<SceneObjectsSection>()->m_SelectedComponent.Get();
		viewport->m_SelectedObject.Get() = GetEditorSection<SceneObjectsSection>()->m_SelectedObject.Get();
		viewport->Render();
	}

	//remove viewports that were closed
	Viewport* destroy = nullptr;
	for (auto& viewport : m_Viewports)
	{
		if (viewport->m_Close)
			destroy = viewport.get();
	}
	if (destroy)
		DestroyViewport(destroy);
}

void EditorApp::BeginFrame()
{
	//Viewports
	{
		//begin frame for all viewports (requires imGui context)
		bool InputSet = false;
		for (auto& viewport : m_Viewports)
		{
			//forward input to viewport
			if (!InputSet)
			{
				if (viewport->m_Scene)
				{
					GetEditorWindow()->GetInputManager().StopForwarding(viewport->m_Scene->GetInputManager());

					//for camera movement and stuff
					GetEditorWindow()->GetInputManager().StopForwarding(viewport->m_InputManager);
				}
				if (viewport->m_Scene && viewport->m_FocusedOnviewport)
				{
					GetEditorWindow()->GetInputManager().ForwardTo(viewport->m_Scene->GetInputManager());

					//for camera movement and stuff
					GetEditorWindow()->GetInputManager().ForwardTo(viewport->m_InputManager);
					InputSet = true;
				}
			}

			//set transform mode
			viewport->m_TransformMode = ImGuizmo::OPERATION::TRANSLATE;
		}
	}
}

void EditorApp::EndFrame()
{
	for (auto& window : GetEditorWindows())
	{
		if(window->GetWindow()->IsClosed()) continue;
		window->Render();
	}
}

void EditorApp::DrawEditorMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("Viewport"))
		{
			if (ImGui::MenuItem("Create New Viewport"))
			{
				CreateViewport(m_EditorScene);
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

Ref<Viewport> EditorApp::CreateViewport(Ref<Scene> scene)
{
	Ref<Viewport> viewport = MakeRef<Viewport>(scene, GetEditorWindow(), std::string("Viewport " + STRING(m_Viewports.size())));
	m_Viewports.push_back(viewport);
	return viewport;
}

void EditorApp::DestroyViewport(Viewport* viewport)
{
	VectorUtils::RemovePointerFromRefVector(viewport, m_Viewports);
}

