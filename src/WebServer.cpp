#include "WebServer.h"

WebServer::WebServer(DefaultEvent defaultEvent){
	server = new EthernetServer(80);
	
	this->defaultEvent = defaultEvent;
	
	eventsCount = 0;
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

void WebServer::addURIEvent(URIEvent *event){
	if(eventsCount < URI_EVENTS_MAX_COUNT){
		events[eventsCount] = event;
		eventsCount++;
		
		return;
	}
	
	#if LOGGING_OUTPUT > 1
	Serial.println("[WebServer] -> Warning: too many URI Events have been added");
	#endif
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
			if(HTTP::getRequest(client, requestBuffer, requestBufferLength, requestMethod)){
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
						
						evaluateRequest(client, requestURI);
						
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

void WebServer::openFile(EthernetClient &client, const char *path){
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
	
	HTTP::ok(client, file);
	
	file.close();
}

bool WebServer::checkEvents(EthernetClient &client, char *path, char *queryParameterNames[], char *queryParameterValues[]){
	for(byte i = 0; i < eventsCount; i++){
		if(events[i]->triggerEvent(client, path, queryParameterNames, queryParameterValues)) return true;
	}
	
	return false;
}

void WebServer::evaluateRequest(EthernetClient &client, char *requestURI){
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
	
	char *path = strtok(requestURI, "?");
	char *query = strtok(NULL, "?");
	
	if(strlen(path) == 1) path = "/index.htm";
	
	char *parameterNames[URI_QUERY_PARAMETERS_MAX_COUNT] = { NULL };
	char *parameterValues[URI_QUERY_PARAMETERS_MAX_COUNT] = { NULL };
	
	if(HTTP::getURIQueryParameters(query, parameterNames, parameterValues) && checkEvents(client, path, parameterNames, parameterValues)) return;
	
	defaultEvent(client, path);
}
