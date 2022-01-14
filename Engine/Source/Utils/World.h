#pragma once
#include "Core/Core.h"

/*
	2 dimentional transform
*/
struct DENGINE_API Transform2D
{
	Transform2D(const vec2d& p, const float& r, const vec2d& s) :pos(p), rot(r), scale(s)
	{

	}
	vec2d pos;
	float rot;
	vec2d scale;
};

#define SMALL_NUMBER		(1.e-8f)
#define KINDA_SMALL_NUMBER	(1.e-4f)
#define BIG_NUMBER			(3.4e+38f)

#define FASTASIN_HALF_PI (1.5707963050f)
#define INV_PI			(0.31830988618f)
#define HALF_PI			(1.57079632679f)

class b2World;
struct b2RayCastOutput;
class Camera;

/*
	Util class for transformations and matrix stuff
*/
class DENGINE_API World
{
public:

	static vec3d Radians(const vec3d& rad);
	static vec3d Degrees(const vec3d& deg);

	static float TriangleArea2D(const vec2d& v1, const vec2d& v2, const vec2d& v3);
	static bool BoxvsPointCollisionCheck2D(const vec2d& v1, const vec2d& v2, const vec2d& v3, const vec2d& v4, const vec2d& worldPos);
	static bool BoxvsPointCollisionCheck2D(const Bound& Box, const vec2d& worldPos);

// 	static vec2d ScreenSpaceToWorldSpace(Camera* camera, const vec2d& in);
// 	static vec2d WorldSpaceToScreenSpace(Camera* camera, const vec2d& in);

	static vec3d VectorToRotation(const vec3d& VectorDir, const vec3d& WorldSpaceUpVector = vec3d(0,1,0));

	static glm::quat RotationDegreesToQuat(const vec3d& Rotation);
	static vec3d QuatToRotationDegrees(const glm::quat& Rotation);

	static vec2d Rotate2D(const vec2d& in, const float& Angle = 0.f);
	static vec3d Rotate3D(const vec3d& in, const vec3d& rot = {0,0,0});

	template<typename T>
	static T Lerp(const T& A, const T& B, float Alpha)
	{
		Alpha = MIN(Alpha, 1.0f);
		Alpha = MAX(Alpha, 0.f);

		return B * Alpha + A * (1.f - Alpha);
	}

	static float  DotProduct2D(const vec2d& v1, const vec2d& v2);
	static vec2d CrossProduct2D(const vec2d& in1);
	static float LookAt2D(const vec2d& start, const vec2d& end);
	static float Length2D(const vec2d& in);
	static vec2d Normalize2D(const vec2d& in);

	static Transform Make3D(const vec2d& pos, const float& rot, const vec2d& scale);
	static Transform2D Make2D(const Transform& trans);

	static glm::mat4 MakeMatrix(const Transform& trans);
	static Transform MakeTransform(const glm::mat4& mat);

	static Transform OffsetChildByParent(const Transform& parent, const Transform& child);
	static Transform SubtractTransform(const Transform& A, const Transform& B);

	static vec3d GetForwardVector(const vec3d& Rot);
	static vec3d GetRightVector(const vec3d& Rot);
	static vec3d GetUpVector(const vec3d& Rot);

	static glm::quat MatToQuat(const glm::mat4& mat);
	static glm::mat4 QuatToMat(const glm::quat& rotation);

	static bool IsNearlyZero(const vec3d& in);
	static bool IsNearlyZero(const vec2d& in);
	static bool IsNearlyZero(float in);
	static bool NearlyEqual(const vec3d& v1, const vec3d& v2);
	static bool NearlyEqual(const Transform& T1, const Transform& T2);

	static float Fmod(float X, float Y);
	static float ClampAngle(float Angle);
	static float NormalizeAngle(float Angle);
	static vec3d NormalizeRot(const vec3d& rot);
	static vec3d LerpRot(const vec3d& A, const vec3d& B, float Alpha);
	static vec3d LerpRotLong(const vec3d& A, const vec3d& B, float Alpha);
	static vec3d InterpRot(const vec3d& A, const vec3d& B, float DeltaTime, float Speed);

	static glm::quat VectorDirToQuat(const vec3d& DirVector);
	static glm::quat LerpQuat(const glm::quat& A, const glm::quat& B, float Alpha);
	static glm::quat CombineQuat(const glm::quat& Parent, const glm::quat& Child);

	static float FastAsin(float rad);
	static float Atan2(float y, float x);
	static void  SinCos(float* ScalarSin, float* ScalarCos, float  Value);

	static vec3d QuatToEuler(const glm::quat& q);
	static glm::quat EulerToQuat(const vec3d& rot);

	static vec3d RotateVector(const vec3d& inVector, const vec3d& rotation);
};

