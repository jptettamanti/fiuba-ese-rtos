#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "wah.h"
#include "fixedptc.h"

#define damp	fixedpt_rconst(0.05)
#define minf	fixedpt_rconst(0.5)	// En KHz
#define maxf	fixedpt_rconst(3)	// En KHz
#define wahf	fixedpt_rconst(0.5)	// En KHz
#define fss	fixedpt_rconst(44.1)	// En KHz

void wah_init(void *obj, int td, int fs)
{
	wah_t *wah = (wah_t*)malloc(sizeof(wah_t));

	wah->yh = 0;	wah->yb = 0;	wah->yl = 0;

	*(wah_t **)obj = wah;
}

void wah_run(void *obj, sound_t *out, sound_t *in)
{
	wah_t *wah = (wah_t *)obj;
	static fixedpt fc;
	fixedpt q1;
	fixedpt f1;

	FILE * fd;
	fd = fopen("Dump.csv","a+");

	// Calculo la constante de damping del filtro
	q1  = fixedpt_mul(damp, FIXEDPT_TWO);

	// Calculo el siguiente valor de t para la triangular
	fc += fixedpt_div(wahf, fss);
	fc %= FIXEDPT_TWO;

	// Calcular el valor de la onda triangular
	f1  = fixedpt_tri(fc);
	f1  = f1 + FIXEDPT_ONE;
	f1  = fixedpt_div(f1, FIXEDPT_TWO);
	f1  = fixedpt_mul(f1, maxf - minf) + minf;

	// Calcular el valor de la senoidal que modula
	f1  = fixedpt_mul(f1, FIXEDPT_PI);
	f1  = fixedpt_div(f1, fss);
	f1  = fixedpt_sin(f1);
	f1  = fixedpt_mul(f1, FIXEDPT_TWO);

	// Calcular el wah
	wah->yh = *in - wah->yl - fixedpt_mul(wah->yb, q1);
	wah->yb =       wah->yb + fixedpt_mul(wah->yh, f1);
	wah->yl =       wah->yl + fixedpt_mul(wah->yb, f1);

	wah->yh = fixedpt_div(wah->yh, fixedpt_rconst(3));
	wah->yb = fixedpt_div(wah->yb, fixedpt_rconst(2));
	wah->yl = fixedpt_div(wah->yl, fixedpt_rconst(2));

	fprintf(fd, "%d\t%d\t%d\n", wah->yl, wah->yb, wah->yh);

	*out = wah->yb;

	fclose(fd);
}

void wah_end(void *obj)
{
	wah_t *wah = (wah_t *)obj;
	// There's nothing to be done here
}
