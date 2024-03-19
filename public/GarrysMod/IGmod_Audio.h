#pragma once


#define INTERFACEVERSION_GMODAUDIO			"IGModAudio001"

enum GModChannelFFT_t {
	FFT_256 = 0,
	FFT_512 = 1,
	FFT_1024 = 2,
	FFT_2048 = 3,
	FFT_4096 = 4,
	FFT_8192 = 5,
	FFT_16384 = 6,
	FFT_32768 = 7,
};

class IGModAudioChannel
{
public:
	virtual void Destroy() = 0;
	virtual void Stop() = 0;
	virtual void Pause() = 0;
	virtual void Play() = 0;
	virtual void SetVolume(float) = 0;
	virtual float GetVolume() = 0;
	virtual void SetPlaybackRate(float) = 0;
	virtual float GetPlaybackRate() = 0;
	virtual void SetPos( Vector*, Vector*, Vector* ) = 0;
	virtual void GetPos( Vector*, Vector*, Vector* ) = 0;
	virtual void SetTime( double, bool ) = 0;
	virtual double GetTime() = 0;
	virtual void Set3DFadeDistance( float, float ) = 0;
	virtual void Get3DFadeDistace( float, float ) = 0;
	virtual void Set3DCone( int, int, float ) = 0;
	virtual void Get3DCone( int*, int*, float* ) = 0;
	virtual bool GetState() = 0;
	virtual void SetLooping( bool ) = 0;
	virtual bool IsLooping() = 0;
	virtual bool IsOnline() = 0;
	virtual bool Is3D() = 0;
	virtual bool IsBlockStreamed() = 0;
	virtual bool IsValid() = 0;
	virtual double GetLength() = 0;
	virtual const char* GetFileName() = 0;
	virtual int GetSamplingRate() = 0;
	virtual int GetBitsPerSample() = 0;
	virtual int GetAverageBitRate() = 0;
	virtual void GetLevel( float*, float* ) = 0;
	virtual void FFT( float*, GModChannelFFT_t ) = 0;
	virtual void SetChannelPan( float ) = 0;
	virtual float GetChannelPan() = 0;
	virtual void GetTags( int ) = 0;
	virtual void Set3DEnabled( bool ) = 0;
	virtual bool Get3DEnabled() = 0;
	virtual void Restart() = 0;
};

class IAudioStreamEvent;
class HSTREAM;
class CALLBACK;

abstract_class IBassAudioStream
{
public:
	virtual void Decode( void*, uint ) = 0;
	virtual int GetOutputBits() = 0;
	virtual int GetOutputRate() = 0;
	virtual int GetOutputChannels() = 0;
	virtual uint GetPosition() = 0;
	virtual void SetPosition( uint distance ) = 0;
	virtual HSTREAM GetHandle() = 0;
	virtual CALLBACK MyFileCloseProc( void* ) = 0;
	virtual CALLBACK MyFileLenProc( void* ) = 0;
	virtual CALLBACK MyFileReadProc( void*, uint, void* ) = 0;
	virtual CALLBACK MyFileSeekProc( uint64_t, void* ) = 0;
};

abstract_class IGMod_Audio
{
public:
	virtual int Init( CreateInterfaceFn ) = 0;
	virtual void Shutdown() = 0;
	virtual void Update( unsigned int ) = 0;
	virtual IBassAudioStream* CreateAudioStream( IAudioStreamEvent* ) = 0;
	virtual void SetEar( Vector*, Vector*, Vector*, Vector* ) = 0;
	virtual void PlayURL( const char* url, const char* flags, int* ) = 0;
	virtual void PlayFile( const char* path, const char* flags, int* ) = 0;
	virtual void SetGlobalVolume( float ) = 0;
	virtual void StopAllPlayback() = 0;
	virtual const char* GetErrorString( int ) = 0;
};