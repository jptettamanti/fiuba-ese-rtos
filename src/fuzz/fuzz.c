#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fuzz.h"
#include "fixedptc.h"

#define mix   fixedpt_rconst(0.2)
#define gain  fixedpt_rconst(100)

void fuzz_init(void *obj, int td, int fs)
{
	fuzz_t *fuzz = (fuzz_t*)malloc(sizeof(fuzz_t));

	*(fuzz_t **)obj = fuzz;
}

void fuzz_run(void *obj, sound_t *out, sound_t *in)
{
	fuzz_t  *fuzz = (fuzz_t *)obj;
	sound_t fuzzy;

	// Calcular señal de fuzz
	fuzzy = fixedpt_mul(*in, gain);
	fuzzy = fixedpt_rconst(1) - fixedpt_exp(-fuzzy);
	fuzzy = fixedpt_mul(fuzzy, -fixedpt_sign(*in));

	// Calculo la salida
	*out = fixedpt_mul(*in, fixedpt_rconst(1) - mix) + fixedpt_mul(fuzzy, mix);
}

void fuzz_end(void *obj)
{
	fuzz_t *fuzz = (fuzz_t *)obj;
}
