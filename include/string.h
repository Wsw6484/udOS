//常见字符串处理函数

#ifndef INCLUDE_STRING_H_
#define INCLUDE_STRING_H_

#include "types.h"

static inline void memcpy(uint8_t* dst,uint8_t* src,uint32_t len)
{
	while(len--) *dst++=*src++;
}

static inline void memset(void* dst,uint8_t val,uint32_t len)
{
	uint8_t* temp=(uint8_t*)dst;
	while(len--) *temp++=val;
}

static inline int strlen(const char* s)
{	
	int len=0;
	while(*s++) len++;	
	return len;
}

static inline void bzero(void* dst,uint32_t len)
{
	memset(dst,0,len);
}

static inline int strcmp(const char* str1,const char* str2)
{
	while(*str1&&*str2&&*str1==*str2)
	{
		str1++;
		str2++; 
	}
	
	return *str1-*str2;
}

static inline char* strcpy(char* dst,const char* src)
{
	char* temp=dst;
	
	while(*src) *temp++=*src++;
	
	*temp='\0';
	return dst;
}

static inline char* strcat(char* dst,const char* src)
{
	char* temp=dst;
	while(*temp++);
	while(*src) *temp++=*src++;	
	return dst;
}



#endif 	

