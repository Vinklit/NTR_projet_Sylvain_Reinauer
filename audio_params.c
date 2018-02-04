/*
 * ======== audio_params.c ========
 *
 */
#include <psp_audio.h>
#include "dsp_types.h"
#include "audioProcessing.h"
#include "evmAudioTemplate.h"

PSP_audio_cfg   audio_cfg = {
        PSP_AUDIO_AIC_MASTER,   /* AIC Mode */
        99000000,               /* Input Frequency to McBSP or McASP module */
        SAMPLING_RATE,                     /* Sampling Rate */
        30,                     /* Input Gain */
        80,                     /* Output Gain */
        PSP_AUDIO_LINE_IN,      /* Input Select */
        PSP_AUDIO_LINE_OUT      /* Output Select */
};
