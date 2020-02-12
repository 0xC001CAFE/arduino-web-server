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
	private:
		EthernetServer *server;
		
		byte eventsCount;
		URIEvent *events[URI_EVENTS_MAX_COUNT];
		
		bool checkEvents(EthernetClient &client, char *path, char *queryParameterNames[], char *queryParameterValues[]);
		
		void evaluateRequest(char *requestURI, EthernetClient &client);
		
		void openFile(const char *path, EthernetClient &client);
	public:
		WebServer();
		~WebServer();
		
		void init(byte *mac, IPAddress ip);
		
		void addURIEvent(URIEvent *event);
		
		void getMAC(byte *mac);
		String getMAC();
		IPAddress getIP();
		
		void run();
};

#endif
