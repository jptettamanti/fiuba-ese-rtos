#ifndef OVERDRIVE_H
#define OVERDRIVE_H

#include "sound.h"

typedef struct
{
}overdrive_t;

void overdrive_init(void *obj, int td, int fs);
void overdrive_run(void *obj, sound_t *out, sound_t *in);
void overdrive_end(void *obj);

#endif
