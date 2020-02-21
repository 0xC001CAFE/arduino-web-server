#ifndef WebServer_h
#define WebServer_h

#include "Settings.h"
#include "arduino-log-handler/LogHandler.h"
#include "HTTP.h"
#include "URL.h"
#include "RequestEvent.h"

#include <stdint.h>
#include <string.h>
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

class WebServer{
	public:
		WebServer(Print &logOutput, RequestEvent defaultRequestEvent);
		
		void init(uint8_t *mac, IPAddress ip);
		
		void run();
		
		void writeFile(const char *pathname);
	private:
		LogHandler logHandler;
		
		RequestEvent defaultRequestEvent;
		
		EthernetServer server;
		EthernetClient client;
		
		void evaluateRequestLine(char *requestLine);
};

#endif
