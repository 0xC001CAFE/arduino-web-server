#ifndef URL_h
#define URL_h

#include "Settings.h"
#include "arduino-log-handler/LogHandler.h"
#include "Params.h"

#include <stdint.h>
#include <string.h>

class URL{
	public:
		URL(LogHandler &logHandler, char *url);
		
		char* getPathname() const;
		
		const Params& getSearchParams() const;
	private:
		LogHandler &logHandler;
		
		char *pathname;
		
		Params searchParams;
		
		void identifyComponents(char *url);
};

#endif
