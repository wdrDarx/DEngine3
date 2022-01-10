#pragma once
#include <cmath>
#include <algorithm>
#include <iostream>
#include <type_traits>
#include <typeinfo>
#include <typeindex>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtx/rotate_vector.hpp>
#define USE_GLM_TYPES

//# define M_PI  3.14159265358979323846  /* pi */
constexpr float M_PI = (float)3.14159265358979323846; /* pi */

const float DegToRad = M_PI / 180.f;
const float RadToDeg = 180.f / M_PI;

typedef uint32_t uint;
typedef uint64_t uint64;
typedef uint8_t byte;
typedef uintptr_t uptr;



inline std::string Substring(const std::string& source, const size_t& start, const size_t& end)
{
	return std::string(source.begin() + start, source.begin() + end);
}

//enum with object class type
enum class ObjectClassType
{
	OBJECT = 0,
	APPOBJECT,
	SCENEOBJECT,
	OBJECTCOMPONENT
};

//unique ID (0 means invalid)
struct UID
{
	uint64 ID = 0;

	bool operator==(const UID& other) const
	{
		return ID == other.ID;
	}

	bool operator!=(const UID& other) const
	{
		return ID != other.ID;
	}

	// 0 = invalid
	operator bool() const
	{
		return ID != 0;
	}

	// 0 = invalid
	bool operator!() const
	{
		return ID == 0;
	}
};

//contains type info and friendly name of a class
struct ClassType
{
	std::type_index typeIndex;
	std::string Name;

	static std::string GetFriendlyTypeName(const std::type_index& index)
	{
		std::string base = index.name();
		//remove all spaces
		base.erase(std::remove_if(base.begin(), base.end(), isspace), base.end());

		auto endpos = std::min<size_t>(base.find("const"), base.find("*"));

		if(endpos == std::string::npos)
			endpos = base.length(); //for non pointer types

		if (auto classpos = base.find("class") != std::string::npos)
		{
			return Substring(base, classpos + std::string("class").length() - 1, endpos);
		} else
		if (auto structpos = base.find("struct") != std::string::npos)
		{
			return Substring(base, structpos + std::string("struct").length() - 1, endpos);
		}
		return "";
	}

	ClassType(const std::type_index& index) : typeIndex(index)
	{
		Name = GetFriendlyTypeName(index);
	}

};

#ifdef USE_GLM_TYPES
typedef glm::lowp_vec4 color4;
typedef glm::lowp_vec3 color3;
typedef glm::vec3 vec3d;
typedef glm::vec4 vec4d;
typedef glm::vec2 vec2d;
typedef glm::quat quat;


struct Transform
{
	vec3d pos;
	vec3d rot;
	vec3d scale = {1,1,1};

	Transform() : pos({ 0,0,0 }), rot({ 0,0,0 }), scale({ 1,1,1 })
	{

	}

	Transform(const vec3d& p, const vec3d& r, const vec3d& s) : pos(p), rot(r), scale(s)
	{
	
	}

	Transform operator+(const Transform& rhs) const
	{
		return Transform(this->pos + rhs.pos, this->rot + rhs.rot, this->scale + rhs.scale);
	}

	Transform operator-(const Transform& rhs) const
	{
		return Transform(this->pos - rhs.pos, this->rot - rhs.rot, this->scale - rhs.scale);
	}

	bool operator==(const Transform& rhs) const
	{
		return this->pos == rhs.pos && this->rot == rhs.rot && this->scale == rhs.scale;
	}

	bool operator!=(const Transform& rhs) const
	{
		return !(this->pos == rhs.pos && this->rot == rhs.rot && this->scale == rhs.scale);
	}
};

struct Bound
{
	vec3d min;
	vec3d max;
};
#endif

enum class TextureType : uint
{
	RGBA = 0,
	HDR = 1
};

enum class FilterType : uint
{
	LINEAR = 0,
	NEAREST = 1
};

enum class WrapMode : uint
{
	REPEAT = 0,
	CLAMP = 1
};

//describes a texture format
struct TextureSpec
{
	TextureType Type = TextureType::RGBA;
	FilterType Filter = FilterType::LINEAR;
	WrapMode Wrap = WrapMode::REPEAT;
	bool Anisotropic = true;
};

