#include "QuadRenderer.h"
#include "DEngine.h"

using namespace QR;

void QuadRenderer::OnConstruct()
{
	Super::OnConstruct();

	//common vertex buffer (each quad uses this)
	m_QuadCommonBuffer = MakeRef<VertexBuffer>();
	std::vector<Vertex> ConstVerts;
	ConstVerts.push_back({ m_QuadPositions[0], m_QuadTexCoords[0] });
	ConstVerts.push_back({ m_QuadPositions[1], m_QuadTexCoords[1] });
	ConstVerts.push_back({ m_QuadPositions[2], m_QuadTexCoords[2] });
	ConstVerts.push_back({ m_QuadPositions[3], m_QuadTexCoords[3] });
	m_QuadCommonBuffer->SetData(ConstVerts.data(), ConstVerts.size() * sizeof(Vertex));

	//common index buffer per quad
	m_QuadCommonIndexBuffer = MakeRef<IndexBuffer>();
	m_QuadCommonIndexBuffer->SetData(m_QuadIndecies.data(), m_QuadIndecies.size());

	//per vertex layout 
	m_VertexBufferLayout = MakeRef<VertexBufferLayout>();
	m_VertexBufferLayout->Push<float>(3); //position
	m_VertexBufferLayout->Push<float>(2); //texture coords

	//common vertex array (contains common vertex buffer and common vertex buffer layout)
	m_QuadCommonArray = MakeRef<VertexArray>();
	m_QuadCommonArray->Addbuffer(*m_QuadCommonBuffer, *m_VertexBufferLayout);

	//matrix storage buffer per draw call
	m_StorageBuffer = MakeRef<ShaderStorageBuffer>(0); //layout = 0
}

void QuadRenderer::RenderFrame(Ref<Camera> camera)
{
	Super::RenderFrame(camera);

	
}

void QuadRenderer::PrepareFrame()
{
	Super::PrepareFrame();

	if (!GetPipeline()->GetRenderAPI()->IsShaderInCache("QuadShader"))
		GetPipeline()->GetRenderAPI()->AddShaderToCache(MakeRef<Shader>(Paths::GetEngineDirectory() + "Shaders\\QuadShader_Instanced.shader"), "QuadShader");
}

void QuadRenderer::ClearFrame()
{
	Super::ClearFrame();

}


void QuadRenderer::RenderQuads(Ref<Camera> camera)
{
	glEnable(GL_DEPTH_TEST);
	GetPipeline()->GetRenderAPI()->GetShaderFromCache("QuadShader")->Bind();
	GetPipeline()->GetRenderAPI()->GetShaderFromCache("QuadShader")->SetUniformMat4f("u_ViewProjectionMatrix", camera->GetViewProjectionMatrix());

	m_StorageBuffer->Bind();
	//draw each draw call
	for (int i = 0; i <= m_QuadBuffer.size(); i++)
	{
		auto shader = GetPipeline()->GetRenderAPI()->GetShaderFromCache("QuadShader");
		shader->Bind();
		shader->SetUniformMat4f("u_ViewProjectionMatrix", camera->GetViewProjectionMatrix());
		shader->SetUniform4f("u_Color", m_QuadBuffer[i]->color.r, m_QuadBuffer[i]->color.g, m_QuadBuffer[i]->color.b, m_QuadBuffer[i]->color.a);
		shader->SetUniformMat4f("u_Transform", World::MakeMatrix(m_QuadBuffer[i]->trans));

		//draw instanced (call.Instances.size() is the instance count)
		GetPipeline()->GetRenderAPI()->DrawIndexed(*GetPipeline()->GetRenderAPI()->GetShaderFromCache("QuadShader"), *m_QuadCommonArray, *m_QuadCommonIndexBuffer);
	}

	m_StorageBuffer->Unbind();
	glDisable(GL_DEPTH_TEST);
}

QR::Quad* QuadRenderer::SubmitQuad(const Transform& trans, const color4& color)
{
	Ref<Quad> newQuad = MakeRef<Quad>(trans, color);
	newQuad->trans = trans;
	newQuad->color = color;
	m_QuadBuffer.push_back(newQuad);
	return newQuad.get();
}


void QuadRenderer::RemoveQuad(QR::Quad* quad)
{
	VectorUtils::RemovePointerFromRefVector(quad, m_QuadBuffer);
}

void QuadRenderer::DrawQuads()
{

}


