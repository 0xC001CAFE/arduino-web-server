#include <WebServer.h>

// status led
#define LED

#ifdef LED
#define LED_PIN 30
#define LED_INTERVAL 250

unsigned long previousMillis = 0;
int ledState = LOW;
#endif

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(192, 168, 0, 150);

WebServer server;

void setup(){
	#ifdef LED
	pinMode(LED_PIN, OUTPUT);
	#endif
	
	// initialize web server
	Serial.begin(9600);
	server.init(mac, ip);
	
	Serial.println("Basic Web Server");
	Serial.print("IP: ");
	Serial.println(server.getIP());
	Serial.print("MAC: ");
	Serial.println(server.getMAC());
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
