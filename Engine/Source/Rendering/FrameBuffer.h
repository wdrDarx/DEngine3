#pragma once
#include "Core/Core.h"
#include "RenderAPI.h"

struct FrameBufferSpec;
struct DENGINE_API ColorAttachmentSpec
{
	uint ColorFormat = GL_RGBA16F;
	uint Channels = GL_RGBA;
	uint ColorDataType = GL_UNSIGNED_BYTE;
	uint FilterMode = GL_LINEAR;
	uint AttachIndex;
};

struct DENGINE_API FrameBufferSpec
{
	uint Width, Height;
	uint Samples = 1;
	uint ColorFormat = GL_RGBA16F;
	uint Channels = GL_RGBA;
	uint ColorDataType = GL_UNSIGNED_BYTE;
	uint FilterMode = GL_LINEAR;

	uint DepthFormat = GL_DEPTH32F_STENCIL8;
	uint IntermediateColorFormat = GL_RGBA16F;
	uint IntermediateDepthFormat = GL_DEPTH32F_STENCIL8;
	bool CreateDepthTexture = true;
	bool CreateColorTexture = true;

	//optional, if empty it just creates a single color attachment
	std::vector<ColorAttachmentSpec> ColorAttachments = std::vector<ColorAttachmentSpec>();

	void AttachColor(uint AttachmentIndex, ColorAttachmentSpec spec)
	{
		spec.AttachIndex = AttachmentIndex;
		ColorAttachments.push_back(spec);
	}
};

struct DENGINE_API FrameBuffer
{
public:
	FrameBuffer(const FrameBufferSpec& spec);
	~FrameBuffer();
	const FrameBufferSpec& GetSpec() const {return m_Spec; };
	
	uint GetColorAttachement(uint AttachmentIndex = 0) const;
	uint GetDepthAttachement() const;

	void Bind();
	void Unbind();
	void Invalidate();

	void Resize(const uint& Width, const uint& Height);

	uint GetFrameBufferID() const
	{
		return m_MainFrameBuffer;
	}

private:

	struct ColorAttachment
	{
		ColorAttachmentSpec Spec;
		uint TextureID;
	};

	uint m_MainFrameBuffer = 0;
	
	std::vector<ColorAttachment> m_MainColorAttachments;
	uint m_MainDepthRenderBuffer = 0;

	uint m_IntermidiateBuffer = 0;
	std::vector<ColorAttachment> m_IntermidiateColorAttachments;

	uint m_IntermidiateDepthAttachment = 0;
	FrameBufferSpec m_Spec;
};

