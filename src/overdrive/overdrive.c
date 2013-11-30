#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "overdrive.h"
#include "fixedptc.h"

void overdrive_init(void *obj, int td, int fs)
{
	overdrive_t *overdrive = (overdrive_t*)malloc(sizeof(overdrive_t));

	*(overdrive_t **)obj = overdrive;
}

void overdrive_run(void *obj, sound_t *out, sound_t *in)
{
	overdrive_t *overdrive = (overdrive_t *)obj;

	if( ((*in) <= fixedpt_div(1,3)) && ((*in) > fixedpt_div(-1,3)) )
	{
		*out = fixedpt_mul(*in, 2);
	}
	// [(+3) - ((+2) - (+3) * in)^2] / 3
	else if( ((*in) <= fixedpt_div(2,3)) && ((*in) > fixedpt_div(1,3)) )
	{
		*out = fixedpt_mul(1, 2) - fixedpt_mul(*in, 3);
		*out = fixedpt_div(fixedpt_mul(1, 3) - fixedpt_mul(*out, *out), 3);
	}
	// [(-3) + ((-2) - (-3) * in)^2] / 3
	else if( ((*in) <= fixedpt_div(-1,3)) && (fixedpt_div(-2,3)) )
	{
		*out = fixedpt_mul(1, -2) + fixedpt_mul(*in, -3);
		*out = fixedpt_div(fixedpt_mul(1, -3) + fixedpt_mul(*out, *out), 3);
	}
	else
	{
		*out = *in;
	}

}

void overdrive_end(void *obj)
{
	overdrive_t *overdrive = (overdrive_t *)obj;
}
