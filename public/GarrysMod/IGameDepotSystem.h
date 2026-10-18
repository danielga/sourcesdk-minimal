#pragma once

#include <cstdint>
#include <string>
#include <list>

class IGameDepotSystem
{
public:
	struct Information
	{
		uint32_t appid;
		std::string title;
		std::string folder;
		bool mounted;
		bool enabled;
		bool owned;
		bool installed;
		// RaphaelIT7:
		// I am unsure of these two below but we leave them as the struct size fits good enouth for GMod to accept.
		bool retail;
		bool bundled;
		std::list<std::string> mountSubDirs{};
	};

	virtual void Refresh() = 0;
	virtual void Clear() = 0;
	virtual void Save() = 0;
	virtual void SetMount( uint32_t nAppID, bool bMounted ) = 0;
	virtual void MarkGameAsMounted( const std::string strGameFolder ) = 0;
	virtual const std::list<IGameDepotSystem::Information> &GetList() = 0;
	virtual int GetRefreshCount() = 0;
};