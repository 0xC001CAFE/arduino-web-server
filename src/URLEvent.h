#ifndef URLEvent_h
#define URLEvent_h

#include "Settings.h"
#include "arduino-log-handler/LogHandler.h"
#include "RequestEvent.h"
#include "URL.h"

#include <stdint.h>
#include <string.h>
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

class URLEvent{
	public:
		URLEvent(LogHandler &logHandler, const char *pathname, const char *searchParam, RequestEvent requestEvent);
		
		bool addSearchParam(const char *searchParam);
		
		bool triggerEvent(EthernetClient &client, URL &url) const;
	private:
		LogHandler &logHandler;
		
		const char *pathname;
		
		const char *searchParams[URL_SEARCH_PARAMS_MAX_COUNT];
		uint8_t searchParamsLength;
		
		RequestEvent requestEvent;
};

#endif
