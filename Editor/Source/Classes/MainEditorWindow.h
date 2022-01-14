#pragma once
#include "DEngine.h"
#include "EditorApp.h"

class MainEditorWindow : public EditorWindow
{
public:
	using EditorWindow::EditorWindow;

	void Init() override
	{
		m_Window = MakeRef<Window>("DEditor", 1280, 720);
	}

	void Render() override
	{
		m_Window->StartFrame();
		m_Window->SetCurrentContext();
		GetEditorApp()->GetImGuiLayer().Begin();

		GetEditorApp()->RenderUI();

		GetEditorApp()->GetImGuiLayer().End();
		m_Window->EndFrame();
	}
};

class TestEditorWindow : public EditorWindow
{
public:
	using EditorWindow::EditorWindow;

	void Init() override
	{
		m_Window = MakeRef<Window>("Test", 1280, 720);
	}

	void Render() override
	{
 		m_Window->StartFrame();

 		m_Window->EndFrame();
	}
};

