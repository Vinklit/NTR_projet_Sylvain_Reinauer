//*****************************************************************************
// file		: modulation.c
// author	: Sylvain Reinauer
// date		: 11.02.18
// purpose	: Projet NTR
//*****************************************************************************

#include "modulation.h"

//*****************************************************************************
// Average Channels
// simple function who return the average of tow channels addeded
//*****************************************************************************
int16 functionAverage(int16 channel1, int16 channel2){
	int32 avgChannel = (channel1 + channel2) >> 1;
	return (avgChannel & MASK_16b);
}

//*****************************************************************************
// functionClipping
// simple modulation of clipping audio
//*****************************************************************************
int16 functionClipping(int16 channel1, int16 channel2){
	int32 result = (channel1 + channel2);
	if(result > VALUE16b_MAX){
		result = VALUE16b_MAX;
	}else if(result < VALUE16b_MIN){
		result = VALUE16b_MIN;
	}
	return (result & MASK_16b);
}

//*****************************************************************************
// FUNCTION GetModulation
// all mode are protected by semConfig
// return the right pointer of function selected with modulationMode
//*****************************************************************************
pFunctionModulation getModulation(eModulationMode* modulation){
	switch (*modulation){
		case modulation_AVG:
			return functionAverage;
		case modulation_CLIPPING:
			return functionClipping;
		default:
			return functionClipping;
		}
}

