#include "URL.h"

URL::URL(char *url): searchParams(URL_SEARCH_PARAMS_MAX_COUNT){
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
	
	if(!search) return;
	
	char *paramPairs[URL_SEARCH_PARAMS_MAX_COUNT] = { 0 };
	
	paramPairs[0] = strtok(search, "&");
	for(uint8_t i = 1; i < URL_SEARCH_PARAMS_MAX_COUNT && paramPairs[i - 1]; i++){
		paramPairs[i] = strtok(0, "&");
	}
	
	for(uint8_t i = 0; i < URL_SEARCH_PARAMS_MAX_COUNT && paramPairs[i]; i++){
		char *name = strtok(paramPairs[i], "=");
		char *value = strtok(0, "=");
		
		searchParams.addParam(name, value);
	}
}
