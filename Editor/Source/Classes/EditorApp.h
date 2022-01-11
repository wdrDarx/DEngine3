#pragma once
#include "DEngine.h"
#include "EditorSection.h"

class EditorApp : public Application
{
public:
	EditorApp();
	using Super = Application;

	//called after adding application to engine
	void Init();

	void OnUpdate(float DeltaSeconds) override;
	void RenderUI();
	void BeginFrame();
	void EndFrame();

	Ref<Window> CreateNewWindow(const std::string& name, const vec2d& size);


	template<class T>
	void CreateEditorSection(Window* targetWindow)
	{
		FORCE_BASE_CLASS(T, EditorSection)

		Ref<T> newSection = MakeRef<T>(targetWindow, this);
		m_EditorSections.push_back(newSection);
		newSection->Init();
	}


	std::vector<Ref<Window>>& GetWindows()
	{
		return m_Windows;
	}

	std::vector<Ref<EditorSection>>& GetEditorSections()
	{
		return m_EditorSections;
	}

	const Ref<Window>& GetEditorWindow()
	{
		return m_EditorWindow;
	}

	ImGuiLayer& GetImGuiLayer()
	{
		return m_ImGuiLayer;
	}

	ImGuiLayer m_ImGuiLayer;
	Ref<Window> m_EditorWindow = nullptr;
	std::vector<Ref<Window>> m_Windows;
	std::vector<Ref<EditorSection>> m_EditorSections;
};

