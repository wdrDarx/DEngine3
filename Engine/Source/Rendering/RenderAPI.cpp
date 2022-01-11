#include "RenderAPI.h"
//#include "VertexArray.h"
//#include "Indexbuffer.h"
//#include "Shader.h"


void OpenGLMessageCallback(
	unsigned source,
	unsigned type,
	unsigned id,
	unsigned severity,
	int length,
	const char* message,
	const void* userParam)
{
	switch (severity)
	{
		case GL_DEBUG_SEVERITY_HIGH:         LOG_ERROR(std::string(message)); return;
		case GL_DEBUG_SEVERITY_MEDIUM:       LOG_WARN(std::string(message)); return;
		case GL_DEBUG_SEVERITY_LOW:          LOG_WARN(std::string(message)); return;
		case GL_DEBUG_SEVERITY_NOTIFICATION: LOG_TEMP(std::string(message)); return;
	}

	ASSERT(false); //severity?
}

RenderAPI::RenderAPI(GLFWwindow* windowContext) : m_windowContext(windowContext)
{
	ASSERT(windowContext); // this class requires a context
	SetCurrentContext(windowContext);

	//load GLAD
	ASSERT(gladLoadGLLoader((GLADloadproc)glfwGetProcAddress));

	//opengl debug messaging 
	glEnable(GL_DEBUG_OUTPUT);
	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
	glDebugMessageCallback(OpenGLMessageCallback, nullptr);
	glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DEBUG_SEVERITY_NOTIFICATION, 0, NULL, GL_FALSE);
}

void RenderAPI::SetCurrentContext(GLFWwindow* windowContext)
{
	PROFILE_FUNC()

	glfwMakeContextCurrent(windowContext);
	m_windowContext = windowContext;
}

void RenderAPI::ClearCurrentContext()
{
	PROFILE_FUNC()

	glfwMakeContextCurrent(nullptr);
	m_windowContext = nullptr;
}
void RenderAPI::Clear()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void RenderAPI::SetClearColor(const color4& color)
{
	glClearColor(color.r, color.g, color.b, color.a);
}

void RenderAPI::SetViewport(const vec2d& Size)
{
	glViewport(0,0, (GLsizei)Size.x, (GLsizei)Size.y);
	m_LastViewportSize = Size;
}

// void RenderAPI::DrawIndexed(Shader& shader, VertexArray& vertexArray, IndexBuffer& indexBuffer, uint32_t indexCount /*= 0*/)
// {
// 	uint32_t count = indexCount ? indexCount : indexBuffer.GetCount();
// 	shader.Bind();
// 	vertexArray.Bind();
// 	indexBuffer.Bind();
// 
// 	glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
// 
// 	shader.Unbind();
// 	vertexArray.Unbind();
// 	indexBuffer.Unbind();
// 
// 	//record draw calls
// 	m_Stats.DrawCalls++;
// }
// 
// void RenderAPI::DrawIndexed(IndexBuffer& indexBuffer)
// {
// 	indexBuffer.Bind();
// 	glDrawElements(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr);
// 	//record draw calls
// 	m_Stats.DrawCalls++;
// }
// 
// void RenderAPI::DrawInstanced(Shader& shader, VertexArray& vertexArray, IndexBuffer& indexBuffer, uint InstanceCount /*= 0*/, uint32_t indexCount /*= 0*/)
// {
// 	uint32_t count = indexCount ? indexCount : indexBuffer.GetCount();
// 	shader.Bind();
// 	vertexArray.Bind();
// 	indexBuffer.Bind();
// 
// 	glDrawElementsInstanced(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr, InstanceCount);
// 
// 	shader.Unbind();
// 	vertexArray.Unbind();
// 	indexBuffer.Unbind();
// 
// 	//record draw calls
// 	m_Stats.DrawCalls++;
// }
// 
// void RenderAPI::DrawInstanced(IndexBuffer& indexBuffer, uint InstanceCount)
// {
// 	indexBuffer.Bind();
// 	glDrawElementsInstanced(GL_TRIANGLES, indexBuffer.GetCount(), GL_UNSIGNED_INT, nullptr, InstanceCount);
// 
// 	//record draw calls
// 	m_Stats.DrawCalls++;
// }
// 
// void RenderAPI::AddShaderToCache(Ref<Shader> shader, const std::string& shaderName)
// {
// 	m_ShaderCache[shaderName] = shader;
// }
// 
// void RenderAPI::ReloadShader(const std::string& shaderName)
// {
// 	for (auto& pair : m_ShaderCache)
// 	{
// 		if (pair.first == shaderName)
// 		{ 
// 			if (pair.second->m_Filepath.empty())
// 			{
// 				if (pair.second->m_GeomPath.empty())
// 				{
// 					pair.second = MakeRef<Shader>(pair.second->m_VertPath, pair.second->m_FragPath);
// 					break;
// 				} 
// 				else
// 				{
// 					pair.second = MakeRef<Shader>(pair.second->m_VertPath, pair.second->m_GeomPath, pair.second->m_FragPath);
// 					break;
// 				}
// 				
// 			}
// 
// 			pair.second = MakeRef<Shader>(pair.second->m_Filepath);
// 			break;
// 		}
// 	}
// }
// 
// bool RenderAPI::IsShaderInCache(const std::string& shaderName)
// {
// 	bool found = false;
// 	for (auto& pair : m_ShaderCache)
// 	{
// 		if (pair.first == shaderName)
// 		{
// 			return true;
// 		}
// 	}
// 
// 	return false;
// }

