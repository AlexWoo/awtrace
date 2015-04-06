/********************************************************************
 * File: instrument.c
 *
 * link this to your application,
 * you will get an record file <pid>.recrod
 * in your app execute director
 *
 * Author: AlexWoo <wj19840501@gmail.com>
 *
 * install:
 * > make
 * > make install
 * Use libawtrace.so:
 *   -g -lawtrace -finstrument-functions
 */

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/time.h>

/* Function prototypes with attributes */
void main_constructor(void)
	__attribute__ ((no_instrument_function, constructor));

void main_destructor(void)
	__attribute__ ((no_instrument_function, destructor));

void __cyg_profile_func_enter(void*, void*) 
	__attribute__ ((no_instrument_function));

void __cyg_profile_func_exit(void*, void*)
	__attribute__ ((no_instrument_function));

static FILE *fp;

void main_constructor(void)
{
	char tracefile[256];
	snprintf(tracefile, sizeof(tracefile), "%lu.record", (long)getpid());
	fp = fopen(tracefile, "w");
	if (NULL == fp)
	{
  		exit(-1);
	}
}

void main_deconstructor(void)
{
	fclose(fp);
}

void __cyg_profile_func_enter(void* funcaddr, void* callsite)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	fprintf(fp, "E%p %lu %lu.%06d\n", (long*)funcaddr, pthread_self(),
		now.tv_sec, (int)now.tv_usec);
}

void __cyg_profile_func_exit(void *funcaddr, void *callsite)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	fprintf(fp, "X%p %lu %lu.%06d\n", (long*)funcaddr, pthread_self(),
		now.tv_sec, (int)now.tv_usec);
}

#ifdef __cplusplus
};
#endif
