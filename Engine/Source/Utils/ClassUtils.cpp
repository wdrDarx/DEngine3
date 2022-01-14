#include "ClassUtils.h"
#include "DEngine.h"

bool ClassUtils::IsTemplateType(const ClassType& type)
{
	return (type.Name.find("<") != type.Name.npos && type.Name.find(">") != type.Name.npos);
}

std::string ClassUtils::GetTemplateBaseFromType(const ClassType& type)
{
	if (!IsTemplateType(type))
	{
		LOG_ERROR("Cant get template base from non temaplte type!");
	}

	return Substring(type.Name, 0, type.Name.find("<"));
}

std::string ClassUtils::GetTemplateFromType(const ClassType& type)
{
	if (!IsTemplateType(type))
	{
		LOG_ERROR("Cant get template from non temaplte type!");
	}

	std::string temp = Substring(type.Name, type.Name.find("<"), type.Name.find(">"));

	//remove "class" from inside the template
	if (auto classpos = temp.find("class") != std::string::npos)
	{
		return Substring(temp, classpos + std::string("class").length(), temp.length());
	}
	else //remove "typename"
		if (auto classpos = temp.find("typename") != std::string::npos)
		{
			return Substring(temp, classpos + std::string("typename").length(), temp.length());
		}

	return temp;
}

bool ClassUtils::IsObject(const ClassType& type)
{
	if (type.Name == "ObjectBase") return true;

	ObjectRegistry& reg = GET_SINGLETON(ObjectRegistry);
	for (auto& key : reg.GetRegisteredKeys())
	{
		if(key.name == type.Name)
			return true;
	}

	return false;
}


bool ClassUtils::IsObjectBaseOf(const std::string& child, const std::string& parent)
{
	//ehh
	if(child == parent) return true;

	ObjectRegistry& reg = GET_SINGLETON(ObjectRegistry);
	ClassType CurrentChildParent = ToRef<ObjectBase>(reg.MakeObjectFromClassName(child))->GetParentClassType();

	while (!(CurrentChildParent.Name == parent)) //every itteration, we compare the child with the parent of the parent and so on
	{
		if (CurrentChildParent.Name == "_placeholder")
			return false;

		Ref<ObjectBase> ChildParent = ToRef<ObjectBase>(reg.MakeObjectFromClassName(CurrentChildParent.Name));
		CurrentChildParent = ChildParent->GetParentClassType();
	}

	return true;
}

bool ClassUtils::IsObjectBaseOf(const ClassType& child, const ClassType& parent)
{
	return IsObjectBaseOf(child.Name, parent.Name);
};

bool ClassUtils::IsObjectBaseOf(ObjectBase* child, ObjectBase* parent)
{
	return IsObjectBaseOf(child->GetClassType(), parent->GetClassType());
}

bool ClassUtils::IsObjectBaseOf(const Ref<ObjectBase>& child, const Ref<ObjectBase>& parent)
{
	return IsObjectBaseOf(child.get(), parent.get());
}

bool ClassUtils::IsStruct(const ClassType& type)
{
	if(type.Name == "StructBase") return true;

	StructRegistry& reg = GET_SINGLETON(StructRegistry);
	for (auto& key : reg.GetRegisteredKeys())
	{
		if (key.name == type.Name)
			return true;
	}

	return false;
}

bool ClassUtils::IsStructBaseOf(const std::string& child, const std::string& parent)
{
	//ehh
	if (child == parent) return true;

	StructRegistry& reg = GET_SINGLETON(StructRegistry);
	ClassType CurrentChildParent = ToRef<StructBase>(reg.MakeObjectFromClassName(child))->GetParentClassType();

	while (!(CurrentChildParent.Name == parent)) //every itteration, we compare the child with the parent of the parent and so on
	{
		if (CurrentChildParent.Name == "StructBase")
			return false;

		Ref<StructBase> ChildParent = ToRef<StructBase>(reg.MakeObjectFromClassName(CurrentChildParent.Name));
		CurrentChildParent = ChildParent->GetParentClassType();
	}

	return true;
}

bool ClassUtils::IsStructBaseOf(const ClassType& child, const ClassType& parent)
{
	return IsStructBaseOf(child.Name, parent.Name);
}

bool ClassUtils::IsStructBaseOf(StructBase* child, StructBase* parent)
{
	return IsStructBaseOf(child->GetClassType(), parent->GetClassType());
}

bool ClassUtils::IsStructBaseOf(const Ref<StructBase>& child, const Ref<StructBase>& parent)
{
	return IsStructBaseOf(child.get(), parent.get());
}

bool ClassUtils::IsEnum(const ClassType& type)
{
	if (type.Name == "EnumBase") return true;

	EnumRegistry& reg = GET_SINGLETON(EnumRegistry);
	for (auto& key : reg.GetRegisteredKeys())
	{
		if (key.name == type.Name)
			return true;
	}

	return false;
}

