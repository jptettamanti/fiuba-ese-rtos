#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wah.h"

void wah_init(void *obj)
{
	wah_t *wah = (wah_t*)malloc(sizeof(wah_t));

	wah->yh = 0;	wah->yb = 0;	wah->yl = 0;

	*(wah_t **)obj = wah;
}

void wah_run(void *obj, sound_t *out, sound_t *in)
{
	wah_t *wah = (wah_t *)obj;

	// Effect computation
	wah->yh = *in - wah->yl - wah->q1 * wah->yb;
	wah->yb = wah->yb + wah->f1 * wah->yh;
	wah->yl = wah->yl + wah->f1 * wah->yb;

	f1 = 2*sin(PI*wah->tri()/FS);
}

void wah_end(void *obj)
{
	wah_t *wah = (wah_t *)obj;
	// There's nothing to be done here
}
