//*****************************************************************************
// file		: evmAudioTemplate.h
// author	: Sylvain Reinauer
// date		: 11.02.18
// purpose	: Projet NTR
//*****************************************************************************

#ifndef _MODULATION_H_
#define _MODULATION_H_


#include "dsp_types.h"
#include "evmAudioTemplate.h"
#include "audioProcessing.h"

//*****************************************************************************
// Preprocessor symbols
//*****************************************************************************
#define MASK_16b 0xFFFF

//*****************************************************************************
// New types
//*****************************************************************************
typedef int16 (*pFunctionModulation)(int16, int16);

//*****************************************************************************
// Enumerations
//*****************************************************************************
typedef enum{
	modulation_AVG,
	modulation_CLIPPING
}eModulationMode;

//*****************************************************************************
// Function's prototypes
//*****************************************************************************
int16 functionAverage(int16 channel1, int16 channel2);
int16 functionClipping(int16 channel1, int16 channel2);
pFunctionModulation getModulation(eModulationMode* modulation);

#endif // _MODULATION_H_

