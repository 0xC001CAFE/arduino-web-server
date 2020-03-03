#include "URLEvent.h"

URLEvent::URLEvent(LogHandler &logHandler, const char *pathname, const char *searchParam, RequestEvent requestEvent): logHandler(logHandler){
	this->pathname = pathname;
	
	searchParams[0] = searchParam;
	searchParamsLength = 1;
	
	this->requestEvent = requestEvent;
}

bool URLEvent::addSearchParam(const char *searchParam){
	if(searchParamsLength < URL_SEARCH_PARAMS_MAX_COUNT){
		searchParams[searchParamsLength] = searchParam;
		searchParamsLength++;
		
		return true;
	}
	
	#if LOG_HANDLER_LEVEL > 1
	logHandler.log(LogHandler::WARNING, "too many search parameters were added to an URL event");
	#endif
	
	return false;
}

bool URLEvent::triggerEvent(EthernetClient &client, URL &url) const{
	if(strcmp(pathname, url.getPathname()) != 0) return false;
	
	if(searchParamsLength != url.getSearchParams().getLength()) return false;
	
	for(uint8_t i = 0; i < searchParamsLength; i++){
		if(url.getSearchParams().nameExists(searchParams[i]) == -1) return false;
	}
	
	requestEvent(client, url);
	
	return true;
}
