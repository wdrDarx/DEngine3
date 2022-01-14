#include "FrameBuffer.h"


FrameBuffer::FrameBuffer(const FrameBufferSpec& spec) : m_Spec(spec)
{
	Invalidate();
}

FrameBuffer::~FrameBuffer()
{
	if (m_MainFrameBuffer != 0)
	{
		glDeleteFramebuffers(1, &m_MainFrameBuffer);

		for (auto& attachment : m_MainColorAttachments)
		{
			if(attachment.TextureID > 0)
				glDeleteTextures(1, &attachment.TextureID);
		}
		m_MainColorAttachments.clear();

		if (m_MainDepthRenderBuffer > 0)
			glDeleteRenderbuffers(1, &m_MainDepthRenderBuffer);
	}

	if (m_IntermidiateBuffer != 0)
	{
		glDeleteFramebuffers(1, &m_IntermidiateBuffer);

		for (auto& attachment : m_IntermidiateColorAttachments)
		{
			if (attachment.TextureID > 0)
				glDeleteTextures(1, &attachment.TextureID);
		}
		m_IntermidiateColorAttachments.clear();

		if (m_IntermidiateDepthAttachment > 0)
			glDeleteTextures(1, &m_IntermidiateDepthAttachment);
	}
}

uint FrameBuffer::GetColorAttachement(uint AttachmentIndex) const
{
	if (AttachmentIndex >= m_MainColorAttachments.size())	
	{
		LOG_ERROR("Color attachment index doesnt exist!");
		return 0;
	}	

	//save previously bound framebuffer (because Invalidate binds framebuffer 0)
	GLint DrawFrameBuffer = 0;
	GLint ReadFrameBuffer = 0;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &DrawFrameBuffer);
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &ReadFrameBuffer);

	GLint LastReadBuffer = 0;
	glGetIntegerv(GL_READ_BUFFER, &LastReadBuffer);

	if (m_Spec.Samples <= 1)
	{
		return m_MainColorAttachments[AttachmentIndex].TextureID;
	}
	else
	{	
		glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MainFrameBuffer);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, m_IntermidiateBuffer);

		uint attachment = GL_COLOR_ATTACHMENT0 + AttachmentIndex;

		//intermediate color attachments should match the index of the main attachments
		glReadBuffer(attachment);
		glDrawBuffers(1, &attachment);

		glBlitFramebuffer(0, 0, m_Spec.Width, m_Spec.Height, 0, 0, m_Spec.Width, m_Spec.Height, GL_COLOR_BUFFER_BIT, m_IntermidiateColorAttachments[AttachmentIndex].Spec.FilterMode);

		glBindFramebuffer(GL_DRAW_FRAMEBUFFER, DrawFrameBuffer);
		glBindFramebuffer(GL_READ_FRAMEBUFFER, ReadFrameBuffer);

		return m_IntermidiateColorAttachments[AttachmentIndex].TextureID;
	}
}

uint FrameBuffer::GetDepthAttachement() const
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, m_MainFrameBuffer);
	glBindRenderbuffer(GL_DRAW_FRAMEBUFFER, m_IntermidiateBuffer);
	glBlitFramebuffer(0, 0, m_Spec.Width, m_Spec.Height, 0, 0, m_Spec.Width, m_Spec.Height, GL_DEPTH_BUFFER_BIT, GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	return m_IntermidiateDepthAttachment;
}

void FrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, m_MainFrameBuffer);
	glViewport(0,0, m_Spec.Width, m_Spec.Height);
}

void FrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void FrameBuffer::Invalidate()
{
	GLuint TextureType;
	TextureType = m_Spec.Samples <= 1 ? GL_TEXTURE_2D : GL_TEXTURE_2D_MULTISAMPLE;

	//gen main framebuffer
	{
		if (m_MainFrameBuffer != 0)
		{
			glDeleteFramebuffers(1, &m_MainFrameBuffer);

			for (auto& attachment : m_MainColorAttachments)
			{
				if (attachment.TextureID > 0)
					glDeleteTextures(1, &attachment.TextureID);
			}
			m_MainColorAttachments.clear();

			if (m_MainDepthRenderBuffer > 0)
				glDeleteRenderbuffers(1, &m_MainDepthRenderBuffer);
		}

		glCreateFramebuffers(1, &m_MainFrameBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_MainFrameBuffer);

		//create just 1 color attachment as default
		if(m_Spec.ColorAttachments.size() < 1 && m_Spec.CreateColorTexture)
			m_Spec.ColorAttachments.push_back({ m_Spec.ColorFormat, m_Spec.Channels, m_Spec.ColorDataType, m_Spec.FilterMode, 0 });

		for (uint i = 0; i < m_Spec.ColorAttachments.size(); i++)
		{
			uint ColorTexture = 0;
			glCreateTextures(TextureType, 1, &ColorTexture);
			glBindTexture(TextureType, ColorTexture);
			if (TextureType == GL_TEXTURE_2D)
			{
				glTexImage2D(TextureType, 0, m_Spec.ColorAttachments[i].ColorFormat , m_Spec.Width, m_Spec.Height, 0, m_Spec.ColorAttachments[i].Channels, m_Spec.ColorAttachments[i].ColorDataType, nullptr);
				glTexParameteri(TextureType, GL_TEXTURE_MIN_FILTER, m_Spec.ColorAttachments[i].FilterMode);
				glTexParameteri(TextureType, GL_TEXTURE_MAG_FILTER, m_Spec.ColorAttachments[i].FilterMode);
			}
			else
			{
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, m_Spec.Samples, m_Spec.ColorAttachments[i].ColorFormat, m_Spec.Width, m_Spec.Height, GL_TRUE);
				glBindTexture(GL_TEXTURE_2D_MULTISAMPLE, 0);
			}
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_Spec.ColorAttachments[i].AttachIndex, TextureType, ColorTexture, 0);

			auto ColorSpec = m_Spec.ColorAttachments[i];
			ColorSpec.AttachIndex = i;
			m_MainColorAttachments.push_back({ ColorSpec, ColorTexture });
		}

		std::vector<uint> ColorBuffers;
		ColorBuffers.resize(m_MainColorAttachments.size());
		for (uint i = 0; i < m_MainColorAttachments.size(); i++)
		{
			ColorBuffers[i] = GL_COLOR_ATTACHMENT0 + i;
		}
		glNamedFramebufferDrawBuffers(m_MainFrameBuffer, m_MainColorAttachments.size(), ColorBuffers.data());

		if(m_Spec.CreateDepthTexture)
		{ 
			glGenRenderbuffers(1, &m_MainDepthRenderBuffer);
			glBindRenderbuffer(GL_RENDERBUFFER, m_MainDepthRenderBuffer);

			if(m_Spec.Samples > 1)
				glRenderbufferStorageMultisample(GL_RENDERBUFFER, m_Spec.Samples, m_Spec.DepthFormat, m_Spec.Width, m_Spec.Height);
			else
				glRenderbufferStorage(GL_RENDERBUFFER, m_Spec.DepthFormat, m_Spec.Width, m_Spec.Height);

			glBindRenderbuffer(GL_RENDERBUFFER, 0); //unbind the render buffer
			glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, m_MainDepthRenderBuffer);
		}
		
		ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//generate intermediate if the frame buffer is multi sampled
	if (TextureType == GL_TEXTURE_2D_MULTISAMPLE)
	{
		if (m_IntermidiateBuffer != 0)
		{
			glDeleteFramebuffers(1, &m_IntermidiateBuffer);

			for (auto& attachment : m_IntermidiateColorAttachments)
			{
				if (attachment.TextureID > 0)
					glDeleteTextures(1, &attachment.TextureID);
			}
			m_IntermidiateColorAttachments.clear();

			if (m_IntermidiateDepthAttachment > 0)
				glDeleteTextures(1, &m_IntermidiateDepthAttachment);
		}

		glCreateFramebuffers(1, &m_IntermidiateBuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, m_IntermidiateBuffer);

		for(uint i = 0; i < m_Spec.ColorAttachments.size(); i++)
		{ 
			uint ColorTexture = 0;
			glCreateTextures(GL_TEXTURE_2D, 1, &ColorTexture);
			glBindTexture(GL_TEXTURE_2D, ColorTexture);

			glTexImage2D(GL_TEXTURE_2D, 0, m_Spec.ColorAttachments[i].ColorFormat, m_Spec.Width, m_Spec.Height, 0, m_Spec.ColorAttachments[i].Channels, m_Spec.ColorAttachments[i].ColorDataType, nullptr);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, m_Spec.ColorAttachments[i].FilterMode);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, m_Spec.ColorAttachments[i].FilterMode);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + m_Spec.ColorAttachments[i].AttachIndex, GL_TEXTURE_2D, ColorTexture, 0);
			m_IntermidiateColorAttachments.push_back({ m_Spec.ColorAttachments[i], ColorTexture});
		}

		glCreateTextures(GL_TEXTURE_2D, 1, &m_IntermidiateDepthAttachment);
		glBindTexture(GL_TEXTURE_2D, m_IntermidiateDepthAttachment);
		glTexStorage2D(GL_TEXTURE_2D, 1, m_Spec.IntermediateDepthFormat, m_Spec.Width, m_Spec.Height);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_IntermidiateDepthAttachment, 0);

		ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}
}

void FrameBuffer::Resize(const uint& Width, const uint& Height)
{
	if(m_Spec.Width == Width && m_Spec.Height == Height) return;

	//save previously bound framebuffer (because Invalidate binds framebuffer 0)
	GLint DrawFrameBuffer = 0;
	GLint ReadFrameBuffer = 0;
	glGetIntegerv(GL_DRAW_FRAMEBUFFER_BINDING, &DrawFrameBuffer);
	glGetIntegerv(GL_READ_FRAMEBUFFER_BINDING, &ReadFrameBuffer);

	m_Spec.Width = Width;
	m_Spec.Height = Height;
	
	glViewport(0, 0, m_Spec.Width, m_Spec.Height);
	Invalidate();

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, DrawFrameBuffer);
	glBindFramebuffer(GL_READ_FRAMEBUFFER, ReadFrameBuffer);
}
