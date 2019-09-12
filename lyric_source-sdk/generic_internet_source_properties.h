#ifndef _GENERIC_INTERNET_SOURCE_PROPERTIES_H_
#define _GENERIC_INTERNET_SOURCE_PROPERTIES_H_

//A service which displays the generic internet properties dialogue.
class generic_internet_source_properties : public service_base
{
public:

	//Displays the generic internet source properties.
	virtual void run( HWND parent ) = 0;

	FB2K_MAKE_SERVICE_INTERFACE_ENTRYPOINT( generic_internet_source_properties );
};

#endif

