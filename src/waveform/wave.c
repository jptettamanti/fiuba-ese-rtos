#include "wave.h"

sound_t sawtooth()
{
	sawtooth->wave = (sawtooth->wave + f) % fs;
}

sound_t triangle()
{
	value = 2 * sawtooth();

	if(value > SOUND_MAX/2)
		value = SOUND_MAX - (2 * value);

	if(value < SOUND_MIN/2)
		value = SOUND_MIN - (2 * value);
}

sound_t sine()
{

}