#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <list>

class CSteamAPIContext;
class IAddonDownloadNotification;
struct SteamUGCDetails_t;

namespace Addon
{
	class FileSystem;
	namespace Job
	{
		class Base
		{
		public:
			virtual ~Base() = default;
			virtual void Start() = 0;
			virtual void Cycle() = 0;
			virtual bool Finished() = 0;
			virtual void Init( Addon::FileSystem* pAddonSystem ) { m_pAddonSystem = pAddonSystem; }

		protected:
			Addon::FileSystem* m_pAddonSystem;
		};
	}
}

class IAddonSystem
{
public:
	struct Information
	{
		std::string title;
		std::string file;
		std::string tags;
		std::string failure; // only set when the addon failed to mount?
		uint32_t time_updated;
		uint32_t model_count;
		uint64_t wsid;
		uint64_t creator;
		uint64_t hcontent_file;
		uint64_t size;
		uint64_t hcontent_preview;
		uint32_t timeadded;
		bool canUpdate;
		bool downloaded;
	};

	struct UGCInfo
	{
		std::string title;
		std::string file;
		std::string placeholder1;
		uint64_t wsid;
		uint64_t creator;
		uint32_t pubdate;
	};

	struct AddonSource
	{
		AddonSource( int unknown ) : m_UnknownValue(unknown)
		{
		
		}

		bool operator==(const AddonSource& other) const
		{
			return m_UnknownValue == other.m_UnknownValue;
		}

		int m_UnknownValue;
	}; // unknown

	virtual void Clear() = 0;
	virtual void Refresh() = 0;
	virtual bool MountFile( const std::string &gmaPath, std::vector<std::string> *files, uint64_t wsid, uint64_t wsid2, IAddonSystem::AddonSource source ) = 0;
	virtual bool ShouldMount( uint64_t wsid ) = 0;
	virtual void SetShouldMount( uint64_t wsid, bool bShouldMount ) = 0;
	virtual void Save() = 0;
	virtual const std::list<IAddonSystem::Information> &GetList() const = 0;
	virtual const std::list<IAddonSystem::UGCInfo> &GetUGCList() const = 0;
	virtual void ScanForSubscriptions( const char *unknown1, bool unknown2 ) = 0;
	virtual void Think() = 0;
	virtual void SetDownloadNotify( IAddonDownloadNotification *pDownloadNotify ) = 0;
	virtual IAddonDownloadNotification *Notify() = 0;
	virtual bool IsSubscribed( uint64_t wsid ) = 0;
	virtual const IAddonSystem::Information *FindFileOwner( const std::string &strFileName ) = 0;
	virtual void AddAddon( const IAddonSystem::Information &info ) = 0;
	virtual void ClearUnusedGMAs() = 0;
	virtual std::string GetAddonFilepath( uint64_t wsid, bool bGMAOnly ) = 0;
	virtual void UnmountAddon( uint64_t wsid, const char *pszReason ) = 0;
	virtual void UnmountServerAddons() = 0;
	virtual std::string IsAddonValidPreInstall( SteamUGCDetails_t details ) = 0;
	virtual bool AllJobsFinished() = 0;
	virtual void Shutdown() = 0;
	virtual void AddJob( Addon::Job::Base *job ) = 0;
	virtual const std::list<SteamUGCDetails_t> &GetSubList() const = 0;
	virtual void MountFloatingAddons() = 0;
	virtual void AddAddonFromSteamDetails( const SteamUGCDetails_t &details ) = 0;
	virtual void OnAddonSubscribed( const SteamUGCDetails_t &details ) = 0;
	virtual void AddUnloadedSubscription( uint64_t wsid ) = 0;
	virtual void EnableLoadingUnloadedAddons() = 0;
	virtual bool HasChanges() = 0;
	virtual void MarkChanged() = 0;
	virtual void OnAddonDownloaded( const IAddonSystem::Information &info ) = 0;
	virtual void OnAddonDownloadFailed( const IAddonSystem::Information &info ) = 0;
	virtual void Load() = 0;
};