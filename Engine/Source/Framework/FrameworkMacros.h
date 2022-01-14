#pragma once
#include "Core/Core.h"


//simple definitions given the name of the class and its super class
#define OBJECT_CLASS_DEF(class, superclass) using ThisClass = class; \
using Super = superclass; \
ClassType GetClassType() const override { return typeid(this); }; \
ClassType GetParentClassType() const override { return typeid(Super); }; \
using superclass::superclass;  

//simple definitions given the name of the class and its super class
#define STRUCT_CLASS_DEF(class, superclass) using ThisClass = class; \
using Super = superclass; \
ClassType GetClassType() const override { return typeid(this); }; \
ClassType GetParentClassType() const override { return typeid(Super); }; \
ThisClass() : Super(false) { DefineProperties(); }

//Property stuff

#define PROPERTIES_MEMBER_NAME m_Properties

#define PROPS_BEGIN() void DefineProperties() override { Super::DefineProperties(); std::string _Category = "Default"; std::map<std::string, std::string> _Metadata; _Metadata["Category"] = _Category;

#define PROPS_CATEGORY(category) _Category = #category; _Metadata["Category"] = _Category;
#define PROPDEF(member, flags) PROPERTIES_MEMBER_NAME.push_back(CreatePropertyFromMember(member, #member, _Metadata, flags));
#define PROPDEF_NAME(member, name, flags) PROPERTIES_MEMBER_NAME.push_back(CreatePropertyFromMember(member, name, _Metadata, flags));

#define PROPS_END()  }

//get static class of an object
#define OBJECT_STATIC_CLASS(ObjectClass) ObjectClass::_GetStaticClass<ObjectClass>()

