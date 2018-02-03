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
uint32 thisDelay = NO_DELAY;

//*****************************************************************************
// Fonction mode LOOPBACK
//*****************************************************************************
void audioLoopback(int16 *rxBuf, int16 *txBuf, uint32 numSamples) {
	memcpy(txBuf,rxBuf,numSamples*sizeof(int16));
  	return;
}

//*****************************************************************************
// Fonction mode DELAY
//*****************************************************************************
void fonctionDelay (int16 *rxBuf, int16 *txBuf, uint32 numSamples){
	int16 indice;
	pChannel pRxIndice = (pChannel)rxBuf;
	pChannel pTxIndice = (pChannel)txBuf;
	indiceRD = movePointer(indiceWR, thisDelay);

	for(indice = 0; indice < numSamples/NB_CHANNELS; indice++){
		*(recBuffer+indiceWR) = *(pRxIndice+indice);
		(pTxIndice+indice)->channel_L = (recBuffer+indiceWR)->channel_L + ((recBuffer+indiceRD)->channel_L >> GAIN_ALPHA);
		(pTxIndice+indice)->channel_R = (recBuffer+indiceWR)->channel_R+ ((recBuffer+indiceRD)->channel_R >> GAIN_ALPHA);
		indiceRD = movePointer(indiceRD, 1);
		indiceWR = movePointer(indiceWR, 1);
	}
}

//*****************************************************************************
// Fonction mode REVERB
//*****************************************************************************
void fonctionReverb (int16 *rxBuf, int16 *txBuf, uint32 numSamples){
	int16 indice;
	pChannel pRxIndice = (pChannel)rxBuf;
	pChannel pTxIndice = (pChannel)txBuf;
	indiceRD = movePointer(indiceWR, thisDelay);

	for(indice = 0; indice < numSamples/NB_CHANNELS; indice++){
		(recBuffer+indiceWR)->channel_L = (pRxIndice+indice)->channel_L + ((recBuffer+indiceRD)->channel_L >> GAIN_BETA);
		(recBuffer+indiceWR)->channel_R = (pRxIndice+indice)->channel_R + ((recBuffer+indiceRD)->channel_R >> GAIN_BETA);
		(pTxIndice+indice)->channel_L = (recBuffer+indiceWR)->channel_L;
		(pTxIndice+indice)->channel_R = (recBuffer+indiceWR)->channel_R;
		indiceRD = movePointer(indiceRD, 1);
		indiceWR = movePointer(indiceWR, 1);
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
// movePointer
//*****************************************************************************
uint32 movePointer(uint32 position, uint32 step){
	return (position+step)%BUFFER_SIZE;
}
//*****************************************************************************
// GetProcess
//*****************************************************************************
ptrFonction getProcess(eMode mode){
	switch (mode){
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
// audioProcess
//*****************************************************************************
void audioProcess(int16 *rxBuf, int16 *txBuf, uint32 numSamples, uint32 dDelay, eMode mode) {
	thisDelay = dDelay;
	(*(getProcess(mode)))(rxBuf, txBuf, numSamples);
}



