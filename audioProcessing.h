//*****************************************************************************
// file		: audioProcessing.h
// author	: Sylvain Reinauer
// date		: 11.02.18
// purpose	: Projet NTR
//*****************************************************************************

#ifndef AUDIOPROCESSING_H_
#define AUDIOPROCESSING_H_

#include "dsp_types.h"
#include "evmAudioTemplate.h"
#include "modulation.h"


//*****************************************************************************
// Preprocessor symbols
//*****************************************************************************
#define SAMPLING_RATE    		44100     			// sampling rate in Hz

#if SAMPLING_RATE<8000
#error "SAMPLING RATE TOO LOW"
#endif

#define NUMSAMPLES	 			4800     			// number of samples in TX or RX buffer
#define NB_CHANNELS     		2     				// stereo in buffer

#define BUFFER_SIZE 			(SAMPLING_RATE*2) 	// number of sample for 2s
#define GAIN_ALPHA 				1
#define GAIN_BETA				1
#define STEP_DELAY				4410   				// Step to up or down delay ~0.1s
#define ONE_STEP 				1
#define DELAY_MAX				1
#define NO_DELAY				BUFFER_SIZE

#define VALUE16b_MAX			32767
#define VALUE16b_MIN			-32768
#define VALUE_PEAK_MAX 			20000

#define NUM_R_SAMPLES (NUMSAMPLES>>1) 				// number of samples for the left channel
#define NUM_L_SAMPLES (NUMSAMPLES>>1)				// number of samples for the right channel

#define LEFT_CHANNEL 1								// offset of first sample of left channel in buffer
#define RIGHT_CHANNEL 0								// offset of first sample of right channel in buffer


//*****************************************************************************
// New types
//*****************************************************************************
typedef void (*pFunctionProcess)(int16*, int16*, uint32, int32*, int*);

//*****************************************************************************
// Enumerations
//*****************************************************************************
typedef enum{
	process_DELAY,
	process_REVERB,
	process_LOOPBACK
}eProcessMode;

//*****************************************************************************
// Strucutres
//*****************************************************************************
typedef struct{
	int16 channel_L, channel_R;
}sChannel, *pChannel;

//*****************************************************************************
// Function's prototypes
//*****************************************************************************
void audioProcess(int16 *rxBuf, int16 *txBuf, uint32 numSamples, eProcessMode* process, int* modulation, int32* myDelay);
void audioLoopback(int16 *rxBuf, int16 *txBuf, uint32 numSamples, int* modulation, int32* myDelay);
void fonctionDelay(int16 *rxBuf, int16 *txBuf, uint32 numSamples, int* modulation, int32* myDela);
void fonctionReverb(int16 *rxBuf, int16 *txBuf, uint32 numSamples, int* modulation, int32* myDelay);
uint32 moveIndice(uint32 indice, uint32 step, uint32 bufferSize);
pFunctionProcess getProcess(eProcessMode* process);

#endif /* AUDIOPROCESSING_H_ */
