#ifndef SOUND_H
#define SOUND_H

//#define fs 		44100			// Sample frequency

typedef int sound_t;			// Basic sound type for register size independence

typedef struct				// Template for generic effect type object
{
	void *self;
	void *name;
	void *conf;
	void *data;

	void (*ini)(void *self);
	void (*run)(void *self);
	void (*end)(void *self);
}effect_t;

typedef struct				// Template for generic bank type object (a bank is a group of effects)
{
	void     *self;
	void     *name;
	effect_t effects[1];

	void (*ini)(void *self);
	void (*run)(void *self);
	void (*end)(void *self);
}bank_t;

#endif
