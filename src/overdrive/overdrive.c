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

	// Zona de ganancia lineal
	if( ((*in) <= fixedpt_rconst(1/3)) && ((*in) > fixedpt_rconst(-1/3)) )
	{
		// Calculo la salida
		*out = fixedpt_mul(*in, 2);
	}
	// Zona de ganancia cuadratica en primer cuadrante
	else if( ((*in) <= fixedpt_rconst(2/3)) && ((*in) > fixedpt_rconst(1/3)) )
	{
		// Calculo la salida
		*out = fixedpt_rconst(+2) - fixedpt_mul(*in, +3);
		*out = fixedpt_rconst(+3) - fixedpt_mul(*out, *out);
		*out = fixedpt_div(*out, 3);
	}
	// Zona de ganancia cuadratica en primer cuadrante
	else if( ((*in) <= fixedpt_rconst(-1/3)) && ((*in) > fixedpt_rconst(-2/3)) )
	{
		// Calculo la salida
		*out = fixedpt_rconst(-2) + fixedpt_mul(*in, -3);
		*out = fixedpt_rconst(-3) + fixedpt_mul(*out, *out);
		*out = fixedpt_div(*out, 3);
	}
	// Zona sin ganancia
	else
	{
		// Calculo la salida
		*out = *in;
	}

}

void overdrive_end(void *obj)
{
	overdrive_t *overdrive = (overdrive_t *)obj;
}
