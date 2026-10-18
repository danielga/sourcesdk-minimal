#pragma once

#include <cstdint>

class IGModLanguage
{
public:
	// If bForceReload = true then it won't tell Lua about it
	virtual void ChangeLanguage( const char *pszLangCode, bool bForceReload = false ) = 0;
	// Just a wrapper to translate the steam language key into one recognized by GMod -> calls if found ChangeLanguage
	virtual void ChangeLanguage_Steam( const char *pszLanguage ) = 0;
	virtual void ReloadLanguage() = 0;
	virtual bool GetString( const char *pszPhraseKey, wchar_t *pszPhraseOut, uint32_t nPhraseOutLength ) = 0;
	virtual void UpdateSourceEngineLanguage() = 0;
};
