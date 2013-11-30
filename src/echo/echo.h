#ifndef ECHO_H
#define ECHO_H

#include "delay.h"

typedef struct
{
	delay_t *	delay;
	int		fbk;
}echo_t;

void echo_init(void *obj, int td, int fs);
void echo_run(void *obj, sound_t *out, sound_t *in);
void echo_end(void *obj);

#endif
