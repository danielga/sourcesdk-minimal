#include <stdint.h>
#include <refcount.h>
#include <utlsymbol.h>
#include <utlstring.h>
#include <utllinkedlist.h>
#include <utldict.h>
#include <tier1.h>
#include <byteswap.h>
#include <UtlSortVector.h>
#include <filesystem.h>
#include <GarrysMod/Addon.h>
#include <GarrysMod/LegacyAddons.h>
#include <GarrysMod/Gamemode.h>
#include <GarrysMod/GameDepot.h>
#include <GarrysMod/Language.h>

#undef GetCurrentDirectory
#undef AsyncRead

class CPackFile;
class CBaseFileSystem;
class IGet;
struct FIND_DATA;

enum FileMode_t
{
	FM_BINARY,
	FM_TEXT
};

enum FileType_t
{
	FT_NORMAL,
	FT_PACK_BINARY,
	FT_PACK_TEXT
};

union Placeholder4
{
	const uint8_t bytes[4];
	const char *string;
	uint32_t integer;
	void *pointer;
};

template<class T> class CThreadSafeRefCountedObject
{
public:
	CThreadSafeRefCountedObject( T initVal )
	{
		m_RefCount = 0;
		m_pObject = initVal;
		m_RefCount = 0;
	}

	void Init( T pObj )
	{
		Assert( ThreadInMainThread() );
		Assert( !m_pObject );
		m_RefCount = 0;
		m_pObject = pObj;
		m_RefCount = 1;
	}

	T AddRef()
	{
		if ( ++m_RefCount > 1 )
			return m_pObject;

		--m_RefCount;
		return nullptr;
	}
	void ReleaseRef( T pObj )
	{
		if ( --m_RefCount >= 1 )
			Assert( m_pObject == pObj );
	}

	T GetInMainThread()
	{
		Assert( ThreadInMainThread() );
		return m_pObject;
	}

	void ResetWhenNoRemainingReferences( T newValue )
	{
		Assert( ThreadInMainThread() );

		while ( m_RefCount > 0 )
			CThread::Sleep( 20 );

		m_pObject = newValue;
	}

private:
	CInterlockedIntT<long> m_RefCount;
	T m_pObject;
};

class CPackFileHandle
{
public:
	int64_t m_nBase;
	uint32_t m_nFilePointer;
	CPackFile *m_pOwner;
	uint32_t m_nLength;
	uint32_t m_nIndex;
};

class CFileHandle
{
public:
#if !defined( _RETAIL )
	char *m_pszTrueFileName;
#endif

	CPackFileHandle *m_pPackFileHandle;
	int64_t m_nLength;
	FileType_t m_type;
	FILE *m_pFile;
	CBaseFileSystem *m_fs;

	enum
	{
		MAGIC = 1128679521, // 'CFHa'
		FREE_MAGIC = 1181902157 // 'FreM'
	};

	uint32_t m_nMagic;
};

class CPackFile : public CRefCounted<CRefCountServiceMT>
{
public:
	virtual ~CPackFile( ) = 0;
	virtual CFileHandle *OpenFile( const char *pFileName, const char *pOptions = "rb" ) = 0;
	virtual bool Prepare( int64_t fileLen = -1, int64_t nFileOfs = 0 ) = 0;
	virtual bool FindFile( const char *pFilename, int32_t &nIndex, int64_t &nPosition, int32_t &nLength ) = 0;
	virtual int32_t ReadFromPack( int32_t nIndex, void *buffer, int32_t nDestBytes, int32_t nBytes, int64_t nOffset ) = 0;
	virtual bool IndexToFilename( int32_t nIndex, char *buffer, int32_t nBufferSize ) = 0;
	virtual void SetupPreloadData( ) = 0;
	virtual void DiscardPreloadData( ) = 0;
	virtual int64_t GetPackFileBaseOffset( ) = 0;

	CThreadFastMutex m_mutex; // 8b
	CUtlSymbol m_Path; // 4b
	int64_t m_nBaseOffset; // 8b
	CUtlString m_ZipName; // 4b
	bool m_bIsMapPath; // 4b
	int32_t m_lPackFileTime; // 4b
	int32_t m_refCount; // 4b
	int32_t m_nOpenFiles; // 4b
	FILE *m_hPackFileHandle; // 4b
	int64_t m_FileLength; // 8b
	CBaseFileSystem *m_fs;  // 4b
};

