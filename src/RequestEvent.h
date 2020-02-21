#ifndef RequestEvent_h
#define RequestEvent_h

#include "URL.h"

#include <Arduino.h>
#include <SPI.h>
#include <Ethernet.h>

typedef void (*RequestEvent)(EthernetClient &client, URL &url);

#endif
