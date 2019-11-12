#include <string.h>
#include <ctype.h>

void strToLwr(char* str){
	for (int i = 0; i < strlen(str); ++i)
	{
		str[i] = tolower(str[i]);
	}
}