#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "overdrive.h"

void overdrive_init(void *obj, int td, int fs)
{
	overdrive_t *overdrive = (overdrive_t*)malloc(sizeof(overdrive_t));

	*(overdrive_t **)obj = overdrive;
}

void overdrive_run(void *obj, sound_t *out, sound_t *in)
{
	overdrive_t *overdrive = (overdrive_t *)obj;

	if( ((*in) <= (SOUND_MAX / 3)) && ((*in) > (SOUND_MIN / 3)) )
	{
		*out = *in * 2;
	}
	else if( ((*in) <= (2 * SOUND_MAX / 3)) && ((*in) > (SOUND_MAX / 3)) )
	{
		*out = SOUND_MAX * 2 - *in * 3;
		*out = SOUND_MAX - (*out / 3) * (*out);
	}
	else if( ((*in) <= (SOUND_MIN / 3)) && ((*in) > (2 * SOUND_MIN / 3)) )
	{
		*out = SOUND_MIN * 2 + *in * 3;
		*out = SOUND_MIN + (*out / 3) * (*out);
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
