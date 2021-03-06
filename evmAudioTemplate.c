//*****************************************************************************
// file		: evmAudioTemplate.c
// author	: Sylvain Reinauer
// date		: 11.02.18
// purpose	: Projet NTR
//*****************************************************************************

/* runtime include files */
#include <stdio.h>
#include <string.h>
#include <math.h>

/* DSP/BIOS include files */
#include <std.h>
#include <log.h>
#include <sys.h>
#include <gio.h>
#include <iom.h>

/* EVM DM6437 include files */
#include "evmdm6437_dip.h"
#include "evmdm6437_led.h"

/* DSP/BIOS objects include files (from .tcf file) */
#include "evmAudioTemplatecfg.h"

/* Application include files */
#include "evmAudioTemplate.h"

/** \brief Ping-pong receive buffers to be used for transfers 
 *
 * rxBuf is a dual buffer containing mixed stereo input
 *
 * dual buffer => rxBuf[0] contains NUMSAMPLES mixed stereo samples\n
 *                rxBuf[1] contains NUMSAMPLES mixed stereo samples\n
 *                driver uses rxBuf[0] to store data, then swap to [1]...\n
 *                a semaphor is used to protect access to unsued buffer\n
 *                rxDone is used to point on usable buffer for processing\n
 * 
 * mixed stereo input (or ouput) =>\n
 *    data type is int16 (signed 16 bits),\n 
 *    L=left, R=right\n
 *    |Sample L 0|Sample R 0|Sample L 1|Sample R 1|Sample L 2|Sample R 2|...\n
 *
 * PRAGMA : rx buffers are aligned on x128bytes (driver constraint)\n
 *
 * NB_CHANNELS = 2 for stereo, left and right\n
 * if NUMSAMPLES = 1024 => 512 samples for left channel mixed with 512 samples for right channel\n
 */
// default: globals are in L2CACHE of DSP 
#pragma DATA_ALIGN(rxBuf, BUFALIGN)
int16 rxBuf[NB_BUFFERS][NUMSAMPLES] = {0};


/** \brief Ping-pong transmit buffers to be used for transfers 
 *
 * txBuf is a dual buffer containing mixed stereo input
 *
 * dual buffer => rxBuf[0] contains NUMSAMPLES mixed stereo samples\n
 *                rxBuf[1] contains NUMSAMPLES mixed stereo samples\n
 *                driver uses rxBuf[0] to store data, then swap to [1]...\n
 *                a semaphor is used to protect access to unsued buffer\n
 *                rxDone is used to point on usable buffer for processing\n
 * 
 * mixed stereo input (or ouput) =>\n
 *    data type is int16 (signed 16 bits),\n 
 *    L=left, R=right\n
 *    |Sample R 0|Sample L 0|Sample R 1|Sample L 1|Sample R 2|Sample L 2|...\n
 *
 * PRAGMA : tx buffers are aligned on x128bytes (driver constraint)\n
 *
 * NB_CHANNELS = 2 for stereo, left and right\n
 * if NUMSAMPLES = 1024 => 512 samples for left channel mixed with 512 samples for right channel\n
 */
// default: globals are in L2CACHE of DSP 
#pragma DATA_ALIGN(txBuf, BUFALIGN)
int16 txBuf[NB_BUFFERS][NUMSAMPLES] = {0};

/** \brief rxDone is used to keep track of the usage of the rx buffers */
int32 rxDone = 0;
/** \brief txDone is used to keep track of the usage of the tx buffers */
int32 txDone = 0;
/** \brief inChan is a GIO handle created in main */
GIO_Handle      inChan;
/** \brief outChan is a GIO handle created in main */
GIO_Handle      outChan;
/** \brief  appReadCb is a pointer on Callback function for driver */
GIO_AppCallback appReadCb;
/** \brief  appWriteCb is a pointer on Callback function for driver */
GIO_AppCallback appWriteCb;


//*****************************************************************************
// Global variables
//*****************************************************************************
uint8 statusDIP0 = DIP_UP;
uint8 statusDIP1 = DIP_UP;
uint8 statusDIP2 = DIP_UP;
uint8 statusDIP3 = DIP_UP;

