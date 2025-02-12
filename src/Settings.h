#ifndef Settings_h
#define Settings_h

#define STATUS_LED
#ifdef STATUS_LED
#define STATUS_LED_PIN 30
#define STATUS_LED_INTERVAL 250
#endif

// off = 0
// error = 1
// error + warning = 2
// error + warning + info = 3
#define LOG_HANDLER_LEVEL 3

#define DEFAULT_FILENAME "index.htm"

#define REQUEST_TIMEOUT 1000
#define REQUEST_BUFFER_SIZE 100

#define SD_SLAVE_SELECT_PIN 4
#define ETHERNET_SLAVE_SELECT_PIN 10

#define URL_SEARCH_PARAMS_MAX_COUNT 5

#define URL_EVENTS_MAX_COUNT 5

#endif
