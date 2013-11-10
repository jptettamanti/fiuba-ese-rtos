#ifndef ECHO_H
#define ECHO_H

#include "delay.h"

typedef struct
{
	delay_t *	delay;
	int		fbk;
}echo_t;

void echo_init(void *obj, int td, int fs);
void echo_run(void *obj, unsigned char *out, unsigned char *in);
void echo_end(void *obj);

#endif
