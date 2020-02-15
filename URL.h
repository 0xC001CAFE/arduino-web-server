#ifndef URL_h
#define URL_h

#include "Settings.h"
#include "Params.h"

#include <stdint.h>
#include <string.h>

class URL{
	public:
		URL(char *url);
		
		char* getPathname() const;
		
		const Params& getSearchParams() const;
	private:
		char *pathname;
		
		Params searchParams;
		
		void identifyComponents(char *url);
};

#endif
