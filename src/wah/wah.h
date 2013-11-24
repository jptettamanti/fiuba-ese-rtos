#ifndef WAH_H
#define WAH_H

#include "delay.h"

typedef struct
{
	yh	// High-pass signal
	yb	// Band-pass signal
	yl	// Low-pass signal
	f1	// Center frequency coefficient
	q1	// Damping frequency coefficient
}wah_t;

typedef struct
{
	damp	// Damping factor
	minf	// Min band-pass filter cutoff frequency
	maxf	// Max band-pass filter cutoff frequency
	wahf	// Wah effect frecuency (how often it sweeps between 'minf' and 'maxf' cutoff frequencies)
}wahconf_t;

void wah_init(void *obj);
void wah_run(void *obj, sound_t *out, sound_t *in);
void wah_end(void *obj);

#endif
