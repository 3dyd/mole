#ifndef _LYRICS_SOURCE_BASE
#define _LYRICS_SOURCE_BASE

//Don't change this value
#define LYRICS3_SOURCE_SDK_VERSION 07

//If you want to create custom requirements, you must derive from here. See my_lyrics_source.cpp for an example of it's usage.
class search_requirements : public service_base
{
public:
	//! Internal use only
	int iSearchOrderPrefs;

	//! Internal use only
	bool bPreparedSuccessfully;

	FB2K_MAKE_SERVICE_INTERFACE_ENTRYPOINT( search_requirements );
};

//Contains the track info
struct search_info
{
	metadb_handle_ptr pTrack;
	const char* artist;
	const char* album;
	const char* title;
	const char* path;
	unsigned int subsong_index;
};

//Used in the load function to determine who the caller is
enum request_source
{
	normal = 1,
	search_dialog,
	batch_search
};

//! The interface for notifying the plugin of lyrics you've found.
//! You don't need to create an instance of this, just use what is passed into the respective methods.
//! See my_lyrics_source.cpp for more explanantion.
class lyric_result_client : public service_base
{
	FB2K_MAKE_SERVICE_INTERFACE_ENTRYPOINT( lyric_result_client );

public:
	//Returns an interface which you can fill out to create a result
	virtual lyric_container_base* AddResult() = 0;
};

//To create a lyric source, implement this service
class lyric_source_base : public service_base
{
protected:

public:
	int GetSDKVersion() { return LYRICS3_SOURCE_SDK_VERSION; }

	virtual void GetName( pfc::string_base& out ) { out = "<UNKNOWN>"; }

	virtual const GUID GetGUID() { return pfc::guid_null; }

	//! This is always called from main thread
	//! Most sources don't need to do much in here, only those which require the main thread to search.
	//! retrun true if successful, false if not
	//! @param pQuery - Provides information about the current search. artist, title etc.
	//! @param p_results - The client where you can add your lyric results. see above and in my_lyrics_source.cpp for more info.
	//! @param pRequirements - You don't have to use this. It's purpose is to allow to you to save some data (or whatever) so you can then use it in the Search() function. To use it you need to create your own search_requirements class, see my_lyrics_source.h and my_lyrics_source.cpp for an example.
	virtual bool PrepareSearch( const search_info* pQuery, lyric_result_client::ptr p_results, search_requirements::ptr& pRequirements ) { return false; }

	//! This will not be called from the main thread
	//! This is where you should do your search, if loading requires a separate internet request (or whatever) don't load the lyrics here, instead load them in Load().
	//! Don't recreate pRequirements, it's passing in the same instance you created in PrepareSearch()
	//! return true if successful, false if not
	//! @param pQuery - Provides information about the current search. artist, title etc.
	//! @param pRequirements - The same pRequirements that you created in PrepareSearch(). If you didn't create one, then the plugin will have created one. You do not need to release the requirements, the plugin will do this for you when it is finished with your lyric.
	//! @param p_results - The client where you can add your lyric results. see above and in my_lyrics_source.cpp for more info.
	virtual bool Search( const search_info* pQuery, search_requirements::ptr& pRequirements, lyric_result_client::ptr p_results  ) { return false; }

	//! This is where you should load your lyrics. You don't need to call this, it will be called if lyrics3 wants to display your lyric.
	//! You only need to implement this if the lyric is not loaded in Search()
	//! @param lyric - The lyric which you started to load in Search().
	virtual bool Load( lyric_container_base* lyric, request_source p_source ) { return false; }

	//! return true in your implementation if you want to be able to show a properties dialogue (in preferences)
	virtual bool HasProperties() { return false; }

	//! show your dialogue
	virtual void ShowProperties( HWND parent ) { return; }

	//! sets the http helper
	virtual void SetHttpClient( lyric_http_request::ptr p_client ) = 0;

	FB2K_MAKE_SERVICE_INTERFACE_ENTRYPOINT( lyric_source_base );
};

template<typename T>
class source_factory : public service_factory_t<T> {};

#endif

