/************************************************************************/
/*			KEGS: Apple //gs Emulator			                        */
/*			Copyright 2002 by Kent Dickey			                    */
/*									                                    */
/*		This code is covered by the GNU GPL			                    */
/*									                                    */
/*	The KEGS web page is kegs.sourceforge.net			                */
/*	You may contact the author at: kadickey@alumni.princeton.edu	    */
/************************************************************************/

#define SOUND_SHM_SAMP_SIZE        (32*1024)

#define SAMPLE_SIZE                2
#define NUM_CHANNELS               2
#define SAMPLE_CHAN_SIZE           (SAMPLE_SIZE * NUM_CHANNELS)

STRUCT(Doc_reg) {
    double dsamp_ev;
    double dsamp_ev2;
    double complete_dsamp;
    int samps_left;
    word32 cur_acc;
    word32 cur_inc;
    word32 cur_start;
    word32 cur_end;
    word32 cur_mask;
    int size_bytes;
    int event;
    int running;
    int has_irq_pending;
    word32 freq;
    word32 vol;
    word32 waveptr;
    word32 ctl;
    word32 wavesize;
    word32 last_samp_val;
};

void generic_sound_init(word32 *shmaddr);
void generic_sound_shutdown();
void child_sound_init();
int generic_send_audio(byte *ptr, int in_size);
