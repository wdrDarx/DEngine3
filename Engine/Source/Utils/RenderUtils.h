#pragma once
#include "Core/Core.h"

struct DENGINE_API CubeMesh
{
public:
	void Init();
	void Draw();

	uint m_VA;
	uint m_VB;
};

struct DENGINE_API QuadMesh
{
public:
	void Init();
	void Draw();

	uint m_VA;
	uint m_VB;
};

class DENGINE_API RenderUtils
{
public:	

	//Gets current viewport size for rendering
	static vec2d GetCurrentViewport();

	//sets the render viewport size
	static void SetViewport(const vec2d& size);

	static void Clear();
	static void SetClearColor(const color3& color);
	static int GetCurrentDrawFramebuffer();
	static int GetCurrentReadFramebuffer();

	static void SetCurrentDrawFramebuffer(int framebuffer);
	static void SetCurrentReadFramebuffer(int framebuffer);

	/*
		Renders a generated cube Vertex array,
		Bind your shader before calling this
	*/
	static void RenderCube();

	/*
		Renders a generated quad Vertex array,
		Bind your shader before calling this
	*/
	static void RenderQuad();

 	static CubeMesh s_CubeMesh;
 	static QuadMesh s_QuadMesh;
};

