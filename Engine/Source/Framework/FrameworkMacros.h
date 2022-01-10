#pragma once
#include "Core/Core.h"


//simple definitions given the name of the class and its super class
#define OBJECT_CLASS_DEF(class, superclass) using ThisClass = class; \
using Super = superclass; \
ClassType GetClassType() override { return typeid(this); }; \
using superclass::superclass;  

//get static class of an object
#define OBJECT_STATIC_CLASS(ObjectClass) ObjectClass::_GetStaticClass<ObjectClass>()

#define _PROP_MEMBER_NAME m_Properties
