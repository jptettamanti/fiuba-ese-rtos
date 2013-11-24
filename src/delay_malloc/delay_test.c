// A simple C example to play a mono or stereo, 16-bit 44KHz
// WAVE file using ALSA. This goes directly to the first
// audio card (ie, its first set of audio out jacks). It
// uses the snd_pcm_writei() mode of outputting waveform data,
// blocking.
//
// Compile as so to create "alsawave":
// gcc -o alsawave alsawave.c -lasound
//
// Run it from a terminal, specifying the name of a WAVE file to play:
// ./alsawave MyWaveFile.wav

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// Include the ALSA .H file that defines ALSA functions/data
#include <alsa/asoundlib.h>

#include "delay.h"

#pragma pack (1)
/////////////////////// WAVE File Stuff /////////////////////
// An IFF file header looks like this
typedef struct _FILE_head
{
	unsigned char	ID[4];	// could be {'R', 'I', 'F', 'F'} or {'F', 'O', 'R', 'M'}
	unsigned int	Length;	// Length of subsequent file (including remainder of header). This is in
									// Intel reverse byte order if RIFF, Motorola format if FORM.
	unsigned char	Type[4];	// {'W', 'A', 'V', 'E'} or {'A', 'I', 'F', 'F'}
} FILE_head;


// An IFF chunk header looks like this
typedef struct _CHUNK_head
{
	unsigned char ID[4];	// 4 ascii chars that is the chunk ID
	unsigned int	Length;	// Length of subsequent data within this chunk. This is in Intel reverse byte
							// order if RIFF, Motorola format if FORM. Note: this doesn't include any
							// extra byte needed to pad the chunk out to an even size.
} CHUNK_head;

// WAVE fmt chunk
typedef struct _FORMAT {
	short				wFormatTag;
	unsigned short	wChannels;
	unsigned int	dwSamplesPerSec;
	unsigned int	dwAvgBytesPerSec;
	unsigned short	wBlockAlign;
	unsigned short	wBitsPerSample;
  // Note: there may be additional fields here, depending upon wFormatTag
} FORMAT;
#pragma pack()






// Size of the audio card hardware buffer. Here we want it
// set to 1024 16-bit sample points. This is relatively
// small in order to minimize latency. If you have trouble
// with underruns, you may need to increase this, and PERIODSIZE
// (trading off lower latency for more stability)
#define BUFFERSIZE	(2*1024)

// How many sample points the ALSA card plays before it calls
// our callback to fill some more of the audio card's hardware
// buffer. Here we want ALSA to call our callback after every
// 64 sample points have been played
#define PERIODSIZE	(2*64)

// Handle to ALSA (audio card's) playback port
snd_pcm_t			*PlaybackHandle;

// Handle to our callback thread
snd_async_handler_t	*CallbackHandle;

// Points to loaded WAVE file's data
unsigned char			*WavePtr;

// Size (in frames) of loaded WAVE file's data
snd_pcm_uframes_t		WaveSize;

// Sample rate
unsigned short			WaveRate;

// Bit resolution
unsigned char			WaveBits;

// Number of channels in the wave file
unsigned char			WaveChannels;

// The name of the ALSA port we output to. In this case, we're
// directly writing to hardware card 0,0 (ie, first set of audio
// outputs on the first audio card)
static const char		SoundCardPortName[] = "plughw:0,0";

// For WAVE file loading
static const unsigned char Riff[4]	= { 'R', 'I', 'F', 'F' };
static const unsigned char Wave[4] = { 'W', 'A', 'V', 'E' };
static const unsigned char Fmt[4] = { 'f', 'm', 't', ' ' };
static const unsigned char Data[4] = { 'd', 'a', 't', 'a' };


inline static print_progress(char* effect_name, int percent)
{
	printf("%s %d%%\r", effect_name, percent);
}

/********************** compareID() *********************
 * Compares the passed ID str (ie, a ptr to 4 Ascii
 * bytes) with the ID at the passed ptr. Returns TRUE if
 * a match, FALSE if not.
 */

static unsigned char compareID(const unsigned char * id, unsigned char * ptr)
{
	register unsigned char i = 4;

	while (i--)
		if ( *(id)++ != *(ptr)++ ) return 0;

	return 1;
}



/********************** waveLoad() *********************
 * Loads a WAVE file.
 *
 * fn =			Filename to load.
 *
 * RETURNS: 0 if success, non-zero if not.
 *
 * NOTE: Sets the global "WavePtr" to an allocated buffer
 * containing the wave data, and "WaveSize" to the size
 * in sample points.
 */

