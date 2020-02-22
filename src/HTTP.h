#ifndef HTTP_h
#define HTTP_h

#include "Settings.h"
#include "StringFunctions.h"

#include <stdint.h>
#include <string.h>
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

class HTTP{
	public:
		enum RequestStatus{
			BUFFER_OVERFLOW,
			INVALID_HTTP_VERSION,
			INVALID_REQUEST_METHOD,
			GET_REQUEST
		};
		
		enum ContentType{
			PLAIN,
			HTML
		};
		
		static bool getRequestLine(EthernetClient &client, char *requestBuffer, uint8_t &requestBufferLength, RequestStatus &status);
		
		static bool validRequestURL(char *requestLine, char *&requestURL);
		
		static bool supportsFileExtension(char *fileExtension, ContentType &type);
		
		static void ok(EthernetClient &client, ContentType type);
		static void movedPermanently(EthernetClient &client, IPAddress ip, const char *pathname, const char *pathnameSuffix = 0);
	private:
		static const char *contentTypes[];
		
		static const char *fileExtensions[];
};

#endif
