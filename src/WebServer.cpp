#include "WebServer.h"

WebServer::WebServer(Print &logOutput, RequestEvent defaultRequestEvent): logHandler(logOutput, "WebServer"), server(80){
	this->defaultRequestEvent = defaultRequestEvent;
}

void WebServer::init(uint8_t *mac, IPAddress ip){
	pinMode(ETHERNET_SLAVE_SELECT_PIN, OUTPUT);
	digitalWrite(ETHERNET_SLAVE_SELECT_PIN, HIGH);
	
	if(!SD.begin(SD_SLAVE_SELECT_PIN)){
		#if LOG_HANDLER_LEVEL > 0
		logHandler.log(LogHandler::ERROR, "SD card could not be initialized");
		#endif
		
		return;
	}
	
	#if LOG_HANDLER_LEVEL > 1
	if(!SD.exists(ROOT_PATHNAME)){
		logHandler.log(LogHandler::WARNING, "root pathname \"%s\" does not exist", ROOT_PATHNAME);
	}
	#endif
	
	Ethernet.init(ETHERNET_SLAVE_SELECT_PIN);
	Ethernet.begin(mac, ip);
	
	server.begin();
	
	#if LOG_HANDLER_LEVEL > 2
	logHandler.log(LogHandler::INFO, "web server is running at \"%u.%u.%u.%u\"", ip[0], ip[1], ip[2], ip[3]);
	#endif
}

void WebServer::run(){
	/* if(!server){
		#if LOG_HANDLER_LEVEL > 0
		logHandler.log(LogHandler::ERROR, "a problem has occurred");
		#endif
		
		return;
	} */
	
	client = server.available();
	
	if(client){
		#if LOG_HANDLER_LEVEL > 2
		IPAddress remoteIP = client.remoteIP();
		
		logHandler.log(LogHandler::INFO, ">>>>> new client \"%u.%u.%u.%u\" is connected <<<<<", remoteIP[0], remoteIP[1], remoteIP[2], remoteIP[3]);
		#endif
		
		char requestBuffer[REQUEST_BUFFER_SIZE] = { 0 };
		uint8_t requestBufferLength = 0;
		
		HTTP::RequestStatus requestStatus;
		
		uint32_t requestTimeout = millis() + REQUEST_TIMEOUT;
		
		while(client.connected()){
			if(HTTP::getRequestLine(client, requestBuffer, requestBufferLength, requestStatus)){
				switch(requestStatus){
					case HTTP::BUFFER_OVERFLOW:
						#if LOG_HANDLER_LEVEL > 0
						logHandler.log(LogHandler::ERROR, "request buffer runs out of memory");
						#endif
						
						// HTTP response
						
						break;
					case HTTP::INVALID_HTTP_VERSION:
						#if LOG_HANDLER_LEVEL > 1
						logHandler.log(LogHandler::WARNING, "invalid HTTP version");
						#endif
						
						// HTTP response
						
						break;
					case HTTP::INVALID_REQUEST_METHOD:
						#if LOG_HANDLER_LEVEL > 1
						logHandler.log(LogHandler::WARNING, "invalid request method");
						#endif
						
						//HTTP::notImplemented(client);
						
						break;
					case HTTP::GET_REQUEST:
						#if LOG_HANDLER_LEVEL > 2
						logHandler.log(LogHandler::INFO, "GET request method -> request line: \"%s\"", requestBuffer);
						#endif
						
						evaluateRequestLine(requestBuffer);
						
						break;
				}
				
				break;
			}
			
			if(millis() > requestTimeout){
				#if LOG_HANDLER_LEVEL > 1
				logHandler.log(LogHandler::WARNING, "request timeout");
				#endif
				
				break;
			}
		}
		
		client.flush();
		client.stop();
		
		#if LOG_HANDLER_LEVEL > 2
		logHandler.log(LogHandler::INFO, ">>>>> client is disconnected <<<<<");
		#endif
	}
}

void WebServer::writeFile(const char *pathname){
	char *defaultPathname = 0;
	
	char *fileExtension = strrchr(pathname, '.');
	
	if(!fileExtension || (fileExtension && strchr(fileExtension, '/'))){
		// directory
		uint8_t pathnameLength = strlen(pathname);
		
		if(pathname[pathnameLength - 1] != '/'){
			// HTTP move permanently
			
			return;
		}
		
		defaultPathname = new char[pathnameLength + strlen(DEFAULT_FILENAME) + 1];
		
		strcpy(defaultPathname, pathname);
		strcat(defaultPathname, DEFAULT_FILENAME);
		
		fileExtension = strrchr(defaultPathname, '.');
	}
	
	HTTP::ContentType contentType;
	
	if(!HTTP::supportsFileExtension(fileExtension, contentType)){
		#if LOG_HANDLER_LEVEL > 1
		logHandler.log(LogHandler::WARNING, "file extension \"%s\" is not supported", fileExtension);
		#endif
		
		// HTTP response
		
		return;
	}
	
	if(!SD.exists(defaultPathname ? defaultPathname : pathname)){
		#if LOG_HANDLER_LEVEL > 1
		logHandler.log(LogHandler::WARNING, "pathname \"%s\" does not exist", defaultPathname ? defaultPathname : pathname);
		#endif
		
		// HTTP response
		
		return;
	}
	
	File file = SD.open(defaultPathname ? defaultPathname : pathname, FILE_READ);
	
	if(!file){
		// log message
		
		// HTTP response
		
		return;
	}
	
	HTTP::ok(client, contentType);
	
	while(file.available()){
		client.write(file.read());
	}
	
	file.close();
}

void WebServer::evaluateRequestLine(char *requestLine){
	char *requestURL;
	
	if(!HTTP::validRequestURL(requestLine, requestURL)){
		#if LOG_HANDLER_LEVEL > 1
		logHandler.log(LogHandler::WARNING, "invalid request URL");
		#endif
		
		// HTTP response
		
		return;
	}
	
	#if LOG_HANDLER_LEVEL > 2
	logHandler.log(LogHandler::INFO, "valid request URL -> URL: \"%s\"", requestURL);
	#endif
	
	URL url(logHandler, requestURL);
	
	defaultRequestEvent(client, url);
}
