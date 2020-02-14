#ifndef Params_h
#define Params_h

#include <stdint.h>
#include <string.h>

class Params{
	public:
		Params(uint8_t maxLength);
		~Params();
		
		uint8_t getMaxLength() const;
		uint8_t getLength() const;
		
		int8_t nameExists(const char *name) const;
		
		const char* getName(uint8_t index) const;
		
		const char* getValueByIndex(uint8_t index) const;
		const char* getValueByName(const char *name) const;
		
		bool addParam(const char *name, const char *value);
		
		bool removeParamByIndex(uint8_t index);
		bool removeParamByName(const char *name);
	private:
		struct Param{
			const char *name;
			const char *value;
		} *params;
		
		uint8_t maxLength;
		uint8_t length;
		
		bool validIndex(uint8_t index) const;
};

#endif
