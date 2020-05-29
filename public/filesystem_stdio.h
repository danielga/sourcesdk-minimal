#ifndef FILESYSTEM_STDIO_H
#define FILESYSTEM_STDIO_H

#if defined( _WIN32 )
#pragma once
#endif

#include "filesystem_base.h"

class CFileSystem_Stdio : public CBaseFileSystem
{
public:
	// Used to get at older versions
	virtual void *QueryInterface( const char *pInterfaceName ) = 0;

	// Higher level filesystem methods requiring specific behavior
	virtual void GetLocalCopy( const char *pFileName ) = 0;
	virtual int	HintResourceNeed( const char *hintlist, int forgetEverything ) = 0;
	virtual bool IsFileImmediatelyAvailable( const char *pFileName ) = 0;
	virtual WaitForResourcesHandle_t WaitForResources( const char *resourcelist ) = 0;
	virtual bool GetWaitForResourcesProgress( WaitForResourcesHandle_t handle, float *progress /* out */, bool *complete /* out */ ) = 0;
	virtual void CancelWaitForResources( WaitForResourcesHandle_t handle ) = 0;
	virtual bool IsSteam( ) const = 0;
	virtual	FilesystemMountRetval_t MountSteamContent( int nExtraAppId = -1 ) = 0;

	virtual bool GetOptimalIOConstraints( FileHandle_t hFile, unsigned *pOffsetAlign, unsigned *pSizeAlign, unsigned *pBufferAlign ) = 0;
	virtual void *AllocOptimalReadBuffer( FileHandle_t hFile, unsigned nSize, unsigned nOffset ) = 0;
	virtual void FreeOptimalReadBuffer( void *p ) = 0;

protected:
	// implementation of CBaseFileSystem virtual functions
	virtual FILE *FS_fopen( const char *filename, const char *options, unsigned flags, int64 *size ) = 0;
	virtual void FS_setbufsize( FILE *fp, unsigned nBytes ) = 0;
	virtual void FS_fclose( FILE *fp ) = 0;
	virtual void FS_fseek( FILE *fp, int64 pos, int seekType ) = 0;
	virtual long FS_ftell( FILE *fp ) = 0;
	virtual int FS_feof( FILE *fp ) = 0;
	virtual size_t FS_fread( void *dest, size_t destSize, size_t size, FILE *fp ) = 0;
	virtual size_t FS_fwrite( const void *src, size_t size, FILE *fp ) = 0;
	virtual bool FS_setmode( FILE *fp, FileMode_t mode ) = 0;
	virtual size_t FS_vfprintf( FILE *fp, const char *fmt, va_list list ) = 0;
	virtual int FS_ferror( FILE *fp ) = 0;
	virtual int FS_fflush( FILE *fp ) = 0;
	virtual char *FS_fgets( char *dest, int destSize, FILE *fp ) = 0;
	virtual int FS_stat( const char *path, struct _stat *buf ) = 0;
	virtual int FS_chmod( const char *path, int pmode ) = 0;
	virtual HANDLE FS_FindFirstFile( const char *findname, FIND_DATA *dat ) = 0;
	virtual bool FS_FindNextFile( HANDLE handle, FIND_DATA *dat ) = 0;
	virtual bool FS_FindClose( HANDLE handle ) = 0;
	virtual int FS_GetSectorSize( FILE * ) = 0;

private:
	bool m_bMounted;
	bool m_bCanAsync;
};

#endif
