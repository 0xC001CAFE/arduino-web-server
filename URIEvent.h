#ifndef URIEvent_h
#define URIEvent_h

#include "Settings.h"

#include <Arduino.h>

class URIEvent{
	public:
		typedef void (*Callback)();
		
		URIEvent(const char *path, const char *queryParameterName, Callback callback);
		
		void addQueryParameter(const char *queryParameterName);
		
		bool triggerEvent(char *currentPath, char *currentQueryParameterNames[]);
	private:
		const char *path;
		
		byte queryParametersCount;
		const char *queryParameterNames[URI_QUERY_PARAMETERS_MAX_COUNT];
		
		Callback callback;
};

#endif
