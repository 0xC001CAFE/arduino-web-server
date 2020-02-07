#include "Settings.h"
#include "WebServer.h"
#include "HTTP.h"

#include <SD.h>

void WebServer::evaluateRequest(char *requestURI, EthernetClient &client){
	if(!requestURI || requestURI[0] != '/'){
		#if LOGGING_OUTPUT > 1
		Serial.println("[WebServer] -> Warning: invalid URI");
		#endif
		
		return;
	}
	
	#if LOGGING_OUTPUT > 2
	Serial.print("[WebServer] -> Info: URI ");
	Serial.println(requestURI);
	#endif
	
	if(strlen(requestURI) == 1){
		openFile("/index.htm", client);
		
		return;
	}
	
	char *path = strtok(requestURI, "?");
	char *query = strtok(NULL, "?");
	
	if(!query){
		#if LOGGING_OUTPUT > 2
		Serial.println("[WebServer] -> Info: no query was found");
		#endif
	}
	
	openFile(path, client);
}

void WebServer::openFile(const char *path, EthernetClient &client){
	if(!SD.exists(path)){
		#if LOGGING_OUTPUT > 0
		Serial.print("[WebServer] -> Error: file \"");
		Serial.print(path);
		Serial.println("\" does not exist");
		#endif
		
		HTTP::notFound(client);
		
		return;
	}
	
	File file = SD.open(path);
	
	if(!file){
		HTTP::notFound(client);
		
		return;
	}
	
	HTTP::ok(file, client);
	
	file.close();
}

WebServer::WebServer(){
	server = new EthernetServer(80);
}

WebServer::~WebServer(){
	delete server;
}

void WebServer::init(byte *mac, IPAddress ip){
	pinMode(ETHERNET_SLAVE_SELECT_PIN, OUTPUT);
	digitalWrite(ETHERNET_SLAVE_SELECT_PIN, HIGH);
	
	if(!SD.begin(SD_SLAVE_SELECT_PIN)){
		#if LOGGING_OUTPUT > 0
		Serial.println("[WebServer] -> Error: SD card not initialized");
		#endif
	}
	
	Ethernet.init(ETHERNET_SLAVE_SELECT_PIN);
	Ethernet.begin(mac, ip);
	
	server->begin();
}

void WebServer::getMAC(byte *mac){
	Ethernet.MACAddress(mac);
}

String WebServer::getMAC(){
	String mac;
	byte macBuffer[6];
	
	getMAC(macBuffer);
	
	for(byte i = 0; i < 6; i++){
		mac += String(macBuffer[i], HEX);
		
		if(i < 5){
			mac += ":";
		}
	}
	
	return mac;
}

IPAddress WebServer::getIP(){
	return Ethernet.localIP();
}

void WebServer::run(){
	EthernetClient client = server->available();
	
	if(client){
		char requestBuffer[REQUEST_BUFFER_SIZE] = "";
		byte requestBufferLength = 0;
		HTTP::RequestMethod requestMethod = HTTP::INVALID;
		
		unsigned long requestTimeout = millis() + REQUEST_TIMEOUT;
		
		#if LOGGING_OUTPUT > 2
		Serial.println("[WebServer] -> Info: new client");
		#endif
		
		while(client.connected()){
			if(HTTP::getRequest(requestBuffer, requestBufferLength, client, requestMethod)){
				switch(requestMethod){
					case HTTP::INVALID:
						#if LOGGING_OUTPUT > 1
						Serial.println("[WebServer] -> Warning: invalid request method");
						#endif
						
						HTTP::notImplemented(client);
						
						break;
					case HTTP::GET:
						#if LOGGING_OUTPUT > 2
						Serial.println("[WebServer] -> Info: GET request method");
						#endif
						
						char *requestURI = HTTP::getRequestURI(requestBuffer);
						
						evaluateRequest(requestURI, client);
						
						break;
				}
				
				break;
			}
			
			if(millis() > requestTimeout){
				#if LOGGING_OUTPUT > 1
				Serial.println("[WebServer] -> Warning: request timeout");
				#endif
				
				break;
			}
		}
		
		client.stop();
		
		#if LOGGING_OUTPUT > 2
		Serial.println("[WebServer] -> Info: client disconnected");
		#endif
	}
}
