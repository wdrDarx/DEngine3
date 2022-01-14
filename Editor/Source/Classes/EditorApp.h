#pragma once
#include "DEngine.h"
#include "EditorSection.h"
#include "EditorWindow.h"
#include "Viewport.h"

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

	template<class T>
	Ref<T> CreateNewEditorWindow()
	{
		FORCE_BASE_CLASS(T, EditorWindow);

		Ref<T> newWindow = MakeRef<T>(this);
		newWindow->Init();
		m_EditorWindows.push_back(newWindow); 
		newWindow->GetWindow()->BindOnWindowClosed(m_WindowCloseEvent);
		return newWindow;
	}

	template<class T>
	void CreateEditorSection(Window* targetWindow)
	{
		FORCE_BASE_CLASS(T, EditorSection)

		Ref<T> newSection = MakeRef<T>(targetWindow, this);
		m_EditorSections.push_back(newSection);
		newSection->Init();
	}

	template<class T>
	T* GetEditorSection()
	{
		FORCE_BASE_CLASS(T, EditorSection)

		for (auto& sec : m_EditorSections)
		{
			if(Ref<T> ref = Cast<T>(sec))
				return ref.get();
		}

		LOG_ERROR("No editor section '{0}' found!", ClassType(typeid(T)).Name);
		return nullptr;
	}


	std::vector<Ref<EditorWindow>>& GetEditorWindows()
	{
		return m_EditorWindows;
	}

	std::vector<Ref<EditorSection>>& GetEditorSections()
	{
		return m_EditorSections;
	}

	const Ref<Window>& GetEditorWindow()
	{
		return m_EditorWindow;
	}

	void DrawEditorMenuBar();

	ImGuiLayer& GetImGuiLayer()
	{
		return m_ImGuiLayer;
	}

	Callback<EventWindowClosed> m_EditorWindowCloseCallback = [&](EventWindowClosed& event) 
	{
		Application::Shutdown();
	};

	Callback<EventWindowClosed> m_WindowCloseEvent = [&](EventWindowClosed& event)
	{	
		//need to call the next tick because the window still needs to destoy context
		GetMainThread().ExecuteParams([&](Window* window)
		{
			auto remove = m_EditorWindows.end();
			for (auto it = GetEditorWindows().begin(); it != GetEditorWindows().end(); it++)
			{
				if ((*it)->GetWindow().get() == window)
				{
					remove = it;
					break;
				}
			}

			if (remove != m_EditorWindows.end())
			{
				m_EditorWindows.erase(remove);
				return;
			}

		}, event.window);		
	};

	Ref<Viewport> CreateViewport(Ref<Scene> scene);
	void DestroyViewport(Viewport* viewport);

	std::vector<Ref<Viewport>> m_Viewports;

	ImGuiLayer m_ImGuiLayer;
	Ref<Window> m_EditorWindow = nullptr;
	std::vector<Ref<EditorWindow>> m_EditorWindows;
	std::vector<Ref<EditorSection>> m_EditorSections;

	Ref<Scene> GetEditorScene()
	{
		return m_EditorScene;
	};

	Ref<Scene> m_EditorScene = nullptr;
};

