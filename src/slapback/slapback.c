#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "echo.h"
#include "fixedptc.h"

#define effect_level 10

void echo_init(void *obj, int td, int fs)
{
	echo_t *echo = (echo_t*)malloc(sizeof(echo_t));

	delay_init(&echo->delay, td, fs);

	*(echo_t **)obj = echo;
}

void echo_run(void *obj, sound_t *out, sound_t *in)
{
	echo_t  *echo = (echo_t *)obj;
	sound_t delayed;

	// Saco la muestra mas vieja de la cadena de delay
	delay_pull(echo->delay, &delayed);

	// Guardo la señal de entrada
	delay_push(echo->delay, in);

	// Calculo la salida
	*out = fixedpt_mul(*in, 100 - effect_level) + fixedpt_mul(fbk, effect_level);

	// Re-normalizo la señal de salida
	*out = fixedpt_div(*out, 100);
}

void echo_end(void *obj)
{
	echo_t *echo = (echo_t *)obj;

	if(echo->delay)
		delay_end(echo->delay);

	if(echo)
		free((void*)echo);
}
