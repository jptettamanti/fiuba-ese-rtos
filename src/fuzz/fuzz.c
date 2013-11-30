#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "fuzz.h"
#include "fixedptc.h"

#define mix   50
#define gain  10

void fuzz_init(void *obj, int td, int fs)
{
	fuzz_t *fuzz = (fuzz_t*)malloc(sizeof(fuzz_t));

	*(fuzz_t **)obj = fuzz;
}

void fuzz_run(void *obj, sound_t *out, sound_t *in)
{
	fuzz_t  *fuzz = (fuzz_t *)obj;
	sound_t fuzzy;

	fuzzy = fixedpt_mul(*in, -gain);
	fuzzy = 1 - fixedpt_exp(fuzzy);
	fuzzy = fixedpt_mul(fuzzy, fixedpt_sign(-*in));

	*out = fixedpt_mul(*in, 100 - mix) + fixedpt_mul(fuzzy, mix);

	*out = fixedpt_div(*out, 100);
}

void fuzz_end(void *obj)
{
	fuzz_t *fuzz = (fuzz_t *)obj;
}
