#include "Params.h"

Params::Params(uint8_t maxLength){
	params = new Param[maxLength];
	
	this->maxLength = maxLength;
	length = 0;
}

Params::~Params(){
	delete[] params;
}

uint8_t Params::getMaxLength() const{
	return maxLength;
}

uint8_t Params::getLength() const{
	return length;
}

int8_t Params::nameExists(const char *name) const{
	for(uint8_t i = 0; i < length; i++){
		if(strcmp(params[i].name, name) == 0) return i;
	}
	
	return -1;
}

const char* Params::getName(uint8_t index) const{
	if(!validIndex(index)) return 0;
	
	return params[index].name;
}

const char* Params::getValueByIndex(uint8_t index) const{
	if(!validIndex(index)) return 0;
	
	return params[index].value;
}

const char* Params::getValueByName(const char *name) const{
	int8_t index = nameExists(name);
	
	if(index <= -1) return 0;
	
	return getValueByIndex(index);
}

bool Params::addParam(const char *name, const char *value){
	if(nameExists(name) != -1) return false;
	
	if(length >= maxLength) return false;
	
	params[length].name = name;
	params[length].value = value;
	
	length++;
	
	return true;
}

bool Params::removeParamByIndex(uint8_t index){
	if(!validIndex(index)) return false;
	
	length--;
	
	for(uint8_t i = index; i < length; i++){
		params[i] = params[i + 1];
	}
	
	return true;
}

bool Params::removeParamByName(const char *name){
	int8_t index = nameExists(name);
	
	if(index <= -1) return false;
	
	return removeParamByIndex(index);
}

bool Params::validIndex(uint8_t index) const{
	if(index < length) return true;
	
	return false;
}
