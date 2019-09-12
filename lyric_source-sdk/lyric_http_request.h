#ifndef _lyric_http_request_H
#define _lyric_http_request_H

//A helper for doing internet requests. This uses the internet settings the user has set in lyrics3 preferences.
class lyric_http_request : public service_base
{
	FB2K_MAKE_SERVICE_INTERFACE_ENTRYPOINT( lyric_http_request );
public:

	virtual bool do_post_request( pfc::string_base& output, const char* userAgent, const char* host, WORD port, const char* request, const char* postdata, unsigned int post_size ) = 0;
	virtual bool do_get_request( pfc::string_base& output, const char* userAgent, const char* host, WORD port, const char* request ) = 0;

	virtual bool download_page( pfc::string_base& output, const char* userAgent, const char* url ) = 0;
};

#endif

