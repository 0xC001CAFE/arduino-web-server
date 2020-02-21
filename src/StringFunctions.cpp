#include "StringFunctions.h"

bool StringFunctions::endsWith(const char *str, const char *suffix){
	if(!str || !suffix) return false;
	
	size_t strLen = strlen(str);
	size_t suffixLen = strlen(suffix);
	
	if(strLen < suffixLen) return false;
	
	for(size_t i = 1; i <= suffixLen; i++){
		if(str[strLen - i] != suffix[suffixLen - i]) return false;
	}
	
	return true;
}
