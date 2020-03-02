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
			requestBuffer[requestBufferLength - 1] = '\0';
			
			if(!StringFunctions::endsWith(requestBuffer, "HTTP/1.1")){
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

void HTTP::movedPermanently(EthernetClient &client, IPAddress ip, const char *pathname, const char *pathnameSuffix){
	client.print("HTTP/1.1 301 Moved Permanently\r\nLocation: http://");
	client.print(ip);
	client.print(pathname);
	if(pathnameSuffix) client.print(pathnameSuffix);
	client.print("\r\n\r\n");
}

void HTTP::badRequest(EthernetClient &client){
	client.print("HTTP/1.1 400 Bad Request\r\n\r\n");
}

void HTTP::notFound(EthernetClient &client){
	client.print("HTTP/1.1 404 Not Found\r\n\r\n");
}

void HTTP::requestTimeout(EthernetClient &client){
	client.print("HTTP/1.1 408 Request Timeout\r\n\r\n");
}

void HTTP::requestURITooLong(EthernetClient &client){
	client.print("HTTP/1.1 414 Request-URI Too Long\r\n\r\n");
}

void HTTP::unsupportedMediaType(EthernetClient &client){
	client.print("HTTP/1.1 415 Unsupported Media Type\r\n\r\n");
}

void HTTP::internalServerError(EthernetClient &client){
	client.print("HTTP/1.1 500 Internal Server Error\r\n\r\n");
}

void HTTP::notImplemented(EthernetClient &client){
	client.print("HTTP/1.1 501 Not Implemented\r\n\r\n");
}

void HTTP::httpVersionNotSupported(EthernetClient &client){
	client.print("HTTP/1.1 505 HTTP Version Not Supported\r\n\r\n");
}
