#ifndef DELAY_H
#define DELAY_H

typedef unsigned char sound_t;

typedef struct
{
	sound_t *	data;
	size_t		position;
	size_t		size;
}delay_t;

void delay_init(void *obj, int td, int fs);
void delay_push(void *obj, sound_t *in);
void delay_pull(void *obj, sound_t *out);
void delay_end(void *obj);
void delay_run(void *obj, sound_t *out, sound_t *in);

#endif