static unsigned char waveLoad(const char *fn)
{
	const char *	message;
	FILE_head	head;
	register int	inHandle;

	if ((inHandle = open(fn, O_RDONLY)) == -1)
	{
		printf("Could not open file\n");
		return 1;
	}

	// Read in IFF File header
	if (read(inHandle, &head, sizeof(FILE_head)) != sizeof(FILE_head))
	{
		printf("Could not read file header\n");
		close(inHandle);
		return 1;
	}

	// Is it a RIFF and WAVE?
	if (!compareID(&Riff[0], &head.ID[0]) || !compareID(&Wave[0], &head.Type[0]))
	{
		printf("File is not a WAVE file\n");
		close(inHandle);
		return 1;
	}

	// Read in next chunk header
	while (read(inHandle, &head, sizeof(CHUNK_head)) == sizeof(CHUNK_head))
	{
		// ============================ Is it a fmt chunk? ===============================
		if (compareID(&Fmt[0], &head.ID[0]))
		{
			FORMAT	format;

			// Read in the remainder of chunk
			if (read(inHandle, &format.wFormatTag, sizeof(FORMAT)) != sizeof(FORMAT)) break;

			// Can't handle compressed WAVE files
			if (format.wFormatTag != 1)
			{
				printf("Compressed WAVE not supported\n");
				close(inHandle);
				return 1;
			}

			WaveBits = (unsigned char)format.wBitsPerSample;
			WaveRate = (unsigned short)format.dwSamplesPerSec;
			WaveChannels = format.wChannels;
		}

		// ============================ Is it a data chunk? ===============================
		else if (compareID(&Data[0], &head.ID[0]))
		{
			// Size of wave data is head.Length. Allocate a buffer and read in the wave data
			if (!(WavePtr = (unsigned char *)malloc(head.Length)))
			{
				printf("File data will not fit in RAM\n");
				close(inHandle);
				return 1;
			}

			if (read(inHandle, WavePtr, head.Length) != head.Length)
			{
				printf("Bad WAVE file\n");
				free(WavePtr);
				close(inHandle);
				return 1;
			}

			// Store size (in frames)
			WaveSize = (head.Length * 8) / ((unsigned int)WaveBits * (unsigned int)WaveChannels);

			close(inHandle);
			return 0;
		}

		// ============================ Skip this chunk ===============================
		else
		{
			if (head.Length & 1) ++head.Length;  // If odd, round it up to account for pad byte
			lseek(inHandle, head.Length, SEEK_CUR);
		}
	}
}



/*********************** free_wave_data() *********************
 * Frees any wave data we loaded.
 *
 * NOTE: A pointer to the wave data be in the global
 * "WavePtr".
 */

static void free_wave_data(void)
{
	if (WavePtr) free(WavePtr);
	WavePtr = 0;
}



/********************** play_audio() **********************
 * Plays the loaded waveform.
 *
 * NOTE: ALSA sound card's handle must be in the global
 * "PlaybackHandle". A pointer to the wave data must be in
 * the global "WavePtr", and its size of "WaveSize".
 */

static void play_audio(void)
{
	register snd_pcm_uframes_t		count, frames;

	// Output the wave data
	for(count = 0; count < WaveSize; count += frames)
	{
		frames = snd_pcm_writei(PlaybackHandle, WavePtr + count, WaveSize - count);

		// If an error, try to recover from it
		if (frames < 0)
			frames = snd_pcm_recover(PlaybackHandle, frames, 0);
		if (frames < 0)
		{
			printf("Error playing wave: %s\n", snd_strerror(frames));
			break;
		}
	}

	// Wait for playback to completely finish
	if (count == WaveSize)
		snd_pcm_drain(PlaybackHandle);
}




/*********************** Add audio effect **********************/
static void add_effect(
	void (*effect_init)(void* obj, int td, int fs),
	void (*effect_run)(void* obj, sound_t *out, sound_t *in),
	void (*effect_end)(void* obj),
	char *effect_name
	)
{
	sound_t 		ModWave1 = 0,ModWave2 = 0;
	snd_pcm_uframes_t	count;
	void *			effect;

	// fs in Hz divided by 100 so that td=1 is set in 10ms intervals
	effect_init(&effect, 10, WaveRate/100);

	print_progress(effect_name, 0);
	// Apply effect
	for(count=0; count < (WaveSize * 8 / WaveBits); count += WaveBits / 8)
	{
		print_progress(effect_name, (count*100) / WaveSize * 8 / WaveBits );

		ModWave1 = *(unsigned short*)(WavePtr + count) - (1 << (WaveBits - 1));

		effect_run(effect, &ModWave2, &ModWave1);

		*(unsigned short*)(WavePtr + count) = ModWave2 + (1 << (WaveBits - 1));
	}
	print_progress(effect_name, 100);
	printf("\n");

	// Free echo data
	effect_end(effect);
}



/*********************** Main program **********************/
int main(int argc, char **argv)
{
	register int		err;

	// No wave data loaded yet
	WavePtr = 0;

	if (argc < 2)
	{
		printf("You must supply the name of a 16-bit mono WAVE file to play\n");
		return 0;
	}

	// Load the wave file
	if (waveLoad(argv[1]))
	{
		printf("Could not load wave file\n");
		return 0;
	}

	if(WaveBits != 16)
	{
		printf("16 bit PCM audio only!!!\n");
		return 0;
	}

	printf("Sample total %d\n", WaveSize);
	printf("Sample rate  %d\n", WaveRate);
	printf("Sample bits  %d\n", WaveBits);
	add_effect(delay_init, delay_run, delay_end, "Delay");
	printf("Finished effect line\n");

	// Open audio card we wish to use for playback
	if ((err = snd_pcm_open(&PlaybackHandle, &SoundCardPortName[0], SND_PCM_STREAM_PLAYBACK, 0)) < 0)
	{
		printf("Can't open audio %s: %s\n", &SoundCardPortName[0], snd_strerror(err));
		return 0;
	}
		
	// Set the audio card's hardware parameters (sample rate, bit resolution, etc)
	if ((err = snd_pcm_set_params(PlaybackHandle, SND_PCM_FORMAT_S16, SND_PCM_ACCESS_RW_INTERLEAVED, WaveChannels, WaveRate, 1, 10000)) < 0)
	{
		printf("Can't set sound parameters: %s\n", snd_strerror(err));
		return 0;
	}

	// Play the waveform
	play_audio();

	// Close sound card
	snd_pcm_close(PlaybackHandle);

	// Free the WAVE data
	free_wave_data();

	return 0;
}
