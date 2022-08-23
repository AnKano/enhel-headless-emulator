/************************************************************************/
/*			KEGS: Apple //gs Emulator			                        */
/*			Copyright 2002 by Kent Dickey			                    */
/*									                                    */
/*		This code is covered by the GNU GPL			                    */
/*									                                    */
/*	The KEGS web page is kegs.sourceforge.net			                */
/*	You may contact the author at: kadickey@alumni.princeton.edu	    */
/************************************************************************/

#include <cctype>

#define    SCC_INBUF_SIZE        512        /* must be a power of 2 */
#define    SCC_OUTBUF_SIZE        512        /* must be a power of 2 */

#define SCC_MODEM_MAX_CMD_STR    128

#ifndef SOCKET
# define SOCKET        word32        /* for non-windows */
#endif

STRUCT(Scc) {
    int port;
    int state;
    int accfd;
    SOCKET sockfd;
    int socket_state;
    int rdwrfd;
    void *host_handle;
    void *host_handle2;
    int host_aux1;
    int read_called_this_vbl;
    int write_called_this_vbl;

    int mode;
    int reg_ptr;
    int reg[16];

    int rx_queue_depth;
    byte rx_queue[4];

    int in_rdptr;
    int in_wrptr;
    byte in_buf[SCC_INBUF_SIZE];

    int out_rdptr;
    int out_wrptr;
    byte out_buf[SCC_OUTBUF_SIZE];

    int br_is_zero;
    int tx_buf_empty;
    int wantint_rx;
    int wantint_tx;
    int wantint_zerocnt;
    int dcd;

    double br_dcycs;
    double tx_dcycs;
    double rx_dcycs;

    int br_event_pending;
    int rx_event_pending;
    int tx_event_pending;

    int char_size;
    int baud_rate;
    double out_char_dcycs;

    int socket_num_rings;
    int socket_last_ring_dcycs;
    word32 modem_mode;
    int modem_dial_or_acc_mode;
    int modem_plus_mode;
    int modem_s0_val;
    int telnet_mode;
    int telnet_iac;
    word32 telnet_local_mode[2];
    word32 telnet_remote_mode[2];
    word32 telnet_reqwill_mode[2];
    word32 telnet_reqdo_mode[2];
    int modem_cmd_len;
    byte modem_cmd_str[SCC_MODEM_MAX_CMD_STR + 5];
};

#define SCCMODEM_NOECHO        0x0001
#define SCCMODEM_NOVERBOSE    0x0002

