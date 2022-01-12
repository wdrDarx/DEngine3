#pragma once
#include "Core/Core.h"


//simple definitions given the name of the class and its super class
#define OBJECT_CLASS_DEF(class, superclass) using ThisClass = class; \
using Super = superclass; \
ClassType GetClassType() override { return typeid(this); }; \
using superclass::superclass;  

//Property stuff

#define PROPERTIES_MEMBER_NAME m_Properties

#define OBJECT_PROPS_BEGIN() void DefineProperties() override { Super::DefineProperties(); std::string _Category = "";

#define PROPS_CATEGORY(category) _Category = #category; 
#define PROPDEF(member, flags) PROPERTIES_MEMBER_NAME.push_back(CreatePropertyFromMember(member, #member, _Category, flags));

#define OBJECT_PROPS_END()  }


//get static class of an object
#define OBJECT_STATIC_CLASS(ObjectClass) ObjectClass::_GetStaticClass<ObjectClass>()

#define _PROP_MEMBER_NAME m_Properties
