#pragma once
#include "DEngine.h"

class Window;
class EditorApp;

class EditorWindow
{
public:
	EditorWindow(EditorApp* editor) : m_EditorApp(editor)
	{

	}

	virtual void Init()
	{

	}

	virtual void Render()
	{

	}

	EditorApp* GetEditorApp()
	{
		return m_EditorApp;
	}

	Ref<Window> GetWindow()
	{
		return m_Window;
	}

	EditorApp* m_EditorApp;
	Ref<Window> m_Window;
};

