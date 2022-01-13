#pragma once
#include "Core/Core.h"
#include "Framework/ClassType.h"

class ObjectBase;
class StructBase;
class ClassUtils
{
	public:
		//returns if the type is an Object
		static bool IsObject(const ClassType& type);

		//returns if an Object inherits from another Object, (this is very slow so dont call it too much, its better to do a cast)
		static bool IsObjectBaseOf(const std::string& child, const std::string& parent);

		//returns if an Object inherits from another Object, (this is very slow so dont call it too much, its better to do a cast)
		static bool IsObjectBaseOf(const ClassType& child, const ClassType& parent);

		//returns if an Object inherits from another Object, (this is very slow so dont call it too much, its better to do a cast)
		static bool IsObjectBaseOf(ObjectBase* child, ObjectBase* parent);

		//returns if an Object inherits from another Object, (this is very slow so dont call it too much, its better to do a cast)
		static bool IsObjectBaseOf(const Ref<ObjectBase>& child, const Ref<ObjectBase>& parent);

		//returns if the type is a Struct
		static bool IsStruct(const ClassType& type);

		//returns if an Struct inherits from another Object, (this is very slow so dont call it too much, its better to do a cast)
		static bool IsStructBaseOf(const std::string& child, const std::string& parent);

		//returns if an Struct inherits from another Object, (this is very slow so dont call it too much, its better to do a cast)
		static bool IsStructBaseOf(const ClassType& child, const ClassType& parent);

		//returns if an Struct inherits from another Object, (this is very slow so dont call it too much, its better to do a cast)
		static bool IsStructBaseOf(StructBase* child, StructBase* parent);

		//returns if an Struct inherits from another Object, (this is very slow so dont call it too much, its better to do a cast)
		static bool IsStructBaseOf(const Ref<StructBase>& child, const Ref<StructBase>& parent);

		
};