class CZipPackFile : public CPackFile
{
public:
	class CPackFileEntry
	{
	public:
		uint32_t m_nPosition;
		uint32_t m_nLength;
		uint32_t m_HashName;
		uint16_t m_nPreloadIdx;
		uint16_t pad;
#if !defined( _RETAIL )
		FileNameHandle_t m_hDebugFilename;
#endif
	};

	class CPackFileLessFunc
	{
	public:
		bool Less( const CPackFileEntry &src1, const CPackFileEntry &src2, void *pCtx );
	};

	CUtlSortVector<CPackFileEntry, CPackFileLessFunc> m_PackFiles; // 28b
	int64_t m_nPreloadSectionOffset; // 8b
	uint32_t m_nPreloadSectionSize; // 4b
	struct ZIP_PreloadHeader *m_pPreloadHeader; // 4b
	uint16_t *m_pPreloadRemapTable; // 4b
	struct ZIP_PreloadDirectoryEntry *m_pPreloadDirectory; // 4b
	void *m_pPreloadData; // 4b
	CByteswap m_swap; // 4b

	Placeholder4 placeholders[13];
};

class WilloxHallOfShame
{
public:
	void **vtable;
	uint32_t refcounter;
	uint32_t filepathid;
	const char filepath[1];
};

class CFileInfo;

class CPathIDFileList
{
public:
	CUtlSymbol m_PathID;
	CUtlDict<CFileInfo *, int32_t> m_Files;
	CUtlLinkedList<CFileInfo *, int32_t>	m_UnverifiedCRCFiles;
};

class CFileInfo
{
public:
	uint16_t m_Flags;
	CRC32_t m_CRC;
	CPathIDFileList	*m_pPathIDFileList;
	int32_t m_PathIDFileListDictIndex;
	int32_t m_iNeedsVerificationListIndex;
};

class CFileTracker
{
	CUtlLinkedList<CFileInfo *> m_NeedsVerificationList;
	CUtlDict<CPathIDFileList *, int32_t> m_PathIDs;
	CBaseFileSystem *m_pFileSystem;
	CThreadMutex m_Mutex;
};

class CWhitelistSpecs
{
public:
	IFileList *m_pWantCRCList;
	IFileList *m_pAllowFromDiskList;
};
typedef CThreadSafeRefCountedObject<CWhitelistSpecs *> CWhitelistHolder;

class CBaseFileSystem : public CTier1AppSystem<IFileSystem>
{
public:
	class CPathIDInfo
	{
	public:
		bool m_bByRequestOnly;
		CUtlSymbol m_PathID;
		const char *m_pDebugPathID;
	};

	class CSearchPath
	{
	public:
		int32_t m_storeId;
		CPathIDInfo *m_pPathIDInfo;
		uint32_t enum1;
		uint32_t enum2;
		CUtlSymbol m_Path;
		const char *m_pDebugPath;
		bool m_bIsRemotePath;
		WilloxHallOfShame *m_pPackFile;
	};

	struct FindData_t
	{ };

	struct CompiledKeyValuesPreloaders_t
	{
		FileNameHandle_t m_CacheFile;
		class CCompiledKeyValuesReader *m_pReader;
	};

	class COpenedFile
	{
	public:
		FILE *m_pFile;
		char *m_pName;
	};

	CWhitelistHolder m_FileWhitelist;
	int32_t m_WhitelistSpewFlags;
	CUtlVector<FileSystemLoggingFunc_t> m_LogFuncs;
	CThreadMutex m_SearchPathsMutex;
	CUtlLinkedList<CSearchPath> m_SearchPaths;
	CUtlVector<CPathIDInfo *> m_PathIDInfos;
	CUtlLinkedList<FindData_t> m_FindData; // DO NOT USE AT ALL!
	int32_t m_iMapLoad;
	CUtlVector<CPackFile *> m_ZipFiles;
	FILE *m_pLogFile;
	bool m_bOutputDebugString;
	IThreadPool *m_pThreadPool;
	CThreadFastMutex m_AsyncCallbackMutex;
	FileSystemStatistics m_Stats;
	//CUtlRBTree<COpenedFile, int> m_OpenedFiles;
	CThreadMutex m_OpenedFilesMutex;
	CUtlVector<COpenedFile> m_OpenedFiles;
	FileWarningLevel_t m_fwLevel;
	CUtlFilenameSymbolTable m_FileNames;
	CFileTracker m_FileTracker;
	int32_t m_WhitelistFileTrackingEnabled;
	FSDirtyDiskReportFunc_t m_DirtyDiskReportFunc;
	CompiledKeyValuesPreloaders_t m_PreloadData[IFileSystem::NUM_PRELOAD_TYPES];
};