eProcessMode modeProcess = process_LOOPBACK;
eModulationMode modeModulation = modulation_CLIPPING;
int32 myDelay = NO_DELAY;

/**
 * \fn void main(void)
 * \brief Entry point of the software
 * - initialize LED and DIP switches interface
 * - initialize EDMA for data transfert of audio driver
 * - set Callback functions
 */
void main(void) {

	EVMDM6437_init();
  	EVMDM6437_LED_init();
	EVMDM6437_DIP_init( );

	EVMDM6437_LED_off(LED0);
	EVMDM6437_LED_off(LED1);
	EVMDM6437_LED_off(LED2);
	EVMDM6437_LED_off(LED3);

	edma3init();

    printf("Audio Processing started\n");

	appReadCb.fxn =  rxCallback;
	appWriteCb.fxn = txCallback;

	return;
}

/**
 * \fn void audiotTask()
 * \brief main audio task
 * - create I/O streams
 * - start driver activity	
 * - loop forever to process input buffer
 * \callgraph
 */
void audiotTask()
{
  uint32 rxSize = BUFSIZE;
  uint32 txSize = BUFSIZE;
	

  /* create I/O streams */
  createChannels();
    
  /* start driver activity */
  startCapture();
	
  /* Loop forever looping back buffers */
  while(1) {

    /* Waiting for receive buffer and transmit buffer */
    SEM_pend(&semRx, SYS_FOREVER);					// attente sem rx
    SEM_pend(&semTx, SYS_FOREVER);					// attente sem tx
	SEM_pend(&semConfig, SYS_FOREVER); 				// wait on semConfig

	audioProcess(rxBuf[rxDone],txBuf[txDone],NUMSAMPLES, &modeProcess, &modeModulation, &myDelay);
	
	SEM_post(&semConfig);							// release semConfig
  			
 	// Release input and output buffers to audio drivers
    GIO_submit(inChan, IOM_READ, rxBuf[rxDone], &rxSize, &appReadCb);
    GIO_submit(outChan, IOM_WRITE, txBuf[txDone], &txSize, &appWriteCb);

	// swap buffer 0<->1
    rxDone = rxDone ^ 1;    /* XOR for rxDone */            
    txDone = txDone ^ 1;    /* XOR for txDone */
  }
}

//*****************************************************************************
// TASK getDipStatusTask
// -statusDIPx are protected by semaphore (semDip)
// -wait on semDip
// -read status of all dip switch
// -release semDip
//*****************************************************************************
void getDipStatusTask(int arg0, int arg1){
	while(1){
		SEM_pend(&semDip, SYS_FOREVER);
		statusDIP0 = EVMDM6437_DIP_get(DIP0);		// Increase delay
		statusDIP1 = EVMDM6437_DIP_get(DIP1);		// Reduce delay
		statusDIP2 = EVMDM6437_DIP_get(DIP2);		// Modulation mode 	0->modulation_AVG, 1->modulation_CLIPPING
		statusDIP3 = EVMDM6437_DIP_get(DIP3);		// Process mode		0->process_DELAY, 1->process_REVERB
		SEM_post(&semDip);
		TSK_sleep(100);
	}
}

//*****************************************************************************
// TASK configProcessTask
// -wait of semDip and semConfig
// -configure delay and mode
// -release semDip and semConfig
//*****************************************************************************
void configProcessTask(int arg0, int arg1){
	while(1){
		SEM_pend(&semDip, SYS_FOREVER);
		SEM_pend(&semConfig, SYS_FOREVER);
		setDelay();
		setProcessMode();
		setModulationMode();
		SEM_post(&semDip);
		SEM_post(&semConfig);
		TSK_sleep(100);
	}
}

//*****************************************************************************
// FUNCTION setDelay
// -statusDIPx and myDelay are protected by semaphore (semDip and semConfig)
// -DIP0 down -> increase Delay until DELAY_MAX (2s)
// -DIP1 down -> reduce Delay until NO_DELAY (0s)
//*****************************************************************************
void setDelay(void){
	if(statusDIP0 == DIP_DOWN && statusDIP1 == DIP_UP){
			//Increase delay by STEP_DELAY until DELAY_MAX (2s)
			myDelay = (myDelay-STEP_DELAY) <= DELAY_MAX ? DELAY_MAX : myDelay-STEP_DELAY;
		}
	else if(statusDIP0 == DIP_UP && statusDIP1 == DIP_DOWN){
			//Reduce delay by STEP_DELAY until NO_DELAY
			myDelay = (myDelay+STEP_DELAY) >= NO_DELAY ? NO_DELAY : myDelay+STEP_DELAY;
		}
}

