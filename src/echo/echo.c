#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "echo.h"

void echo_init(void *obj, int td, int fs)
{
	echo_t *echo = (echo_t*)malloc(sizeof(echo_t));

	delay_init(&echo->delay, td, fs);

	*(echo_t **)obj = echo;
}

void echo_run(void *obj, sound_t *out, sound_t *in)
{
	echo_t *echo = (echo_t *)obj;

	// Pull delayed input value
	delay_pull(echo->delay, out);

	// Calculate new output value
	*out = *in / 2 + ((*out / 10) * 9) / 2;

	// Push current input value
	delay_push(echo->delay, in);
}

void echo_end(void *obj)
{
	echo_t *echo = (echo_t *)obj;

	if(echo->delay)
		delay_end(echo->delay);

	if(echo)
		free((void*)echo);
}
