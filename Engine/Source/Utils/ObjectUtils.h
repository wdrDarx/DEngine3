#pragma once
#include "Framework/ObjectBase.h"

class ObjectUtils
{
	public:

		//returns whethere a property has been altered with a check against
		static bool HasPropertyChanged(const Property& CheckAgainst, const ObjectBase& DefaultObject);

		//resets a property to its default class value by name
		static void ResetProperty(const std::string& PropName, ObjectBase& TargetObject);
};