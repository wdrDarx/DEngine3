#pragma once
#include "DEngine.h"

class Window;
class EditorApp;

class EditorSection
{
public:
	EditorSection(Window* parentWindow, EditorApp* editor) : m_EditorApp(editor), m_Window(parentWindow)
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

	EditorApp* m_EditorApp;
	Window* m_Window;
};

