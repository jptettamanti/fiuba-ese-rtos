#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "delay.h"

void delay_init(void *obj, int td, int fs)
{
	// Si reservaran el espacio afuera
	// delay_t *delay = (delay_t *)obj;

	// Evita que tengan que reservar el espacio afuera
	delay_t *delay = (delay_t*)malloc(sizeof(delay_t));

	delay->position = 0;
	delay->size = td * fs;
	delay->data = (sound_t*)malloc(sizeof(sound_t)*delay->size);

	memset((void*)delay->data, 0, sizeof(sound_t)*delay->size);

	*(delay_t **)obj = delay;
}

/* Solo por simetría y pruebas */
void delay_run(void *obj, sound_t *out, sound_t *in)
{
	delay_pull(obj, out);
	delay_push(obj, in);
}

void delay_end(void *obj)
{
	delay_t *delay = (delay_t *)obj;

	// Libero el espacio usado por los datos usados del delay
	if(delay->data)
		free((void*)delay->data);

	// Libero el espacio usado por la estructura delay
	if(delay)
		free((void*)delay);
}

void delay_push(void *obj, sound_t *in)
{
	delay_t *delay = (delay_t *)obj;

	// In case memory was not allocated
	if(!delay->data) return;

	// Guardo la muestra actual
	*(delay->data + ((delay->position) % delay->size)) = *in;

	// Muevo el puntero a la siguiente posicion
	delay->position = (delay->position++) % delay->size;
}

void delay_pull(void *obj, sound_t *out)
{
	delay_t *delay = (delay_t *)obj;

	// In case memory was not allocated
	if(!delay->data) return;

	// Obtengo la muestra con delay
	*out  = *(delay->data + ((delay->position + 1) % delay->size));
}
