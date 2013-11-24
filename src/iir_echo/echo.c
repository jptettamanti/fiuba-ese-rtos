#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "echo.h"

#define feedback	5
#define effect_level	10

void echo_init(void *obj, int td, int fs)
{
	echo_t *echo = (echo_t*)malloc(sizeof(echo_t));

	delay_init(&echo->delay, td, fs);

	*(echo_t **)obj = echo;
}

void echo_run(void *obj, sound_t *out, sound_t *in)
{
	echo_t  *echo = (echo_t *)obj;
	sound_t fbk;

	// Saco la muestra mas vieja de la cadena de delay
	delay_pull(echo->delay, &fbk);

	// Calculo el valor que ingresa a la cadena de delay
	*out = *in * 100 + fbk * feedback / 100;

	// Re-normalizo la señal de realimentada
	*fbk = *fbk / (100 + feedback);

	// Guardo la señal realimentada
	delay_push(echo->delay, &fbk);

	// Calculo la salida
	*out = *in * 100 + fbk * effect_level;

	// Re-normalizo la señal de salida
	*out = *out / (100 + effect_level);
}

void echo_end(void *obj)
{
	echo_t *echo = (echo_t *)obj;

	if(echo->delay)
		delay_end(echo->delay);

	if(echo)
		free((void*)echo);
}
