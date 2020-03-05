#include <WebServer.h>

// function prototype
void defaultRequestEvent(EthernetClient&, URL&);

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 0, 150);

WebServer server(Serial, defaultRequestEvent);

// function to handle valid HTTP requests
void defaultRequestEvent(EthernetClient &client, URL &requestURL){
	server.writeFile(requestURL.getPathname());
}

void setup(){
	Serial.begin(9600);
	Serial.println("Basic Web Server");
	
	// initialize web server
	server.init(mac, ip);
}

void loop(){
	// process HTTP requests
	server.run();
}
