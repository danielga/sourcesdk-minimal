#pragma once

#include <string>
#include <list>

class ILegacyAddons
{
public:
	struct Information
	{
		std::string name;
		std::string path;
		std::string luapath;
		std::string gamemodepath;
	};

	virtual void Refresh() = 0;
	virtual const std::list<ILegacyAddons::Information> &GetList() const = 0;
};