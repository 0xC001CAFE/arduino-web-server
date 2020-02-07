#ifndef WebServer_h
#define WebServer_h

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

class WebServer{
	private:
		EthernetServer *server;
		
		void evaluateRequest(char *requestURI, EthernetClient &client);
		void openFile(const char *path, EthernetClient &client);
	public:
		WebServer();
		~WebServer();
		
		void init(byte *mac, IPAddress ip);
		
		void getMAC(byte *mac);
		String getMAC();
		IPAddress getIP();
		
		void run();
};

#endif
