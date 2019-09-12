#ifndef __LYRIC_CONTAINER_BASE_H
#define __LYRIC_CONTAINER_BASE_H

enum LYRIC_SOURCE_TYPE
{
	ST_TAG, 

	//! internal use only
	ST_ASSOC,

	ST_FILE, 
	ST_INTERNET, 
	ST_UNKNOWN
};

enum LYRIC_RESULT_QUALITY
{
  Q_UNKNOWN, Q_BAD, Q_MEDIUM, Q_GOOD, Q_EXCELLENT
};

class lyric_container_base
{
public:
	//! Sets the basic information about the lyric found.
	//! @param p_artist - The artist of the lyric you've found (you shouldn't just copy in from search_info, use what your source has returned).
	//! @param p_album - The album of the lyric you've found (you shouldn't just copy in from search_info, use what your source has returned).
	//! @param p_title - The title of the lyric you've found (you shouldn't just copy in from search_info, use what your source has returned).
	virtual void SetFoundInfo( const char* p_artist, const char* p_album, const char* p_title ) = 0;

	//! Sets the source information about the lyric.
	//! @param p_source - This string is displayed to the user in both the lyric search dialogue and about current lyrics dialgue
	//! @param p_source_private - This is never displayed to the user, so if you don't want to display the full link to the lyrics use this.
	//! @param p_guid - the guid of your source
	//! @param p_source_type - Indicates where the lyric has come from. Most of the time this is ST_INTERNET.
	virtual void SetSources( const char* p_source, const char* p_source_private, GUID p_guid, LYRIC_SOURCE_TYPE p_source_type ) = 0;
	
	//! Call this if you know whether or not your lyric will be timestamped BEFORE it is loaded. This allows the plugin to choose a suitable lyric more quickly.
	//! It will do nothing if the lyric is already loaded.
	virtual void SetTimestamped( bool p_is_timestamped ) = 0;

	//! Sets the lyric text to display in the panel. You must remove all xml formatting etc - the string should only contain the lyric text and timestamps
	virtual void SetLyric( const char* p_lyric_text ) = 0;

	//! Tells the panel that the lyric is loaded and can be displayed. automatically called in SetLyric().
	virtual void SetLoaded( bool p_isloaded = true ) = 0;

	//! Tells the plugin what quality the lyric is. If your provider doesn't give rating/quality info, ignore this method.
	virtual void SetQuality( LYRIC_RESULT_QUALITY p_quality ) = 0;

	//! Use this to retrieve the links you saved in Search() to load your lyrics.
	virtual void GetSources( pfc::string_base& p_source, pfc::string_base& p_source_private ) = 0;

	//! Completely clears all the previously set lyric info.
	virtual void Reset() = 0;

	//! Called automatically in SetLyric(). Call this to check if your lyric is timestamped or not.
	virtual bool IsTimestamped() = 0;
};

#endif