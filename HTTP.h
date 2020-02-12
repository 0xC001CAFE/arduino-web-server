#ifndef HTTP_h
#define HTTP_h

#include "Settings.h"

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

class HTTP{
	public:
		enum RequestMethod{
			INVALID,
			GET
		};
		
		static bool getRequest(EthernetClient &client, char *requestBuffer, byte &requestBufferLength, RequestMethod &requestMethod);
		static char* getRequestURI(char *requestBuffer);
		
		static bool getURIQueryParameters(char *query, char *parameterNames[], char *parameterValues[]);
		
		static void ok(EthernetClient &client, File &file);
		static void notFound(EthernetClient &client);
		static void notImplemented(EthernetClient &client);
};

#endif
