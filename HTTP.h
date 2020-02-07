#ifndef HTTP_h
#define HTTP_h

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
		
		static bool getRequest(char *requestBuffer, byte &requestBufferLength, EthernetClient &client, RequestMethod &requestMethod);
		static char* getRequestURI(char *requestBuffer);
		
		static void ok(File &file, EthernetClient &client);
		static void notFound(EthernetClient &client);
		static void notImplemented(EthernetClient &client);
};

#endif
