#pragma once

// NOTE: The Comment behind each function show which Lua hook it calls in the menu realm.
class AddonDownloadNotifications
{
public:
	virtual void Start( ) = 0; // Calls GM:WorkshopStart
	virtual void StartDownload( uint64_t wsid, uint64_t imgid, const char* title, unsigned long size ) = 0; // Calls GM:WorkshopDownloadFile
	virtual void FinishDownload( uint64_t wsid ) = 0; // Calls GM:WorkshopDownloadedFile
	virtual void Finish( ) = 0; // Calls GM:WorkshopEnd
	virtual void DownloadProgress( uint64_t wsid, uint64_t imgid, const char* title, unsigned int, unsigned int ) = 0; // Calls GM:WorkshopDownloadProgress
	virtual void ExtractProgress( uint64_t wsid, uint64_t imgid, const char* title, unsigned int percent ) = 0; // Calls GM:WorkshopExtractProgress
	virtual void DownloadTotals( int num, int max ) = 0; // Calls GM:WorkshopDownloadTotals
	virtual void SubscriptionsProgress( int num, int max ) = 0; // Calls GM:WorkshopSubscriptionsProgress
	virtual void SendMessage( char* message ) = 0; // Calls GM:WorkshopSubscriptionsMessage
	virtual void NotifySubscriptionChanges( ) = 0; // Calls GM:WorkshopSubscriptionsChanged
	virtual void NotifyAddonConflict( uint64_t wsid1, uint64_t wsid2, const char* filename ) = 0; // Calls GM:OnNotifyAddonConflict
};

class IAddonDownloadNotification : public AddonDownloadNotifications
{

};