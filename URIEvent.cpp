#include "URIEvent.h"

URIEvent::URIEvent(const char *path, const char *queryParameterName, URIEventCallback callback){
	this->path = path;
	
	queryParametersCount = 1;
	queryParameterNames[0] = queryParameterName;
	
	this->callback = callback;
}

void URIEvent::addQueryParameter(const char *queryParameterName){
	if(queryParametersCount < URI_QUERY_PARAMETERS_MAX_COUNT){
		queryParameterNames[queryParametersCount] = queryParameterName;
		queryParametersCount++;
		
		return;
	}
	
	#if LOGGING_OUTPUT > 1
	Serial.println("[WebServer] -> Warning: too many query parameters have been added to an URI event");
	#endif
}

bool URIEvent::triggerEvent(EthernetClient &client, char *currentPath, char *currentQueryParameterNames[], char *currentQueryParameterValues[]){
	if(!strcmp(path, currentPath)){
		for(byte i = 0; i < queryParametersCount; i++){
			if(strcmp(queryParameterNames[i], currentQueryParameterNames[i])) return false;
		}
		
		callback(client, currentPath, currentQueryParameterValues);
		
		return true;
	}
	
	return false;
}
