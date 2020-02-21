#include <WebServer.h>

// status led
#define LED

#ifdef LED
#define LED_PIN 30
#define LED_INTERVAL 250

unsigned long previousMillis = 0;
int ledState = LOW;
#endif

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
	#ifdef LED
	pinMode(LED_PIN, OUTPUT);
	#endif
	
	Serial.begin(9600);
	Serial.println("Basic Web Server");
	
	// initialize web server
	server.init(mac, ip);
}

void loop(){
	// process HTTP requests
	server.run();
	
	#ifdef LED
	unsigned long currentMillis = millis();
	
	if(currentMillis - previousMillis >= LED_INTERVAL){
		previousMillis = currentMillis;
		
		if(ledState == LOW){
			ledState = HIGH;
		}else{
			ledState = LOW;
		}
		
		digitalWrite(LED_PIN, ledState);
	}
	#endif
}
