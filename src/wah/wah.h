#ifndef WAH_H
#define WAH_H

#include "sound.h"

typedef struct
{
	sound_t	yh;	// High-pass signal
	sound_t	yb;	// Band-pass signal
	sound_t	yl;	// Low-pass signal
	sound_t	f1;	// Center frequency coefficient
	sound_t	q1;	// Damping frequency coefficient
}wah_t;

typedef struct
{
	sound_t	damp;	// Damping factor
	sound_t	minf;	// Min band-pass filter cutoff frequency
	sound_t	maxf;	// Max band-pass filter cutoff frequency
	sound_t	wahf;	// Wah effect frecuency (how often it sweeps between 'minf' and 'maxf' cutoff frequencies)
}wahconf_t;

void wah_init(void *obj, int td, int fs);
void wah_run(void *obj, sound_t *out, sound_t *in);
void wah_end(void *obj);

#endif