#if 0
struct vec2d
{
	float x;
	float y;

	vec2d() : x(0), y(0)
	{}

	vec2d(const float& inx, const float& iny) : x(inx), y(iny)
	{}

	const bool operator==(const vec2d& rhs) const
	{
		return (rhs.x == this->x) && (rhs.y == this->y);
	}

	const bool operator>(const vec2d& rhs) const
	{
		return (this->x > rhs.x) && (this->y > rhs.y);
	}

	const bool operator!=(const vec2d& rhs)
	{
		return (rhs.x != this->x) || (rhs.y != this->y);
	}
	const vec2d operator+(const vec2d& rhs) const
	{
		return vec2d(rhs.x + this->x, rhs.y + this->y);
	}
	const vec2d operator-(const vec2d& rhs) const
	{
		return vec2d(this->x - rhs.x, this->y - rhs.y);
	}
	const vec2d operator*(const vec2d& rhs)
	{
		return vec2d(rhs.x * this->x, rhs.y * this->y);
	}
	const vec2d operator*(const float& rhs) const
	{
		return vec2d(rhs * this->x, rhs * this->y);
	}
	const vec2d operator/(const vec2d& rhs) const
	{
		return vec2d(this->x / rhs.x, this->y / rhs.y);
	}
	const vec2d operator/(const float& rhs) const
	{
		return vec2d(this->x / rhs, this->y / rhs);
	}
	void operator+=(const vec2d& rhs)
	{
		this->x += rhs.x;
		this->y += rhs.y;
	}
};

struct vec3d
{
	float x;
	float y;
	float z;

	vec3d() : x(0), y(0), z(0)
	{}

	vec3d(const float& inx, const float& iny, const float& inz) : x(inx), y(iny), z(inz)
	{}

	bool operator==(const vec3d& rhs) const
	{
		return (rhs.x == this->x) && (rhs.y == this->y) && (rhs.z == this->z);
	}
	bool operator!=(const vec3d& rhs)
	{
		return (rhs.x != this->x) || (rhs.y != this->y) && (rhs.z != this->z);
	}
	const vec3d operator+(const vec3d& rhs) const
	{
		return vec3d(rhs.x + this->x, rhs.y + this->y, rhs.z + this->z);
	}
	const vec3d operator-(const vec3d& rhs) const
	{
		return vec3d(this->x - rhs.x, this->y - rhs.y, this->z - rhs.z);
	}
	vec3d operator*(const vec3d& rhs) const
	{
		return vec3d(rhs.x * this->x, rhs.y * this->y, rhs.z * this->z);
	}
	vec3d operator*(const float& rhs) const
	{
		return vec3d(rhs * this->x, rhs * this->y, rhs * this->z);
	}
	vec3d operator/(const vec3d& rhs) const
	{
		return vec3d(std::max<float>(this->x / rhs.x, 1.0f), std::max<float>(this->y / rhs.y, 1.0f), std::max<float>(this->z / rhs.z, 1.0f));
	}
};

struct color3
{
	float r;
	float g;
	float b;

	color3() : r(0), b(0), g(0)
	{

	}

	color3(const float& inr, const float& ing, const float& inb) : r(inr), g(ing), b(inb)
	{}

	bool operator==(const color3& rhs)
	{
		return (rhs.r == this->r) && (rhs.g == this->g) && (rhs.b == this->b);
	}
	bool operator!=(const color3& rhs)
	{
		return (rhs.r != this->r) && (rhs.g != this->g) && (rhs.b != this->b);
	}
	color3 operator+(const color3& rhs)
	{
		return color3(rhs.r + this->r, rhs.g + this->g, rhs.b + this->b);
	}
	color3 operator-(const color3& rhs)
	{
		return color3(this->r - rhs.r, this->g - rhs.g, this->b - rhs.b);
	}
	color3 operator*(const color3& rhs)
	{
		return color3(rhs.r * this->r, rhs.g * this->g, rhs.b * this->b);
	}
	color3 operator/(const color3& rhs)
	{
		return color3(this->r / rhs.r, this->g / rhs.g, this->b / rhs.b);
	}

	color3 operator*(const float& rhs) const
	{
		return color3(this->r / rhs, this->g / rhs, this->b / rhs);
	}
};

#endif