/*
 * File     : evmAudioTemplate.h
 * Author   : Pierre BRESSY
 * Company  : HEIG-VD
 * Created  : Thu Jan 31 11:04:44 2013
 * Purpose  :

 |   Date   | By  | Description of changes in evmAudioTemplate.h
 +----------+-----+-----------------------------------------------------------+
 |          |     |

*/
#ifndef _EVM_AUDIO_TEMPLATE_H_
#define _EVM_AUDIO_TEMPLATE_H_

/* DSP/BIOS include files */
#include <std.h>
#include <log.h>
#include <sys.h>
#include <gio.h>
#include <iom.h>

#define DIP0	 	0
#define DIP1 		1
#define DIP2 		2
#define DIP3 		3

#define DIP_UP 		1
#define DIP_DOWN 	0

#define LED0 0
#define LED1 1
#define LED2 2
#define LED3 3

/**	\def BUFALIGN
 *  \brief memory alignment constraint for fast access by DSP **/
#define BUFALIGN      128      /* alignment of buffer for use of L2 cache */
/**	\def NB_BUFFERS
 *  \brief buffers numbers for ping-pong technique **/
#define NB_BUFFERS	  2	       
/**	\def BUFSIZE
 *  \brief size in bytes of one RX or TX buffer **/
#define BUFSIZE (NUMSAMPLES * sizeof(int16)) 
/**	\def NB_CHANNELS
 *  \brief buffer mode 
 * - 1 for mono\n
 * - 2 for stereo 
 **/
/**	\def MINI(a,b)
 *  \brief macro to get the min value between a and b
 */
#define MINI(a,b) (a<b?a:b)
/**	\def MAXI(a,b)
 *  \brief macro to get the max value between a and b
 */
#define MAXI(a,b) (a>b?a:b)


// prototypes
static void startCapture();
static void createChannels();
static void rxCallback(Ptr arg, int32 status, Ptr bufp, uint32 size);
static void txCallback(Ptr arg, int32 status, Ptr bufp, uint32 size);
void setDelay(void);
void setMode(void);

/* Needed to initialize audio driver */
extern int32 edma3init();

/* needed for MEM_alloc in DDR2 segment */
extern int SEG_DDR2;

#endif // _EVM_AUDIO_TEMPLATE_H_

