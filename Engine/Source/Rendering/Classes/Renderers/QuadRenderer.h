#pragma once
#include "Rendering/Classes/Renderer.h"
#include "Rendering/RenderCore.h"

namespace QR
{
	struct Vertex
	{
		vec3d position;
		vec2d TexCoord;
	};

	struct Quad
	{
		Transform trans;
		color4 color;
	};
};

struct QuadRendererProxy
{
	std::vector<Ref<QR::Quad>> QuadBuffer;
};



class DENGINE_API QuadRenderer : public Renderer
{
public:
	OBJECT_CLASS_DEF(QuadRenderer, Renderer)

	void OnConstruct() override;

	void PrepareFrame() override;
	void RenderFrame(Ref<Camera> camera) override;
	void ClearFrame() override;

	void RenderQuads(Ref<Camera> camera);
	QR::Quad* SubmitQuad(const Transform& trans, const color4& color);
	void RemoveQuad(QR::Quad* quad);

public:
	

	//storage buffer is shared
	Ref<ShaderStorageBuffer> m_StorageBuffer;

	//shared constant data of quad positions and texture coords
	Ref<VertexBuffer> m_QuadCommonBuffer;
	Ref<VertexArray> m_QuadCommonArray;
	Ref<IndexBuffer> m_QuadCommonIndexBuffer;

	//per vertex layout
	Ref<VertexBufferLayout> m_VertexBufferLayout;

	//per isntance layout (remember to set a layout offset at the start)
	Ref<VertexBufferLayout> m_InstanceBufferLayout;

	//consts per quad
	const std::vector<uint> m_QuadIndecies = { 0, 1, 2, 2, 3, 0 };
	const std::vector<vec2d> m_QuadTexCoords = { {0,0}, {0,1}, {1,1}, {1,0} };
	const std::vector<vec3d> m_QuadPositions = { vec3d{-0.5f, -0.5f, 0.f}, vec3d{-0.5f, 0.5f, 0.f}, vec3d{0.5f, 0.5f, 0.f}, vec3d{0.5f, -0.5f, 0.f} };
};
