#include "EnumBase.h"

std::string EnumBase::ToString(int enumVal)
{
	for (auto& str : GetEnumMap())
	{
		if (str.second == enumVal)
			return str.first;
	}

	return "";
}

int EnumBase::FromString(const std::string& EnumName)
{
	for (auto& str : GetEnumMap())
	{
		if (str.first == EnumName)
			return str.second;
	}

	return 0;
}

