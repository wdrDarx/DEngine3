#include "Camera.h"
#include "DEngine.h"

void Camera::RecalculateViewProjectionMatrix(bool CalcView /*= true*/)
{
	if(CalcView)
	{
		glm::mat4 transform = World::MakeMatrix(GetTransform());
		m_ViewMatrix = glm::inverse(transform);
	}

	float NewWidth = (float)GetRenderAPI()->GetViewportSize().x * m_Zoom;/// WidthRatio;
	float NewHeight = (float)GetRenderAPI()->GetViewportSize().y * m_Zoom; /// HeightRatio;


	if (m_ProjectionType == ProjectionType::Enum::ORTHO)
		m_ProjectionMatrix = glm::ortho(-NewWidth, NewWidth, -NewHeight, NewHeight, m_NearPlane, m_FarPlane);
	else
	{
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), NewWidth / NewHeight, m_NearPlane, m_FarPlane);
	}


	m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;

	GenFrustrumPlaneNormals();
}

void Camera::GenFrustrumPlaneNormals()
{
		// Left Frustum Plane
		// Add first column of the matrix to the fourth column
		const glm::mat4& viewProj = GetViewProjectionMatrix(); 
		m_FrustrumPlaneNormals[0].x = viewProj[0][3] + viewProj[0][0];
		m_FrustrumPlaneNormals[0].y = viewProj[1][3] + viewProj[1][0];
		m_FrustrumPlaneNormals[0].z = viewProj[2][3] + viewProj[2][0];
		m_FrustrumPlaneNormals[0].w = viewProj[3][3] + viewProj[3][0];

		// Right Frustum Plane
		// Subtract first column of matrix from the fourth column
		m_FrustrumPlaneNormals[1].x = viewProj[0][3] - viewProj[0][0];
		m_FrustrumPlaneNormals[1].y = viewProj[1][3] - viewProj[1][0];
		m_FrustrumPlaneNormals[1].z = viewProj[2][3] - viewProj[2][0];
		m_FrustrumPlaneNormals[1].w = viewProj[3][3] - viewProj[3][0];

		// Top Frustum Plane
		// Subtract second column of matrix from the fourth column
		m_FrustrumPlaneNormals[2].x = viewProj[0][3] - viewProj[0][1];
		m_FrustrumPlaneNormals[2].y = viewProj[1][3] - viewProj[1][1];
		m_FrustrumPlaneNormals[2].z = viewProj[2][3] - viewProj[2][1];
		m_FrustrumPlaneNormals[2].w = viewProj[3][3] - viewProj[3][1];

		// Bottom Frustum Plane
		// Add second column of the matrix to the fourth column
		m_FrustrumPlaneNormals[3].x = viewProj[0][3] + viewProj[0][1];
		m_FrustrumPlaneNormals[3].y = viewProj[1][3] + viewProj[1][1];
		m_FrustrumPlaneNormals[3].z = viewProj[2][3] + viewProj[2][1];
		m_FrustrumPlaneNormals[3].w = viewProj[3][3] + viewProj[3][1];

		// Near Frustum Plane
		// We could add the third column to the fourth column to get the near plane,
		// but we don't have to do this because the third column IS the near plane
		m_FrustrumPlaneNormals[4].x = viewProj[0][2];
		m_FrustrumPlaneNormals[4].y = viewProj[1][2];
		m_FrustrumPlaneNormals[4].z = viewProj[2][2];
		m_FrustrumPlaneNormals[4].w = viewProj[3][2];

		// Far Frustum Plane
		// Subtract third column of matrix from the fourth column
		m_FrustrumPlaneNormals[5].x = viewProj[0][3] - viewProj[0][2];
		m_FrustrumPlaneNormals[5].y = viewProj[1][3] - viewProj[1][2];
		m_FrustrumPlaneNormals[5].z = viewProj[2][3] - viewProj[2][2];
		m_FrustrumPlaneNormals[5].w = viewProj[3][3] - viewProj[3][2];

		// Normalize plane normals (A, B and C (xyz))
		// Also take note that planes face inward
		for (uint i = 0; i < 6; ++i)
		{
			glm::normalize(m_FrustrumPlaneNormals[i]);
		}
}