class CFileSystem_Stdio : public CBaseFileSystem
{
public:
	virtual void RemoveSearchPathsByGroup( int ) = 0;
	virtual void SetGet( IGet * ) = 0;
	virtual Addon::FileSystem *Addons( ) = 0;
	virtual Gamemode::System *Gamemodes( ) = 0;
	virtual GameDepot::System *Games( ) = 0;
	virtual LegacyAddons::System *LegacyAddons( ) = 0;
	virtual CLanguage *Language( ) = 0;
	virtual void DoFilesystemRefresh( ) = 0;
	virtual int LastFilesystemRefresh( ) = 0;
	virtual void AddVPKFileFromPath( const char *, const char *, unsigned int ) = 0;
	virtual void GMOD_SetupDefaultPaths( const char *, const char * ) = 0;
	virtual void GMOD_FixPathCase( char *, uint ) = 0;
	virtual FileHandle_t Open( const char *pFileName, const char *pOptions, const char *pathID = nullptr ) = 0;
	virtual void Close( FileHandle_t file ) = 0;
	virtual void Seek( FileHandle_t file, int pos, FileSystemSeek_t seekType ) = 0;
	virtual unsigned int Tell( FileHandle_t file ) = 0;
	virtual unsigned int Size( FileHandle_t file ) = 0;
	virtual unsigned int Size( const char *pFileName, const char *pPathID = nullptr ) = 0;
	virtual void Flush( FileHandle_t file ) = 0;
	virtual bool Precache( const char *pFileName, const char *pPathID = nullptr ) = 0;
	virtual int Read( void* pOutput, int size, FileHandle_t file ) = 0;
	virtual int Write( void const* pInput, int size, FileHandle_t file ) = 0;
	virtual bool ReadFile( const char *pFileName, const char *pPath, CUtlBuffer &buf, int nMaxBytes = 0, int nStartingByte = 0, FSAllocFunc_t pfnAlloc = nullptr ) = 0;
	virtual bool WriteFile( const char *pFileName, const char *pPath, CUtlBuffer &buf ) = 0;
	virtual bool UnzipFile( const char *pFileName, const char *pPath, const char *pDestination ) = 0;
	virtual bool FileExists( const char *pFileName, const char *pPathID = nullptr ) = 0;
	virtual long GetFileTime( const char *pFileName, const char *pPathID = nullptr ) = 0;
	virtual bool IsFileWritable( char const *pFileName, const char *pPathID = nullptr ) = 0;
	virtual bool SetFileWritable( char const *pFileName, bool writable, const char *pPathID = nullptr ) = 0;
	virtual void FixUpPath( const char *, char *, int ) = 0;
	virtual FILE *FS_fopen( const char *, const char *, unsigned int, long long * ) = 0;
	virtual void FS_setbufsize( FILE *, unsigned int ) = 0;
	virtual void FS_fclose( FILE * ) = 0;
	virtual void FS_fseek( FILE *, long long, int ) = 0;
	virtual long FS_ftell( FILE * ) = 0;
	virtual int FS_feof( FILE * ) = 0;
	virtual void FS_fread( void *, unsigned long, unsigned long, FILE * ) = 0;
	virtual void FS_fwrite( const void *, unsigned long, FILE * ) = 0;
	virtual bool FS_setmode( FILE *, FileMode_t ) = 0;
	virtual size_t FS_vfprintf( FILE *, const char *, char * ) = 0;
	virtual int FS_ferror( FILE * ) = 0;
	virtual int FS_fflush( FILE * ) = 0;
	virtual char *FS_fgets( char *, int, FILE * ) = 0;
	virtual int FS_stat( const char *, struct stat *, bool * ) = 0;
	virtual int FS_chmod( const char *, int ) = 0;
	virtual HANDLE FS_FindFirstFile( const char *, FIND_DATA * ) = 0;
	virtual bool FS_FindNextFile( HANDLE, FIND_DATA * ) = 0;
	virtual bool FS_FindClose( HANDLE ) = 0;
	virtual int FS_GetSectorSize( FILE * ) = 0;

	bool m_bMounted;
	bool m_bCanAsync;
};
