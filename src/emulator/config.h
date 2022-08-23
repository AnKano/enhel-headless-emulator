/************************************************************************/
/*			KEGS: Apple //gs Emulator			                        */
/*			Copyright 2002 by Kent Dickey			                    */
/*									                                    */
/*		This code is covered by the GNU GPL			                    */
/*									                                    */
/*	The KEGS web page is kegs.sourceforge.net			                */
/*	You may contact the author at: kadickey@alumni.princeton.edu	    */
/************************************************************************/

#define CONF_BUF_LEN 1024
#define COPY_BUF_SIZE 4096
#define CFG_PRINTF_BUFSIZE 2048

#define CFG_PATH_MAX 1024

#define CFG_NUM_SHOWENTS 16

#define CFGTYPE_MENU 1
#define CFGTYPE_INT 2
#define CFGTYPE_DISK 3
#define CFGTYPE_FUNC 4
#define CFGTYPE_FILE 5
/* CFGTYPE limited to just 4 bits: 0-15 */

/* Cfg_menu, Cfg_dirent and Cfg_listhdr are defined in defc.h */

STRUCT(Cfg_defval) {
    Cfg_menu *menuptr;
    int intval;
    char *strval;
};
