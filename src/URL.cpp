#include "URL.h"

URL::URL(LogHandler &logHandler, char *url): logHandler(logHandler), searchParams(URL_SEARCH_PARAMS_MAX_COUNT){
	identifyComponents(url);
}

char* URL::getPathname() const{
	return pathname;
}

const Params& URL::getSearchParams() const{
	return searchParams;
}

void URL::identifyComponents(char *url){
	pathname = strtok(url, "?");
	char *search = strtok(0, "?");
	
	if(!search){
		#if LOG_HANDLER_LEVEL > 2
		logHandler.log(LogHandler::INFO, "no search was found");
		#endif
		
		return;
	}
	
	char *paramPairs[URL_SEARCH_PARAMS_MAX_COUNT] = { 0 };
	
	paramPairs[0] = strtok(search, "&");
	for(uint8_t i = 1; i < URL_SEARCH_PARAMS_MAX_COUNT && paramPairs[i - 1]; i++){
		paramPairs[i] = strtok(0, "&");
		
		#if LOG_HANDLER_LEVEL > 1
		if(i == (URL_SEARCH_PARAMS_MAX_COUNT - 1) && paramPairs[i] && strtok(0, "&")){
			logHandler.log(LogHandler::WARNING, "not enough memory available for all search parameters");
		}
		#endif
	}
	
	for(uint8_t i = 0; i < URL_SEARCH_PARAMS_MAX_COUNT && paramPairs[i]; i++){
		char *name = strtok(paramPairs[i], "=");
		char *value = strtok(0, "=");
		
		if(searchParams.addParam(name, value)){
			#if LOG_HANDLER_LEVEL > 2
			logHandler.log(LogHandler::INFO, "search parameter \"%s\" was added -> value: \"%s\"", name, value);
			#endif
		}
	}
}
