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

bool HTTP::getURIQueryParameters(char *query, char *parameterNames[], char *parameterValues[]){
	if(!query){
		#if LOGGING_OUTPUT > 2
		Serial.println("[WebServer] -> Info: no query was found");
		#endif
		
		return false;
	}
	
	char *queryParameters[URI_QUERY_PARAMETERS_MAX_COUNT] = { NULL };
	
	queryParameters[0] = strtok(query, "&");
	for(byte i = 1; i < URI_QUERY_PARAMETERS_MAX_COUNT && queryParameters[i - 1]; i++){
		queryParameters[i] = strtok(NULL, "&");
		
		#if LOGGING_OUTPUT > 1
		if(i == (URI_QUERY_PARAMETERS_MAX_COUNT - 1) && queryParameters[i] && strtok(NULL, "&")){
			Serial.println("[WebServer] -> Warning: not enough memory available for all query parameters");
		}
		#endif
	}
	
	#if LOGGING_OUTPUT > 2
	Serial.print("[WebServer] -> Info: query parameters {");
	#endif
	
	for(byte i = 0; i < URI_QUERY_PARAMETERS_MAX_COUNT && queryParameters[i]; i++){
		parameterNames[i] = strtok(queryParameters[i], "=");
		parameterValues[i] = strtok(NULL, "=");
		
		#if LOGGING_OUTPUT > 2
		if(i > 0) Serial.print(", ");
		Serial.print("{");
		Serial.print(parameterNames[i]);
		Serial.print(", ");
		Serial.print(parameterValues[i]);
		Serial.print("}");
		#endif
	}
	
	#if LOGGING_OUTPUT > 2
	Serial.println("}");
	#endif
	
	return true;
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
