#ifndef URIEvent_h
#define URIEvent_h

#include "Settings.h"

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

class URIEvent{
	public:
		typedef void (*URIEventCallback)(EthernetClient &client, char *path, char *queryParameterValues[]);
		
		URIEvent(const char *path, const char *queryParameterName, URIEventCallback callback);
		
		void addQueryParameter(const char *queryParameterName);
		
		bool triggerEvent(EthernetClient &client, char *currentPath, char *currentQueryParameterNames[], char *currentQueryParameterValues[]);
	private:
		const char *path;
		
		byte queryParametersCount;
		const char *queryParameterNames[URI_QUERY_PARAMETERS_MAX_COUNT];
		
		URIEventCallback callback;
};

#endif
