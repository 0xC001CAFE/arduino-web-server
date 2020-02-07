#include "Settings.h"
#include "HTTP.h"

bool HTTP::getRequest(char *requestBuffer, byte &requestBufferLength, EthernetClient &client, RequestMethod &requestMethod){
	if(client.available()){
		if(requestBufferLength < (REQUEST_BUFFER_SIZE - 1)){
			requestBuffer[requestBufferLength] = client.read();
			
			if(requestBufferLength > 0 && requestBuffer[requestBufferLength - 1] == '\r' && requestBuffer[requestBufferLength] == '\n'){
				#if LOGGING_OUTPUT > 2
				Serial.print("[WebServer] -> Info: ");
				Serial.print(requestBuffer);
				#endif
				
				if(!strncmp(requestBuffer, "GET", 3)) requestMethod = GET;
				
				return true;
			}
			
			requestBufferLength++;
		}else{
			#if LOGGING_OUTPUT > 0
			Serial.println("[WebServer] -> Error: request buffer runs out of memory");
			#endif
			
			return true;
		}
	}
	
	return false;
}

char* HTTP::getRequestURI(char *requestBuffer){
	char *requestURI = strtok(requestBuffer, " ");
	requestURI = strtok(NULL, " ");
	
	return requestURI;
}

void HTTP::ok(File &file, EthernetClient &client){
	client.println("HTTP/1.1 200 OK");
	client.println("Content-Type: text/html");
	client.println();
	
	while(file.available()){
		client.write(file.read());
	}
}

void HTTP::notFound(EthernetClient &client){
	client.println("HTTP/1.1 404 Not Found");
	client.println();
}

void HTTP::notImplemented(EthernetClient &client){
	client.println("HTTP/1.1 501 Not Implemented");
	client.println();
}