//*****************************************************************************
// FUNCTION setMode
// -statusDIP3, mode and myDelay are protected by semaphore (semDip and semConfig)
// -if delay 				== 0s 	-> process_LOOPBACK
// -else if status DIP3 	== 0 	-> process_DELAY
// -else if status DIP3 	== 1	-> process_REVERB
//*****************************************************************************
void setProcessMode(void){
		modeProcess = (myDelay == NO_DELAY) ? process_LOOPBACK : (eProcessMode)statusDIP3;
}

//*****************************************************************************
// FUNCTION setModulationType
// -statusDIP2, modulationType are protected by semaphore (semDip and semConfig)
// -if status DIP2 	== 0 	-> modulation_AVG
// -if status DIP2 	== 1	-> modulation_CLIPPING
//*****************************************************************************
void setModulationMode(void){
		modeModulation = statusDIP2 ? modulation_CLIPPING : modulation_AVG;
}


/**
 * \fn void rxCallback(Ptr arg, int32 status, Ptr bufp, uint32 size)
 * \brief function called when RX driver is ready to process next buffer
 * - Callback function for the IOM_WRITE channel
 * - post semRx semaphore to release Rx buffer
 * \callgraph
 */
void rxCallback(Ptr arg, int32 status, Ptr bufp, uint32 size)
{
  SEM_post(&semRx);                   /* Post semaphore for the rx channel */
}

/**
 * \fn void txCallback(Ptr arg, Int status, Ptr bufp, uint32 size)
 * \brief function called when TX driver is ready to process next buffer
 * - Callback function for the IOM_WRITE channel
 * - post semTx semaphore to release Tx buffer
 * \callgraph
 */
void txCallback(Ptr arg, Int status, Ptr bufp, uint32 size)
{
  SEM_post(&semTx);                   /* Post semaphore for the tx channel */
}

/**
 * \fn void createChannels()
 * \brief create input and output channel for acquisition
 * \callgraph
 */
void createChannels()
{
  outChan = GIO_create("/udevCodec", IOM_OUTPUT, NULL, NULL, NULL);
  if (outChan == NULL) {
    SYS_abort("Create output channel FAILED.");
  }

  inChan  = GIO_create("/udevCodec", IOM_INPUT, NULL, NULL, NULL);
  if (inChan == NULL) {
    SYS_abort("Create input channel FAILED.");
  }
}

/**
 * \fn void startCapture()
 * \brief initialize buffers and associate buffers to audio driver
 * \callgraph
 */
void startCapture()
{ 
  uint32 rxSize = BUFSIZE;
  uint32 txSize = BUFSIZE;
 
  /** Initialize buffers of silence for the output */    
  memset(txBuf[0], 0, sizeof(txBuf[0]));
  memset(txBuf[1], 0, sizeof(txBuf[1]));       
     
  if (GIO_submit(inChan, IOM_READ, rxBuf[0], &rxSize, &appReadCb) < 0) {
    SYS_abort("GIO_submit of rxBuf[0] failed");
  } 
  if (GIO_submit(inChan, IOM_READ, rxBuf[1], &rxSize, &appReadCb) < 0) {
    SYS_abort("GIO_submit of rxBuf[1] failed");
  } 
     
  if (GIO_submit(outChan, IOM_WRITE, txBuf[0], &txSize, &appWriteCb) < 0) {
    SYS_abort("GIO_submit of txBuf[0] failed");
  } 
  if (GIO_submit(outChan, IOM_WRITE, txBuf[1], &txSize, &appWriteCb) < 0) {
    SYS_abort("GIO_submit of txBuf[1] failed");
  } 
     
}

void alive() {
	while(1) {
		EVMDM6437_LED_toggle(LED0);
		TSK_sleep(1000);
	}
}
