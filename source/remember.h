#ifndef __REMEMBER_H__
#define __REMEMBER_H__

#include <stdlib.h>



#ifndef __func__
#define __FUNCTION__ __func__
#else
#define __FUNCTION__ NULL
#endif // __FUNCTION__




void* remember_alloc(size_t size, const char* file, const char* function, int line);
void  remember_delete (void* ptr, const char* file, const char* function, int line);


#ifndef __DEBUG__
#define malloc(size) remember_alloc(size, __FILE__, __FUNCTION__, __LINE__)
#define free(ptr)    remember_delete(ptr, __FILE__, __FUNCTION__, __LINE__)
#endif // __DEBUG__



#endif // __REMEMBER_H__
