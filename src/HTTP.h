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
			HTML,
			CSS,
			JAVASCRIPT
		};
		
		static bool getRequestLine(EthernetClient &client, char *requestBuffer, uint8_t &requestBufferLength, RequestStatus &status);
		
		static bool validRequestURL(char *requestLine, char *&requestURL);
		
		static bool supportsFileExtension(char *fileExtension, ContentType &type);
		
		// successful 2xx
		static void ok(EthernetClient &client, ContentType type);
		// redirection 3xx
		static void movedPermanently(EthernetClient &client, IPAddress ip, const char *pathname, const char *pathnameSuffix = 0);
		// client error 4xx
		static void badRequest(EthernetClient &client);
		static void notFound(EthernetClient &client);
		static void requestTimeout(EthernetClient &client);
		static void requestURITooLong(EthernetClient &client);
		static void unsupportedMediaType(EthernetClient &client);
		// server error 5xx
		static void internalServerError(EthernetClient &client);
		static void notImplemented(EthernetClient &client);
		static void httpVersionNotSupported(EthernetClient &client);
	private:
		static const char *contentTypes[];
		
		static const char *fileExtensions[];
};

#endif
