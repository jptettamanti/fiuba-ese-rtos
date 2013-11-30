#ifndef FUZZ_H
#define FUZZ_H

#include "sound.h"

typedef struct
{
}fuzz_t;

void fuzz_init(void *obj, int td, int fs);
void fuzz_run(void *obj, sound_t *out, sound_t *in);
void fuzz_end(void *obj);

#endif
