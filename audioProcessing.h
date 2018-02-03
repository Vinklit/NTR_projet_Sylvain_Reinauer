/*
 * File     : audioProcessing.h
 * Author   : Pierre BRESSY
 * Company  : HEIG-VD
 * Created  : Thu Jan 31 11:04:44 2013
 * Purpose  :

 |   Date   | By  | Description of changes in audioProcessing.h
 +----------+-----+-----------------------------------------------------------+
 |          |     |

*/

#ifndef AUDIOPROCESSING_H_
#define AUDIOPROCESSING_H_

#include "dsp_types.h"

//*****************************************************************************
// Preprocessor symbols
//*****************************************************************************
#define SAMPLING_RATE    		44100     // sampling rate in Hz

#if SAMPLING_RATE<8000
#error "SAMPLING RATE TOO LOW"
#endif

#define NUMSAMPLES	 			4800     // number of samples in TX or RX buffer
#define NB_CHANNELS     		2     	// stereo in buffer
#define BUFFER_SIZE 			(SAMPLING_RATE*2) 	// number of sample for 2s
#define GAIN_ALPHA 				1
#define GAIN_BETA				1
#define STEP_DELAY				4410   // Step to up or down delay ~0.1s
#define DELAY_2S				1
#define NO_DELAY				BUFFER_SIZE
#define VALUE16b_MAX			32767
#define VALUE16b_MIN			-32768

#define NUM_R_SAMPLES (NUMSAMPLES>>1) // number of samples for the left channel
#define NUM_L_SAMPLES (NUMSAMPLES>>1) // number of samples for the right channel

#define LEFT_CHANNEL 1			// offset of first sample of left channel in buffer
#define RIGHT_CHANNEL 0			// offset of first sample of right channel in buffer


//*****************************************************************************
// New types
//*****************************************************************************
typedef void (*ptrFonction)(int16*, int16*, uint32);

//*****************************************************************************
// Enumerations
//*****************************************************************************
typedef enum{
	mode_DELAY,
	mode_REVERB,
	mode_LOOPBACK
}eMode;

//*****************************************************************************
// Strucutres
//*****************************************************************************
typedef struct{
	int16 channel_L, channel_R;
}sChannel, *pChannel;

//*****************************************************************************
// Function's prototypes
//*****************************************************************************
void audioLoopback(int16 *rxBuf, int16 *txBuf, uint32 numSamples);
void audioProcess(int16 *rxBuf, int16 *txBuf, uint32 numSamples, uint32 mDelay, eMode mode);
void fonctionDelay (int16 *rxBuf, int16 *txBuf, uint32 numSamples);
void fonctionReverb (int16 *rxBuf, int16 *txBuf, uint32 numSamples);
uint32 movePointer(uint32 indiceWR, uint32 dDelay);
ptrFonction getProcess(eMode mode);

#endif /* AUDIOPROCESSING_H_ */
