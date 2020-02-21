#include "HTTP.h"

const char *HTTP::contentTypes[] = {
	"text/plain",
	"text/html"
};

const char *HTTP::fileExtensions[] = {
	".txt",
	".htm"
};

bool HTTP::getRequestLine(EthernetClient &client, char *requestBuffer, uint8_t &requestBufferLength, RequestStatus &status){
	if(!client.available()) return false;
	
	if(requestBufferLength < (REQUEST_BUFFER_SIZE - 1)){
		requestBuffer[requestBufferLength] = client.read();
		
		if(requestBufferLength > 0 && requestBuffer[requestBufferLength - 1] == '\r' && requestBuffer[requestBufferLength] == '\n'){
			requestBuffer[requestBufferLength + 1] = '\0';
			
			if(!StringFunctions::endsWith(requestBuffer, "HTTP/1.1\r\n")){
				status = INVALID_HTTP_VERSION;
			}else if(strncmp(requestBuffer, "GET", 3) == 0){
				status = GET_REQUEST;
			}else{
				status = INVALID_REQUEST_METHOD;
			}
			
			return true;
		}
		
		requestBufferLength++;
	}else{
		requestBuffer[REQUEST_BUFFER_SIZE - 1] = '\0';
		
		status = BUFFER_OVERFLOW;
		
		return true;
	}
	
	return false;
}

bool HTTP::validRequestURL(char *requestLine, char *&requestURL){
	strtok(requestLine, " ");
	requestURL = strtok(0, " ");
	
	if(!requestURL || requestURL[0] != '/') return false;
	
	return true;
}

bool HTTP::supportsFileExtension(char *fileExtension, ContentType &type){
	for(uint8_t i = 0; i < (sizeof(fileExtensions) / sizeof(char*)); i++){
		if(strcmp(fileExtensions[i], fileExtension) == 0){
			type = (ContentType)i;
			
			return true;
		}
	}
	
	return false;
}

void HTTP::ok(EthernetClient &client, ContentType type){
	client.print("HTTP/1.1 200 OK\r\nContent-Type: ");
	client.print(contentTypes[type]);
	client.print("\r\n\r\n");
}
