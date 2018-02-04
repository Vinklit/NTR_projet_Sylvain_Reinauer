/*
 * File     : audioProcessing.c
 * Author   : Pierre BRESSY
 * Company  : HEIG-VD
 * Created  : Thu Jan 31 11:04:44 2013
 * Purpose  :

 |   Date   | By  | Description of changes in audioProcessing.c
 +----------+-----+-----------------------------------------------------------+
 |          |     |

*/


#include "audioProcessing.h"

//*****************************************************************************
// Variables globales
//*****************************************************************************
sChannel recBuffer[BUFFER_SIZE] = {0};
uint32 indiceWR = 0;
uint32 indiceRD = 0;

//*****************************************************************************
// Fonction mode LOOPBACK
//*****************************************************************************
void audioLoopback(int16 *rxBuf, int16 *txBuf, uint32 numSamples, int32* myDelay) {
	memcpy(txBuf,rxBuf,numSamples*sizeof(int16));
  	return;
}

//*****************************************************************************
// Fonction mode DELAY
//*****************************************************************************
void fonctionDelay (int16 *rxBuf, int16 *txBuf, uint32 numSamples, int32* myDelay){
	uint16 indice;
	pChannel pRxIndice = (pChannel)rxBuf;
	pChannel pTxIndice = (pChannel)txBuf;
	indiceRD = moveIndice(indiceWR, *myDelay, BUFFER_SIZE);

	for(indice = 0; indice < numSamples/NB_CHANNELS; indice++){
		*(recBuffer+indiceWR) = *(pRxIndice+indice);
		(pTxIndice+indice)->channel_L = (recBuffer+indiceWR)->channel_L + ((recBuffer+indiceRD)->channel_L >> GAIN_ALPHA);
		(pTxIndice+indice)->channel_R = (recBuffer+indiceWR)->channel_R+ ((recBuffer+indiceRD)->channel_R >> GAIN_ALPHA);
		indiceRD = moveIndice(indiceRD, ONE_STEP, BUFFER_SIZE);
		indiceWR = moveIndice(indiceWR, ONE_STEP, BUFFER_SIZE);
	}
}

//*****************************************************************************
// Fonction mode REVERB
//*****************************************************************************
void fonctionReverb (int16 *rxBuf, int16 *txBuf, uint32 numSamples, int32* myDelay){
	uint16 indice;
	pChannel pRxIndice = (pChannel)rxBuf;
	pChannel pTxIndice = (pChannel)txBuf;
	indiceRD = moveIndice(indiceWR, *myDelay, BUFFER_SIZE);

	for(indice = 0; indice < numSamples/NB_CHANNELS; indice++){
		(recBuffer+indiceWR)->channel_L = (pRxIndice+indice)->channel_L + ((recBuffer+indiceRD)->channel_L >> GAIN_BETA);
		(recBuffer+indiceWR)->channel_R = (pRxIndice+indice)->channel_R + ((recBuffer+indiceRD)->channel_R >> GAIN_BETA);
		(pTxIndice+indice)->channel_L = (recBuffer+indiceWR)->channel_L;
		(pTxIndice+indice)->channel_R = (recBuffer+indiceWR)->channel_R;
		indiceRD = moveIndice(indiceRD, ONE_STEP, BUFFER_SIZE);
		indiceWR = moveIndice(indiceWR, ONE_STEP, BUFFER_SIZE);
	}
}

//*****************************************************************************
// Automatic gain control
//*****************************************************************************
uint16 autoGainControl(int32 value){
	int8 offsetDone = 0;
	static uint16 offsetMax[NB_CHANNELS] = {0};

	if(value > VALUE16b_MAX){
		offsetMax[offsetDone] = value-VALUE16b_MAX > offsetMax[offsetDone] ? value-VALUE16b_MAX : offsetMax[offsetDone];
	}else if(value < VALUE16b_MIN){
		offsetMax[offsetDone] = abs(value+VALUE16b_MIN) > offsetMax[offsetDone] ? abs(value+VALUE16b_MIN) : offsetMax[offsetDone];
	}else{
		offsetMax[offsetDone] = offsetMax[offsetDone]-1 > 0 ? offsetMax[offsetDone]-1 : 0;
	}
	offsetDone = offsetDone ^ 1;

	return 1;
}

//*****************************************************************************
// FUNCTION moveIndice
//*****************************************************************************
uint32 moveIndice(uint32 indice, uint32 step, uint32 bufferSize){
	return (indice+step) % bufferSize;
}
//*****************************************************************************
// FUNCTION GetProcess
// mode is protected by semConfig
// return the right pointer of function selected with mode
//*****************************************************************************
ptrFonction getProcess(eMode* mode){
	switch (*mode){
		case mode_DELAY:
			return fonctionDelay;
		case mode_REVERB:
			return fonctionReverb;
		case mode_LOOPBACK:
			return audioLoopback;
		default:
			return audioLoopback;
		}
}
//*****************************************************************************
// FUNCTION audioProcess
// call getProcess to have in return a pointer of the right function
//*****************************************************************************
void audioProcess(int16 *rxBuf, int16 *txBuf, uint32 numSamples, eMode* mode, int32* myDelay) {
	(*getProcess(mode))(rxBuf, txBuf, numSamples, myDelay);
}



