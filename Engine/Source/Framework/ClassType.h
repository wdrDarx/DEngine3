#pragma once
#include "Core/Core.h"


//contains type info and friendly name of a class
struct ClassType
{
	std::type_index typeIndex;
	std::string Name;

	bool operator==(const ClassType& other) const
	{
		return other.typeIndex == typeIndex;
	}

	bool operator<(const ClassType& rhs) const
	{
		return typeIndex < rhs.typeIndex;
	}

	static std::string GetFriendlyTypeName(const std::type_index& index)
	{
		std::string base = index.name();
		//remove all spaces
		base.erase(std::remove_if(base.begin(), base.end(), isspace), base.end());

		auto endpos = std::min<size_t>(base.find("const"), base.find("*"));

		if (endpos == std::string::npos)
			endpos = base.length(); //for non pointer types

		if (auto structpos = base.find("struct") != std::string::npos)
		{
			return Substring(base, structpos + std::string("struct").length() - 1, endpos);
		}
		else
			if (auto classpos = base.find("class") != std::string::npos)
			{
				return Substring(base, classpos + std::string("class").length() - 1, endpos);
			}
			
		return "";
	}

	ClassType(const std::type_index& index) : typeIndex(index)
	{
		Name = GetFriendlyTypeName(index);
	}

};

namespace std
{
	template <>
	struct hash<ClassType>
	{
		std::size_t operator()(const ClassType& k) const
		{
			using std::size_t;
			using std::hash;
			using std::type_index;

			return hash<std::type_index>()(k.typeIndex);
		}
	};
}