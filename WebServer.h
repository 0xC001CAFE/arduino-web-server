#ifndef WebServer_h
#define WebServer_h

#include "Settings.h"
#include "HTTP.h"
#include "URIEvent.h"

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>
#include <SD.h>

class WebServer{
	public:
		typedef void (*DefaultEvent)(EthernetClient &client, char *path);
		
		WebServer(DefaultEvent defaultEvent);
		~WebServer();
		
		void init(byte *mac, IPAddress ip);
		
		void addURIEvent(URIEvent *event);
		
		void getMAC(byte *mac);
		String getMAC();
		IPAddress getIP();
		
		void run();
		
		void openFile(EthernetClient &client, const char *path);
	private:
		EthernetServer *server;
		
		DefaultEvent defaultEvent;
		
		byte eventsCount;
		URIEvent *events[URI_EVENTS_MAX_COUNT];
		
		bool checkEvents(EthernetClient &client, char *path, char *queryParameterNames[], char *queryParameterValues[]);
		
		void evaluateRequest(EthernetClient &client, char *requestURI);
};

#endif
