#ifndef WebServer_h
#define WebServer_h

#include "Settings.h"
#include "arduino-log-handler/LogHandler.h"
#include "HTTP.h"
#include "URL.h"
#include "RequestEvent.h"
#include "URLEvent.h"

#include <stdint.h>
#include <string.h>
#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

class WebServer{
	public:
		WebServer(Print &logOutput, RequestEvent defaultRequestEvent);
		~WebServer();
		
		bool init(uint8_t *mac, IPAddress ip);
		
		IPAddress getIP() const;
		
		bool addURLEvent(const char *pathname, char *searchParams, RequestEvent requestEvent);
		
		void run();
		
		void write(const char *message, HTTP::ContentType contentType);
		void writeFile(const char *pathname);
	private:
		LogHandler logHandler;
		
		RequestEvent defaultRequestEvent;
		
		EthernetServer server;
		EthernetClient client;
		
		URLEvent *urlEvents[URL_EVENTS_MAX_COUNT];
		uint8_t urlEventsLength;
		
		#ifdef STATUS_LED
		uint32_t ledTime;
		
		bool ledStatus;
		#endif
		
		void evaluateRequestLine(char *requestLine);
};

#endif
