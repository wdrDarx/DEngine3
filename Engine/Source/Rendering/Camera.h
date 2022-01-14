#pragma once
#include "Core/Core.h"
#include "Rendering/RenderAPI.h"
#include "Framework/EnumBase.h"

struct DENGINE_API ProjectionType : public EnumBase
{
	enum Enum
	{
		ORTHO = 0,
		PERSPECTIVE
	};

	ENUM_DEF_BEGIN(ProjectionType, Enum);
		ENUM_DEF(ORTHO);
		ENUM_DEF(PERSPECTIVE);
	ENUM_DEF_END()
};

/*
	Camera class housing a transform in world space and the view and projection matrix of the camera
*/
class DENGINE_API Camera
{
public:
	Camera(Ref<RenderAPI> renderAPI) : m_RenderAPI(renderAPI)
	{
		m_Transform = Transform();
	}

	const Transform& GetTransform() const
	{
		return m_Transform;
	}

	void SetTransform(const Transform& trans)
	{
		if(m_Transform != trans)
		{ 
			m_Transform = trans;
			RecalculateViewProjectionMatrix();
		}
	}

	void SetPosition(const vec3d& Position)
	{
		auto trans = GetTransform();
		trans.pos = Position;
		SetTransform(trans);
	}

	void SetRotation(const vec3d& rot)
	{
		auto trans = GetTransform();
		trans.rot = rot;
		SetTransform(trans);
	}

	const glm::vec3& GetRotation() const 
	{
		return GetTransform().rot; 
	}

	const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; };
	const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; };
	const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; };

	const float& GetZoom() const
	{ 
		return m_Zoom;
	}

	void SetZoom(const float& zoom)
	{
		m_Zoom = MAX(zoom, 0.01f);
		RecalculateViewProjectionMatrix();
	}

	Ref<RenderAPI> GetRenderAPI() 
	{
		return m_RenderAPI;
	}

	void RecalculateViewProjectionMatrix(bool CalcView = true);

	void SetProjectionType(const ProjectionType::Enum& proj)
	{
		m_ProjectionType = proj;
		RecalculateViewProjectionMatrix();
	}

	const ProjectionType::Enum& GetProjectionType() const
	{
		return m_ProjectionType;
	}

	void SetFOV(const float& fov)
	{
		if(fov != m_FOV)
		{ 
			m_FOV = fov;
			RecalculateViewProjectionMatrix();
		}
	}

	const float& getFOV() const
	{
		return m_FOV;
	}

	void SetViewMatrix(const glm::mat4& viewMat)
	{
		m_ViewMatrix = viewMat;
	}

	void SetProjectionMatrix(const glm::mat4& projMat)
	{
		m_ProjectionMatrix = projMat;
	}

	void SetViewProjectionMatrix(const glm::mat4& ViewProjMat)
	{
		m_ViewProjectionMatrix = ViewProjMat;
	}

	const std::array<vec4d, 6>& GetFrustrumPlaneNormals() const
	{
		return m_FrustrumPlaneNormals;
	}

	void SetNearPlane(float nearPlane)
	{
		m_NearPlane = nearPlane;
	}

	void SetFarPlane(float farPlane)
	{
		m_FarPlane = farPlane;
	}

	float GetNearPlane() const
	{
		return m_NearPlane;
	}

	float GetFarPlane() const
	{
		return m_FarPlane;
	}

	void GenFrustrumPlaneNormals();

private:
	Ref<RenderAPI> m_RenderAPI;
	ProjectionType::Enum m_ProjectionType = ProjectionType::Enum::PERSPECTIVE;
	std::array<vec4d, 6> m_FrustrumPlaneNormals;

	glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);
	glm::mat4 m_ViewMatrix = glm::mat4(1.0f);
	glm::mat4 m_ViewProjectionMatrix = glm::mat4(1.0f);

	float m_FOV = 60.f;
	float m_Zoom = 1.0f;
	float m_NearPlane = 10.f;
	float m_FarPlane = 10000.f;
	Transform m_Transform = Transform();
};