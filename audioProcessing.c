//*****************************************************************************
// file		: audioProcessing.c
// author	: Sylvain Reinauer
// date		: 11.02.18
// purpose	: Projet NTR
//*****************************************************************************

#include "audioProcessing.h"

//*****************************************************************************
// Global variables
//*****************************************************************************
sChannel recBuffer[BUFFER_SIZE] = {0};
uint32 indiceWR = 0;
uint32 indiceRD = 0;

//*****************************************************************************
// Function mode LOOPBACK
// simple original audioLoopback function
// this function don't use mDelay and modulation...
//*****************************************************************************
void audioLoopback(int16 *rxBuf, int16 *txBuf, uint32 numSamples, eModulationMode* modulation, int32* myDelay) {
	memcpy(txBuf,rxBuf,numSamples*sizeof(int16));
  	return;
}

//*****************************************************************************
// Function mode DELAY
// write in recbuffer: current sample of rxBuf
// write in txBuf: current sample of recBuffer + delayed sample of recBuffer
//*****************************************************************************
void fonctionDelay (int16 *rxBuf, int16 *txBuf, uint32 numSamples, eModulationMode* modulation, int32* myDelay){
	uint16 indice;
	pChannel pRxIndice = (pChannel)rxBuf;
	pChannel pTxIndice = (pChannel)txBuf;
	indiceRD = moveIndice(indiceWR, *myDelay, BUFFER_SIZE);

	for(indice = 0; indice < numSamples/NB_CHANNELS; indice++){
		*(recBuffer+indiceWR) = *(pRxIndice+indice);
		(pTxIndice+indice)->channel_L = (*getModulation(modulation))((recBuffer+indiceWR)->channel_L, ((recBuffer+indiceRD)->channel_L >> GAIN_ALPHA));
		(pTxIndice+indice)->channel_R = (*getModulation(modulation))((recBuffer+indiceWR)->channel_R, ((recBuffer+indiceRD)->channel_R >> GAIN_ALPHA));
		indiceRD = moveIndice(indiceRD, ONE_STEP, BUFFER_SIZE);
		indiceWR = moveIndice(indiceWR, ONE_STEP, BUFFER_SIZE);
	}
}

//*****************************************************************************
// Function mode REVERB
// write in recBuffer: current sample of rxBuf + delayed sample of recBuffer
// write in txBuf: current sample in recBuffer
//*****************************************************************************
void fonctionReverb (int16 *rxBuf, int16 *txBuf, uint32 numSamples, eModulationMode* modulation, int32* myDelay){
	uint16 indice;
	pChannel pRxIndice = (pChannel)rxBuf;
	pChannel pTxIndice = (pChannel)txBuf;
	indiceRD = moveIndice(indiceWR, *myDelay, BUFFER_SIZE);

	for(indice = 0; indice < numSamples/NB_CHANNELS; indice++){
		(recBuffer+indiceWR)->channel_L = (*getModulation(modulation))((pRxIndice+indice)->channel_L, ((recBuffer+indiceRD)->channel_L >> GAIN_BETA));
		(recBuffer+indiceWR)->channel_R = (*getModulation(modulation))((pRxIndice+indice)->channel_R, ((recBuffer+indiceRD)->channel_R >> GAIN_BETA));
		(pTxIndice+indice)->channel_L = (recBuffer+indiceWR)->channel_L;
		(pTxIndice+indice)->channel_R = (recBuffer+indiceWR)->channel_R;
		indiceRD = moveIndice(indiceRD, ONE_STEP, BUFFER_SIZE);
		indiceWR = moveIndice(indiceWR, ONE_STEP, BUFFER_SIZE);
	}
}

//*****************************************************************************
// FUNCTION moveIndice
// move index depend of his start position and the size of the buffer
//*****************************************************************************
uint32 moveIndice(uint32 indice, uint32 step, uint32 bufferSize){
	return (indice+step) % bufferSize;
}
//*****************************************************************************
// FUNCTION GetProcess
// all mode are protected by semConfig
// return the right pointer of function selected with processMode
//*****************************************************************************
pFunctionProcess getProcess(eProcessMode* process){
	switch (*process){
		case process_DELAY:
			return fonctionDelay;
		case process_REVERB:
			return fonctionReverb;
		case process_LOOPBACK:
			return audioLoopback;
		default:
			return audioLoopback;
		}
}
//*****************************************************************************
// FUNCTION audioProcess
// call getProcess to have in return a pointer of the right function tu use
// depend of the mode selected
// call the right function with her parameter
//*****************************************************************************
void audioProcess(int16 *rxBuf, int16 *txBuf, uint32 numSamples, eProcessMode* process, eModulationMode* modulation, int32* myDelay) {
	(*getProcess(process))(rxBuf, txBuf, numSamples, modulation, myDelay);
}



