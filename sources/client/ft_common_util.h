/*  $Id: ft_common_util.h 199 2014-02-06 22:19:38Z quick $  */

/*
 * Copyright 2013 SRC Computers, LLC.  All Rights Reserved.
 *
 *	Manufactured in the United States of America.
 *
 * SRC Computers, LLC.
 * 4240 N Nevada Avenue
 * Colorado Springs, CO 80907
 * (v) (719) 262-0213
 * (f) (719) 262-0223
 *
 * No permission has been granted to distribute this software
 * without the express permission of SRC Computers, LLC.
 *
 * This program is distributed WITHOUT ANY WARRANTY OF ANY KIND.
 */
/* **********************************************************************************+
 */
/* * Options:
 *       
 *       */
#ifndef FT_COMMON_UTIL_H
#define FT_COMMON_UTIL_H 1

#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include "map.h"

#include "ft_user_logic.h"

/* **********************************************************************************
 * Definitions 
 * ******************************************************************************** */
/* **********************************************************************************
 * Union type Definitions 
 * ******************************************************************************** */
/* IEEE 754 based format 
 *  Single precision:
 *  63                                                                   0
 *  |-|--------|-----------------------|-|--------|-----------------------|
 *       Exp1             Sig1              Exp0             Sig0
 *   Hi Order word float [1]              Low Order word float [0]
 *   Left (L)                             Right (R)     
 *    The left and right also matches the parameter order in the Split and combine 
 *    procedures. i.e. the left parameter is the same as the L word.
 * */
/* 
 * Union definition 
 */
typedef unsigned           uint32;
typedef unsigned short     uint16;
typedef unsigned char      uint8;

typedef float     real32;
typedef double    real64;

union mapbuf {
  uint64_t     quad;
  uint32_t     dword [2];
  uint16_t     word [4];
  uint8_t      byte [8];
  real64       fp64;
  real32       fp32[2];
  struct {
    long int wordR; 
    long int wordL;  
  } i;
  struct {
    uint32_t wordR; 
    uint32_t wordL;  
  } w;
  struct {
    real32 wordR; 
    real32 wordL;  
  } f;
  struct { /* IEEE 754 based format */
    unsigned        fp32_Sig0:23; 
    unsigned        fp32_Exp0:8; 
    unsigned        fp32_sign0:1; 
    unsigned        fp32_Sig1:23; 
    unsigned        fp32_Exp1:8; 
    unsigned        fp32_sign1:1; 

  } IEEE_32;
  struct { /* IEEE 754 based format */
    unsigned long long   Sig:52; 
    unsigned             Exp:11; 
    unsigned             sign:1; 

  } IEEE_64;

};

union mapbuf W;


/* 
 * SCA overlay definition
 */

union sca_t {
  uint64_t     quad[SCA_NUMBER_OF_WORDS];
  MAP_SCA_Status_t  sca;
};
/* 
 * the  CM status overlay definition
 */

union CM_Status_t {
  uint64_t     quad[16];
  MAP_CM_Status_t cm;
};


/* 
 * Define the MAP and CM status for use in the SRC7 system info
 */

union Device_Status_t {
  uint64_t     quad[SCA_NUMBER_OF_WORDS];
  MAP_SCA_Status_t  sca;
  struct {
    MAP_CM_Status_t cm;
    uint64_t          filler[SCA_NUMBER_OF_WORDS-16];
  } CM;
};

  union CM_Status_t cm;


union MAP_Rev_t { /* MAP  number */
  uint64_t     quad;
  struct {
    unsigned       RFU0:10;                  /* 00 - 09 */ 
    unsigned       Firmware_Minor:5;         /* 10 - 14 */ 
    unsigned       Firmware_Major:5;         /* 15 - 19 */ 
    unsigned       RFU1:12;                  /* 20 - 31 */ 
    unsigned       RFU32:32;                 /* 32 - 63 */         
  } alt;
};




/* *****
 * log file definition
 * assume that the logfile will be defined at MAP allocate,
 * and use a different FILE variable for the dv output.
 * Base on the DVLOGFILE environment variable for the file name and
 * create in the Initialize_Parameters_Common procedure called by everyone.
 * The Log_File_Defined logical is for the DVLOGFILE file with dv_logfile handle.
 * 
 * */
FILE *dv_logfile;

int     Log_File_Defined;           /* true if a log file was defined.
                                     * need to remove the test suite version*/

/* *******************************************
 * Series H; J and L definitions.
 *
 * User logic Clock conversion to seconds
 * (1000/user logic clock MHz) * 10E-9 = seconds.
 * Series H: 1000/150 MHz = 6.6666 * 10E-9 = 6.6666E-9 seconds.
 * Series J: 1000/200 MHz = 5.0 * 10E-9    = 5.0E-9 seconds
 *
 * NOTE: The MAP L defines are not known yet. 
 * 
 * *******************************************/
/* MAP L and M values */
#define CLOCKS_TO_SECONDS     5.0E-9   /* 1000/200 MHz = 5.0 * 10E-9 = seconds. 200 mHz Series J and L clock. */
#define MAX_BRAM_STORAGE      131072   /* qw amount of BRAM used in test case 8.3. 4x max BRAM array */
//#define BRAM_ARRAY_MAX          32768  /* Max array qw size. defined in ft_user_logic.h */



/* *******************************************
 * Misc Definitions
 * Banks are actually made up of SRAM's 
 * OBM_BANK_SIZE 
 * OBM_NUM_BANKS 4
 * *******************************************/

/* Data Sizes and conversions */
#define KBYTES                1024
#define MBYTES                1048576     /* 1024 * 1024 */
#define GBYTES                1073741824  /* 1024 * 1024 * 1024 */
#define QWORDS_TO_MEGABYTES   131072      /* divide by 131072 =  (1024 * 1024)/8 */
#define QWORDS_TO_KILOBYTES   128         /* divide by  =  (1024)/8 */
#define QWORDS_TO_GIGIBYTES   134217728   /* divide by  */
#define MEGABYTES_TO_QWORDS   131072      /* Multiply by 131072 */
#define GIGIBYTES_TO_QWORDS   134217728   /* Multiply by  */
#define BYTES_TO_MEGAQWORDS   0x800000    /* Divide by 1024 * 1024 * 8 */
#define QWORDS_TO_MEGAQWORDS  0x100000    /* Divide by 1048576 = 1024 * 1024 */
#define BYTES_TO_GIGIBYTES    0x40000000   /* Divide by 1024 * 1024 * 1024 = 1073741824 */
#define SEC2MICRO             1000000
#define CM_QWORD_SIZE        (512 * MEGABYTES_TO_QWORDS)  /* 512 MB Default On-board Common Memory Size */


/* MAP H values */

#define CACHE_SIZE 16                      /* Saturn Cache size */
#define VLM_CACHE_SIZE 4                  /*  Saturn VLM size */

#define DMA_PACKET_SIZE 16                /* MAP DMA Packet size */
#define CM_PACKET_SIZE  16                /* Global CM Packet size */

//@@ need
#define CM_SIZE_QWORD_NORMAL (4096 * MEGABYTES_TO_QWORDS) /* Current CM Size Qwords 512 M, 4Gbytes */
#define CM_SIZE_QWORD        (8192 * MEGABYTES_TO_QWORDS) /* Current CM Size Qwords 1024 M, 8Gbytes */
#define OBM_SIZE             (OBM_BANK_SIZE * OBM_NUM_BANKS) /* OBM size */
#define OBM_BUFFER_SIZE      (OBM_BANK_SIZE * 3) /* Half OBM size 3 banks */

/* Local system limits. */
#define MAX_MEGABYTES         200        /* Max RAM to allocate */
/*
 * SNAP transfer Limit (as of 1/5/2012)
 *  SNAP => GCM  not limited (SNAP chunks up data)
 *  GCM  => SNAP limited to 128 MB
 *  GCM  <=> MAP unlimited streamed
 *  MAP  <=> SNAP limited to 128 MB
 *  
 * */
#define MAX_SNAP_MEGABYTES    128        /* Max RAM to allocate to match SNAP transfer*/

#define MAX_LOCAL_BUFFER_QWORDS  (MAX_MEGABYTES * MEGABYTES_TO_QWORDS)  
                                         /* Max local buffer to allocate  */
#define MAX_DMA_SIZE_QWORDS  (MAX_SNAP_MEGABYTES * MEGABYTES_TO_QWORDS)  
                                   /* Max DMA buffer allowed by SNAP temporary 4/2011*/
#define OBM_DMA_BANKS    OBM_NUM_BANKS   /* Number of OBM banks that are DMA-able */
#define OBM_STRIDE_BANKS    10           /* Number banks in stride  */

#define MAX_PORTS             0xf55  /* total possible nodes for 2 tier 6 port switch, and 16 port 1st tier */

/* from old */
#define GETLINE_SIZE  1024           /* number of characters, need to handle big lines */
#define STDIN_SIZE    1024           /* Buffer size for snap output */

#define MAP_LIST_NULL      -1        /*  */
#define NAME_SIZE     128
#define OPTION_SIZE   256            /* Use for the option display lines */



/* ** 
 * Data Sizes used by the bit error rate calculation, found in map.h
 * All sizes in bytes.
 * 
 * MAP_SCA_Status_Transfer_Size
 * MAP_SCA_Param_Transfer_Size
 * 
 * */


int        MAP_Board_Number; /* Series MAP number the binary was compiled for */
map_types  MAP_Board_Enum; /* Series MAP enumeration the binary was compiled for */

/* *****
 * Output mode definitions
 * DV_OUTPUT_MODE
 * */
#define DV_OUTPUT_PRODUCTION  1
#define DV_OUTPUT_ORIGINAL    0

#define DEFAULT_COMPARE_CONTEXT_WINDOW   5 /* Size of context window for display =4 */

int   DV_Output_Mode;             /* Test Output Mode use Compare_Production */
int   DV_Display_Error_Mode;      /* Test Mode use colors for failure  */
int   DV_Display_Sections;        /* Test Mode use AR and ER display section  */
int   DV_Compare_Context_Window;  /* Context window for display 4  */


/* **************************************************************************
 * Error Definitions
 * ERROR_WITH_DATA (255) and ERROR_DURING_SETUP (110)
 * are the two test case generated errors that are returned as exit codes.
 * The other codes would be in the display. 
 * *******************************************/
#define NO_ERROR                 0
#define MEMORY_ERROR             0     /* The CM allocate failed, returns a NULL. */
#define ERROR_HAPPENED          -1     /* Generic failure,  something failed. */
//#define ERROR_WITH_DATA          1     /* Data error. */
//#define ERROR_WITH_DATA          -1     /* Data error. Use -1 to work better with runtps7,
//                                         * maplib uses 1 as error also need to be different */
//#define ERROR_WITH_DATA          -2     /* Data error. Use -2 to work better with the python,
//                                         * maplib uses 1 as error also need to be different */
//#define ERROR_WITH_DATA          255     /* Data error. Use 255 is what the OS sees as unsigned byte,
//                                         * maplib uses 1 as error also need to be different */
#define ERROR_WITH_DATA          254     /* Data error. Use 254 is what the OS sees as unsigned byte -2,
                                         * maplib uses 1 as error also need to be different */
#define ERROR_DURING_SETUP       110  /* basic error from test cases  */

#define ERROR_SYSTEM_FAILURE     2     /* System failure. */

#define ERROR_SCA_NOT_AVAILABLE  -1     /* map_read_sca_status failed SCA not available */

#define ERROR_Invalid_Selection  102  /* Bad Case selection or test case not valid for binary */

/* normally  setup.*/
#define ERROR_OPENING_FILE       128  /* File not found used by the FT python scripts*/
#define ERROR_READING_FILE       106
//
#define ERROR_DURING_EXECUTION   111 /*  */

#define ERROR_Memory_Allocation  113  /* Failure to allocate memory  */
#define ERROR_MAP_Allocation     112  /* Failure to allocate a MAP  */

/* Status check Error  */
#define ERROR_SBE_Detected          120  /* An CM/Switch status detected a SBE  */
#define ERROR_MBE_Detected          121  /* An CM/Switch status detected a MBE  */
#define ERROR_Address_MBE_Detected  122  /* An CM/Switch status detected an Address MBE  error */
#define ERROR_Other_Detected        123  /* An CM/Switch status detected an other error */

/* Display limits */

#define File_Error_Limit         0x50  /* Max to write to file on error */
#define Display_Error_Limit      0x1a   /* Max to display on error */
#define Display_Header_Limit     0x20  /* Lines to display without header */

#define Display_Debug_Limit      0xc   /* Max data to display for debug */
//
uint64_t  No_Result;                   /* Ignore value for success/failure.*/
#define OBM_PATTERN        0x00ba00000000baadULL   /* OBM background pattern.  */
#define CM_PATTERN         0xcc0000000000baadULL   /* CM background pattern.   */
#define LOCAL_PATTERN      0x10ca10000000baadULL   /* Local  background pattern.   */
#define LOCAL_PATTERN2     0x10ca1000baadbaadULL   /* Local  background pattern.   */
/* from old */
#define File_Trace_Limit        0x40 /* Max to write to file on trace*/
#define Display_Trace_Limit     0x8  /* Max to dispay on Trace 4 */
#define Display_Trace5_Limit    0x30  /* Max to dispay on Trace 5 */
  


/* *******************************************
 * Boolean 
 * *******************************************/
#define FALSE 0
#define TRUE  1
#define FLAG(s) s?"TRUE":"FALSE"
#define BOOLEAN(s) s?"TRUE":"FALSE"
#define MEMORY_MODEL(s) s?"COMMON":"LOCAL"
#define FIRMWARE_LEVEL(s) s?"Release":"  Lab"

/* *******************************************
 * Data_Load_Flag type
 *   Add define here
 *   Add pattern generate in Load_Data procedure. 
 *   Add pattern name in Display_Data_Pattern procedure.
 *   Add to help CMN_DATA_PATTERNS in common_util.h
 *
 *   NOTE:  types defined in ft_user_logic.h which is
 *          included into the user logic (.mc)
 *   
 * *******************************************/
/* Random Lockeed Martin pattern related */
#define LM_VALUE_RANGE    256  /* based on range around zero of 94% LM data set*/
#define LM_SMALL_DATA_LIMIT     32  /* data below this is very little zeros*/
#define LM_ZEROS_MAX     6293  /* based on LM data set*/
#define LM_ZEROS_MIN        1  /* based on LM data set*/
#define LM_DATA_MAX      1921  /* based on LM data set*/
#define LM_DATA_MIN         1  /* based on LM data set*/
  

#define LM_ZEROS_PROB    21474830  /* appox  1% */
#define LM_MAKE_NEG  1073741824
#define LM_START_PHASE  0
#define LM_ZEROS_PHASE  1
#define LM_DATA_PHASE   2

/* Special Patterns Static values defined. */

#define  RANDOMIZER_ARRAY_SIZE    64
#define  DATA_MAGIC_PATTERN_SIZE  32
#define  DATA_ECC_PATTERN_SIZE    16
#define  DATA_SIOX_PATTERN_SIZE    4

/* Magic pattern Static values defined by Bruce B and Jeff H. */
static uint64_t DATA_Magic_Pattern[DATA_MAGIC_PATTERN_SIZE] = {0xffffffffffffffffULL,
                                                              0x0000000000000000ULL,
                                                              0xfeffa60925437111ULL,
                                                              0xdfe890efffff42cdULL,
                                                              0x0000010245067bdfULL,
                                                              0x102d567abc6e78d9ULL,
                                                              0xfdeb000000000000ULL,
                                                              0xaaaaaaaaaaaaaaaaULL,
                                                              0x5555555555555555ULL,
                                                              0x7fff000f00000000ULL,
                                                              0x5a5a5a5a5a5a5a5aULL,
                                                              0xc3c3c3c3c3c3c3c3ULL,
                                                              0x3c3c3c3c3c3c3c3cULL,
                                                              0xf9abcde4518f5dc3ULL,
                                                              0x1024358bd6578314ULL,
                                                              0x2bc78d5a63cbd789ULL,
                                                              0x3333333333333333ULL,
                                                              0x1234567890abcdefULL,
                                                              0x0000000000354261ULL,
                                                              0xbcddbcddbcddbcddULL,
                                                              0x0009f53461cad123ULL,
                                                              0xffdf9009cdcdcdcdULL,
                                                              0xfffba45263784888ULL,
                                                              0x0023764764bcdf31ULL,
                                                              0xdffdaccab4b4b4b4ULL,
                                                              0xeefdad1342674bdfULL,
                                                              0x0030000000000000ULL,
                                                              0x00000012333333cdULL,
                                                              0xffffffeeeee66666ULL,
                                                              0x0000022222fffbbbULL,
                                                              0x756453423109687aULL,
                                                              0x0f0f0f0f0f0f0f0fULL};

static uint64_t DATA_ECC_Pattern[DATA_ECC_PATTERN_SIZE] = {0x000000000000110FULL,
                                                         0x0000000000000000ULL,
                                                         0x0000000000001100ULL,
                                                         0x000000000000000FULL,
                                                         0x0000000000001307ULL, 
                                                         0x0000000000003108ULL, 
                                                         0x0000000000004016ULL, 
                                                         0x0000000000005119ULL, 
                                                         0x0000000000001011ULL, 
                                                         0x0000000000001012ULL, 
                                                         0x0000000000001014ULL, 
                                                         0x0000000000001018ULL, 
                                                         0x000000000200010EULL,
                                                         0x000000000200032EULL, 
                                                         0x000000000200036AULL, 
                                                         0x000000000200102CULL};

static uint64_t DATA_SIOX_Pattern[DATA_SIOX_PATTERN_SIZE] = {0x0000000011111111ULL,
                                                           0x0000000001234567ULL, 
                                                           0x0000000089abcdefULL, 
                                                           0x0000000000000000ULL};

static uint64_t Randomizer_Array[RANDOMIZER_ARRAY_SIZE];



/* *****
 * System_Topology definitons
 * 
 * */

int    Current_Virtual_MAP; /* -M   */
int    map_defined;         /* -M flag that the 1st MAP of pair was received. */
int    Finished_Memory;

#define ST_DEVICE_EMPTY          0  /* Null Device type */
#define ST_DEVICE_MAP            1  /* MAP Device type */
#define ST_DEVICE_CM             2  /* CM Device type */

#define DEVICE(s) s?"CM ":"MAP"  /* Have to subtract 1 as long as ST_DEVICE_CM = 2 */

#define ST_CM_ON_MAP             0x23  /* CM on a MAP H switch  */
#define ST_CM_ON_MAPH            0x23  /* CM on a MAP H switch  */
#define ST_CM_ON_MPCM            0x24  /* CM on a MPCM switch   */
#define ST_CM_ON_MAPJ            0x25  /* CM on a MAP J switch  */
#define ST_CM_ON_SPCM            0x26  /* CM on a SPCM switch   */

/* GPIOX attachments  */
#define NO_DAUGHTERBOARD   0x3f  /* 6 bits set */
#define SIOX_DAUGHTERBOARD 0x8  /* SIOX Single Port */



/* *******************************************
 * System Topology variables.
 * 
 */

int   Server_Processor;    /* Master = True */


int  No_GRM_Flag;       /*  System does not have GRM setup, ie a MAP I */

int              GRM_MAP_Default_Port; /* first MAP port found in GRM */

int Index2MAP[MAXMAPS]; /* mapping of port number to logical from maplib */
int MAP2index[MAXMAPS]; /* mapping of port number to logical from maplib */

/* *****
 * Misc global definitons
 * 
 * */

char    Title[NAME_SIZE];           /* used by Compare_production */
char    Address_Label[8];           /* used by Compare_production max 7 chars */
char    Compare_Label[NAME_SIZE];   /* used by Compare_production */
char    Compare_Label1[NAME_SIZE];  /* used by Compare_production */
int     Bank_Modulus;               /* used by Compare_production */
int     Bank_Mode;                  /* used by Compare_production how to determine bank in stride */
int     Read_Modulus;               /* used by Determine Bank mode#11,14,15
                                     * used by Determine Address */


/* *******************************************
 * Help data 
 * *******************************************/

#define TC_OPTIONS_SMALL_COMMON      "\
    [-d Display Data] [-t Trace Level]  \n\
    [-r Random Seed] [-R seed based on time ] \n\
    [-O Allocate Offset ] [-o Use non-aligned malloc.]  \n\
    [-I Inc of Start Value] [-v Start Value, hex only ]\n\
    [-X Invert Mask for slamming pattern, valid with -F50] \n\
    [-w Data Size in qwords] [-k Data Size in KQWords] [-m Data Size in MQWords] \n\
    [-F Pattern for input data] [-B Initial Pattern for actual results] \n\
    [-Y Invert data each iteration, no argument, valid with -i] \n\
    [-S iteration delay in seconds] [-W iteration delay in micro seconds] \n\
    [-f data pattern file path name] [-L Log file pathname] [-G Cartridge serial number]\n\
     \n"

/* *************** ************************************************** */
#define ENV_LABEL   "   Environment Definition: \n "
#define ENV_DESC   " \n" 
#define ENV_OPTIONS "\
\n \033[1m Environment Variable Options \033[0m\
\n   VARIABLE Name      ||    Values    ||   Meaning \
\n  -- dv only \
\n  PACKET_OVERRIDE          0-16         Value to use as the alignment. \
\n  DV_OUTPUT_MODE          ORIGINAL      Set to ORIGINAL to get Original compare Display \
\n  DV_DISPLAY_ERROR_MODE   COLORFUL      Set to COLORFUL to get red hi-lites on errors. \
\n                          ORIGINAL      Set to ORIGINAL to use normal text on errors. \
\n  DV_USE_DISPLAY_SECTIONS ORIGINAL      Original non-sectioned AR and ER display, unlike XOR.\
\n  DV_COMPARE_CONTEXT_WINDOW 1-100       Context window for display option 4 (-d4). \
\n  -- maplib \
\n  DV_EMUMODE                 1          Use emulation mode, normal is to use HW. \
\n  NOWAITONMAP               0/1         Exit if MAP/SNAP is busy, do not wait.\
\n  -- \
\n  DVLOGFILE                             Path and filename for DV results log file.\
\n  MAPLOGFILE                            Path and filename for trace log file.\
\n  MAPTRACE                 0 - 5        Trace level 0 none; 5 very verbose; 4 normal\
\n  STOPONERROR               0/1         Do not continue processing on error.\
\n  SRC_Memory_Allocate                   If using the procedure SRC_Memory_Allocate, use one of the following: \
\n                 malloc                 default processor local memory \
\n                 Cache_Aligned_Allocate Aligned processor local memory \
\n  -- \
\n   \
 \n"
#define ENV_NOTES   " Not set = 0; Set = 1\n"



#define TC_OPTIONS_COMMON "\
 -w   Size of data in qwords. \
 \n  -k   Size of data in kiloqwords. \
 \n  -m   Size of data in megaqwords. \
 \n  -t   Print tracing data (0-4) where 0 is no output, 4 is verbose.(default: 0) \
 \n  -d   Display specific data based on the number: (default: 0)\
 \n       0 display only a success/fail and error count.  \
 \n       1 display the results, even if correct.  \
 \n       2 display only the Incorrect data to a Max. \
 \n       3 display all of the Incorrect data. \
 \n       4 display the values around the Incorrect data.  \
 \n         Set env DV_COMPARE_CONTEXT_WINDOW to size of window. default 5 \
 \n  -T   Verbose mode. Default is one line per test case. \
 \n  -O   Qword Offset from aligned address. Used to test local processor memory not \
 \n       being aligned to 16 qword cache line. (default 0 cache aligned)\
 \n  -o   Allocate local using malloc, instead of aligned allocate. No Argument. \
 \n  -W   Wait time between iterations. In Microseconds                 \
 \n  -S   Wait time between iterations. In Seconds \
 \n  -i   iterations by test case, number of executions of the comlist. \
 \n  -v   Inputted value is the starting value. Hex Only (0 default). \
 \n       If argument = PID then the pid of the process will be inserted in the\
 \n       upper two bytes, use any option. To get a non-zero starting value\
 \n       and the PID, use -v twice, the first time input the start value, the \
 \n       second time request the PID. e.g. -vafafaf -vPID \
 \n  -I   Inc inputted value (-v) by input. (1 default).\
 \n  -X   Slamming Mask, XOR with data value to invert data bit by bit.  \
 \n       The mask bit set to 1 inverts the corresponding bit in data, 0 does nothing.\
 \n       e.g. a mask of 0000ff would invert only the lower byte and leave the other \
 \n       bits alone. Works with -F50 and -F92, Hex Only (all f's default). \
 \n  -B|F B = Initial Pattern of Local Memory Implemented for some test cases. \
 \n       F = Data Pattern of input data. \
 \n         0  Zero (B default) \
 \n         1  Sequential numbers, starting at 1 increment by 1. \
 \n         2  Random Reseed. Same random numbers every iteration. \
 \n         3  Random (F default). \
 \n         4  Sequential pattern (uses -v and -I). \
 \n         5  Alternate Invert starting with Start_Value (uses -v default=0).  \
 \n             Also known as Slammin Pattern.  \
 \n         More patterns and file reading are available, use option -F without an\
 \n          arg to get an list.  \
 \n  -Y   Inverts the data between iterations, the first iteration uses -F option.\
 \n       Argument not required.  Requires -i to be valid.  \
 \n       Supersedes any other load data options, after first iteration.\
 \n  -R   Use a new random number seed every execution. Based on current time. \
 \n       Argument not required.  Only valid with random based patterns. \
 \n  -r   Random Number seed. ( default seed is 999999, unless -R is set.)\
 \n       Only valid with random based patterns. \
 \n  -D   Dump input data to file, -D1 creates a binary file (.bin extension).  \
 \n       -D0 creates an ASCII file (.dat extension) in hex, 1 qword per line is default.\
 \n  -f   Data Pattern file pathname.  \
 \n  -L   Log file pathname.  \
 \n  -G   Cartridge serial number.  \
 \n  -M   Cartridge slot.  \
 \n  -g   Diagnostic testing option. Use -g0 for more details.\
  \n \n "


#define TC_NOTES_COMMON   "\
\n - Unless noted otherwise the numeric inputted values are default decimal or \
\n    are hexadecimal if prefixed with 0x. \
\n - The trace; display; patterns (B,F); Test case;  are decimal only. \
\n \n" 


#define TC_ANALYSIS_BITERROR "\n ----------------------------------------------------\
  \n Bit Map for decoding single bit errors: \
  \n   XOR pattern: 0  0  0  0  0  0  0  0  0  0  0  0  0  0  0  0 \
  \n  bit position: 60 56 52 48 44 40 36 32 28 24 20 16 12 8  4  0 \
  \n  \
  \n  For the following nybble offsets add to the bit position from the above chart:\
  \n   1  +0\
  \n   2  +1\
  \n   4  +2\
  \n   8  +3\
  \n  \
  \n  e.g. an XOR of 0000000000000400 indicates a bit error in bit 10.\
  \n       8 + 2 = 10 (bit position of the XOR nybble + nybble offset) \
   \n  \
  \n \n"

/*-------------------------------------------------------------------------------*/

#define TC_OPTIONS_ERROR "\n\033[1m Generate Data Error option (-g):\033[0m \n\
      0     Display generate error options. This display. \n\
      1     Zero actual result. \n\
      2     Set Bit error. Use 21 to get mask into the hi order word.\n\
      3     Clear Bit error.  Use 31 to get mask into the hi order word.\n\
      4     Very low Probability to Set Bit error. \n\
      5-9   Not used. \n\
      10    A percentage of qwords randomized.  \n\
      11    every qword randomized. \n\
      12    A percentage of error bit set/cleared. \n\
       \n\
        Will use the Random seed as the error bit mask, the seed is masked with the \n\
      generated pattern.  The Random seed would still be used to seed the random \n\
      number generator.  The -r and -R options would have to appear on the command\n\
      line before the -g option if they are to be used. Any -r or -R after the -g \n\
      would only seed the random numbers. \n\
       \n\
       Generate System Error option: \n\
      Use negative parameters (e.g. -g-104) to generate |parameter| system error.\n\
      Examples: Parameter      System Error \n\
                    -104        ERROR_Abort  \n\
                    -103        ERROR_Comlist  \n\
                    -103 to -114       MAP FAILURE \n\
\n"


/*-------------------------------------------------------------------------------*/
/* |----------------------------------------------------------------------------| new */


#define CMN_DATA_PATTERNS   "\n -------------------------------------------------------------- \
 \n   \033[1m Data Pattern Descriptions.\033[0m  \
 \n         0   Zero, all bits clear. (B default) \
 \n         1   Sequential numbers, starting at 1 increment by 1. \
 \n         11  All F, all bits set. \
 \n         12  All A, even bits set. \
 \n         13  F00FF00FF00FF00F. \
 \n         14  0FF00FF00FF00FF0. \
 \n         15  All fives, odd bits set. \
 \n         16  Sequential pattern increment every byte. Start: 0101010101010101\
 \n         161 Invert Sequential pattern increment every byte. \
 \n         2   Random Reseed. Same random numbers every iteration. \
 \n         3   Random (F default). \
 \n         31  Random in Upper Part of qword Count Other Half. \
 \n         32  Random in Lower Part of qword Count Other Half.\
 \n         4   Sequential pattern (uses -v and -I). \
 \n         41  Upper Static Sequence and Lower Sequence. \
 \n             Lower word modulus of stride (-I) -v option ignored. \
 \n             Upper changes value based on stride.\
 \n         42  Upper Static Random and Lower Sequence. \
 \n             Lower word modulus of stride (-I) -v option ignored. \
 \n             Upper changes value based on stride.\
 \n         5   Alternate Invert starting with Start_Value (uses -v default=0).\
 \n              Also known as Slammin Pattern.  \
 \n         50  Invert starting with Start_Value, based on mask in option -X.\
 \n         501 Full Invert starting at even bits.  \
 \n         502 Full Invert starting at odd bits.  \
 \n         51  Invert Lower word starting with Start_Value.  \
 \n         52  Invert Upper word starting with Start_Value.  \
 \n         53  Invert Middle word starting with Start_Value.  \
 \n         54  Invert odd bits starting with Start_Value.  \
 \n         55  Invert even bits starting with Start_Value.  \
 \n         57  The imaginary numbers random pattern, w/slam inter-mixed.  \
 \n             Also known as Defender 64bit Pattern.  \
 \n         58  The 32bit negative integer random pattern. 32 bit FFT data pattern.\
 \n             Also known as Defender 32bit Pattern.  \
 \n         59  The 32bit negative integer random pattern, zeros inserted.   \
 \n         8   Alt Invert 2 qwords starting with Start_Value (uses -v default=0).  \
 \n         82  Alt Invert 2 qwords starting with Start_Value based on mask in option -X.\
 \n         84  Alt Invert 4 qwords starting with Start_Value based on mask in option -X.\
 \n         88  Alt Invert 8 qwords starting with Start_Value based on mask in option -X.\
  \n"

#define CMN_DATA_SPECIAL  "\
         70  Special pattern.  The first data element is the starting value (-v). \
  \n            The 2nd data element is the starting value inverted.  \
  \n            Then every 4th data element in the packet is inverted again. \
  \n            Every packet would look the same. \
  \n            The pattern of data words: 0FFF000FFF000FFF0FFF000FFF000FFF \
  \n         71  Special ECC pattern. Not randomly distributed. \
  \n         72  Special Magic pattern. \
  \n         73  Special Magic pattern, intermixed with all 1's and all 0's. \
  \n         74  Special SIOX Pattern, used on Defender. Not randomly distributed.  \
  \n         75  Walking ones Pattern. Starts at 0 repeats pattern after bit 63. \
  \n         751 Invert Walking ones Pattern. Starts at F's repeats pattern after bit 63.\
  \n \
 \n          With option -v if argument = PID then the pid of the process will be inserted\
 \n             in the upper two bytes, use any option. To get a non-zero starting value\
 \n             and the PID, use -v twice, the first time input the start value, the \
 \n             second time request the PID. e.g. -vafafaf -vPID \
   \n"
/*-|-------------------------------------------------------------------------------------|-*/

#define CMN_READ_FILE   "\
 \n   \033[1m Read File for data pattern. \033[0m \
 \n     The following are test data that are  read from a file and fills the data pattern\
 \n     to the requested size (options: -w -k -m).  If file is smaller than the data size\
 \n     requested, the data is reused to fill in the rest of the data pattern; unless the\
 \n     -v option is used,  then that value is used for the extra data patterns.  Set env\
 \n     DV_DATA_PATTERN_FILE  to file name.  File has 1 data  value per line.  The normal\
 \n     option reads every data element, some files have a size as  the first element use\
 \n     the 1 suffix options for these files(i.e. -F931). The i formated files will treat\
 \n     data that start with: 0x as hex; 0 as oct and anything else decimal . This format\
 \n     does  not read the  63rd bit  except as the sign  bit,  i.e.  it does not  handle\
 \n     unsigned 64 bit, use -1 for 0xffffffffffffffff. All of the data is read as 64bit.\
 \n     \
 \n     The -f option sets the file name and over ride the DV_DATA_PATTERN_FILE variable.\
 \n     \
 \n     The  -D0 option will  create a file  that is  readable by the  -F93 data  pattern\
 \n     option.  The -D1 option (binary) will create a file that is readable by the  -F96\
 \n     data pattern option.\
 \n   \
 \n   valid Options: \
 \n      93  Read Hex data from file. \
 \n      94  Read decimal data from file. \
 \n      95  Read i formated data from file. \
 \n      931 Read Hex data from file.  1st element ignored.\
 \n      941 Read decimal data from file.  1st element ignored.\
 \n      951 Read i formated data from file. 1st element ignored.\
 \n      96  Read file as binary data. Reads and stores all data, including formatting.\
 \n           \
  \n   Sample file that can be read with the -F93 option:\
  \n     0xFFFFFFFFFFFFFFFF \
  \n     0x0000000000000001 \
  \n     0x000000000000000e \
  \n     0x000000000000000f\
  \n "

#define CMN_DATA_NOTES  "\
  \n   \033[1m NOTES:  \033[0m \
  \n     By setting the -F,  -v and -I  options a specific  pattern can be loaded. The -v\
  \n     option is a starting pattern  which is read as hex from the command line. The -I\
  \n     option is the decimal number to increment -v by, default is 1. Use the following\
  \n     options for each data pattern:\
  \n      - sequence of numbers starting at 0  use: -F4 \
  \n      - sequence of numbers starting at 1  use: -F1 \
  \n      - Same pattern use: -F4 -v<pattern> -I0 \
  \n      - Custom Slam pattern use: -F50 -X<mask> -v<start pattern> \
  \n      - Starting bit walk pattern use: -F4  -v0x1  -I0 \
  \n        Second bit walk pattern use:   -F4  -v0x2  -I0 \
  \n        Continue to change -v for each bit location. \
 \n "


/*-------------------------------------------------------------------------------*/

#define CMN_ADDRESS_CHART "\
  \n   \033[1m Common Memory Address and Sizes Table.\033[0m  \
  \n   \
  \n   Corresponding addresses for different memory locations. GRM reserves 1 Mega-byte \
  \n   of memory at the lowest memory address. The Mega-Qword column can be used as\
  \n   data sizes for the -m command line option. \
  \n   \
  \n   Giga-  Mega- Mega-         Qwords \
  \n   Bytes  Byte  Qword     decimal       hex \
  \n             8      1        131,072     20000 \
  \n    0.25   256     32     33,554,432   2000000 \
  \n    0.5    512     64     67,108,864   4000000 \
  \n     1    1024    128    134,217,728   8000000 \
  \n     2    2048    256    268,435,456  10000000 \
  \n     3    3072    384    402,653,184  18000000\
  \n     4    4096    512    536,870,912  20000000 \
  \n     5    5120    640    671,088,640  28000000\
  \n     6    6144    768    805,306,368  30000000\
  \n     7    7168    896    939,524,096  38000000\
  \n     8    8192   1024  1,073,741,824  40000000\
  \n     9    9216   1152  1,207,959,552  48000000\
  \n    10   10240   1280  1,342,177,280  50000000 \
  \n    11   11264   1408  1,476,395,008  58000000 \
  \n    12   12288   1536  1,610,612,736  60000000 \
  \n    13   13312   1664  1,744,830,464  68000000 \
  \n    14   14336   1792  1,879,048,192  70000000 \
  \n    15   15360   1920  2,013,265,920  78000000 \
  \n    16   16384   2048  2,147,483,648  80000000 \
  \n    32   32768   4096  4,294,967,296 100000000 \
\n \n"

/*-------------------------------------------------------------------------------*/

#define FT_PRODUCTION_NOTES "\
  \n \033[1m Saturn Functional Test Case Descriptions.\033[0m  \
  \n  Last update  10/15/2013 \
  \n  Notes:  An - means it does not use, an X it does use. \
  \n          The test plan column is the Functional Test from the Saturn Test Plan.   \
  \n          \
 \n"

#define FT_PRODUCTION_HEADER "\
  \n  Test Case                             Test  -------- Uses -------- User\
  \n                                        Plan   BRAM    OBM     VLM   Chip  \
  \n                                                                                  \
 \n"

#define FT_PRODUCTION_FT "\
 ft01 Atom Boot Test                      1     -        -      -      -\
  \n ft02 Microprocessor Memory Test          2     -        -      -      -\
  \n ft03 SNAP Interface Test                 3     -        -      -      -\
  \n ft04 SNAP Configuration Test             4     -        -      -      X\
  \n ft05 Config MAP & Streaming DMA w/o OBM  5     -        -      -      X\
  \n ft06 Ethernet Access from User Logic     6     -        -      -      X\
  \n ft07 OBM Access from User Logic          7     X        X      -      X \
  \n ft08 VLM Access from User Logic          8     X               X      X \
  \n ft09 Full Function                       9              X      X      X\
  \n ft10 OBM Stuck Address bit               7     X        X      -      X\
  \n ft11 VLM Stuck Address bit               8     X        -      X      X\
  \n ft12 VLM Data Line                       8     X        -      X      X\
  \n          \
\n \n"


/*-------------------------------------------------------------------------------*/

#define CM_STATUS_SIZE      16        /* defined in map.h currently 16 */

#define SNAP_STATUS_SIZE      9        /*  currently 9 words displayed */

uint64_t     SNAP_Status[SNAP_STATUS_SIZE];

/* *******************************************
 * Global options 
 * *******************************************/

/* Items to read in.*/
int      Display;                 /* -d */
int      Use_Display;             /* -d */
int      Random_Seed;             /* -r */
int      Random_Seed_Flag;        /* -R  use time as seed. */
int      Iterations;              /* -i */
uint64_t Stride;                  /* -I */
uint64_t Starting_Value;          /* -v */
int      Use_Starting_Value;      /* -v */

int64_t  Size_Of_Data;           /* in words  -w,k,m Max side */
int64_t  Size_Of_Data_Used;     /* in words  actual amount of data moved or requested? */
int      Trace;                   /* -t */

int      Specific_Size_Flag;      /* -wkm. Use Size of data for DMA buffer */
int      Delay;                   /* -W -S Wait between loops in micro seconds */
int      OBM_Delay;               /* -S Wait for completion in micro seconds */
int      SMB_Delay;               /* -S Wait for completion in micro seconds */

int      Data_Load_Flag;          /* -F Data_Load_Flag type */
int      Default_Pattern;         /* -F Used with Data_Load_Flag type  Boolean*/
int      Background_Pattern_Flag; /* -B Starting Pattern the actual results */
int      Default_Background;      /* -B Used with Background_Pattern_Flag type  Boolean*/
uint64_t Slamming_Mask;           /* -X Slamming pattern mask, 1 invert bit */
int      Data_Invert_Flag;        /* -Y Invert data for each iteration flag  Boolean*/

int      Allocate_Offset;         /* -O Number of qwords to offset address from Cache aligned */
int      Cache_Size;              /* Size of cache lines */
uint64_t Packet_Size;           /* Size of Packets and the enforced alignment */

uid_t    pid;                     /* Pid of the process  */
int      Use_PID;                 /* -v use the Pid of the process to label data, 
                                   forces a F4 */
/* Determine the Memory type Local vs Common */
char   SRC_Memory[NAME_SIZE];
int    Common_Memory;   /* Common Memory Flag.  Boolean */
int    User_Chip;       /* User_Chip the binary was compiled for */


/* File Data pattern option  File  variables */
char   Data_Pattern_Filename_Option[NAME_SIZE];
int    Data_Pattern_Filename_Flag;
char   Data_Pattern_Filename[NAME_SIZE];
FILE   *Data_File;         /* Pointer to file */

char   Host_Name[NAME_SIZE];
char   Test_Name[NAME_SIZE];

/* Probes  variables */

/* time functions */
struct timeval TP;
struct tm      TM;
char Time_Str[NAME_SIZE];
char Date_Str[NAME_SIZE];
char Echo_Str[NAME_SIZE];
size_t Time_Size;
time_t Current_Time;

int long Start_Time;            /* seconds since epoch */
int long Finish_Time;           /* seconds since epoch */
int long Start_Time_usec;       /* microseconds since start of current second */
int long Finish_Time_usec;      /* microseconds since start of current second  */
int long long Total_Time_usec;  /* microseconds execution of program */

int long TC_Start_Time;            /* seconds since epoch */
int long TC_Finish_Time;           /* seconds since epoch */
int long TC_Start_Time_usec;       /* microseconds since start of current second */
int long TC_Finish_Time_usec;      /* microseconds since start of current second  */


/* OBM stride  */
int Banks[OBM_STRIDE_BANKS];


/* *******************************************
 * Generate errors type
 * *******************************************/
int Use_Generate_Error;         /* -G  boolean*/
int Generate_Error_Flag;        /* -G */
int Generate_Error_Code;        /* -G */
uint64_t Generate_Error_Mask;     /*  */
int Probability_Of_Error;       /* used as Mod to random number (0-65536)  */


/* *********************************************************************************
 * Procedure  Initialize_Parameters
 * ****************************************************************************** */
void Initialize_Parameters_Common ();


/* ****************************************************************
 *   Compare_Production
 *   Newer Compare results procedure, used for the production tests.
 *
 *
 *   Parameter      Definition
 *   Bank_Modulus   for striding tests the number of banks, used to
 *                  determine the actual address from the index
 *   Mode           Index into Determine_Bank, returns label of the
 *                  qword for the correct bank.
 *   Address_Label  Label the index as Address or only an index.
 *   Label          Bank or test case specific label, usually the bank
 *   
 * Determine_Bank
 *   Modes   meaning
 *    0      do not change label
 *    1      in group ABCD
 *    2      in group EFGH
 *    3      in group A1B1C1D1
 *    4      in group E1F1G1H1
 *    5      in Bridge Port A1,A2,B1,B2  ( bridge port)
 *    6      mixed 8 banks ABCDABCD
 *    7      mixed 8 banks EFGHEFGH
 *    8      lower 8 banks ABCDEFGH
 *    9      Upper 8 banks ABCDEFGH
 *    10     in Bridge Port A1/B1,A2/B2  (streaming bridge port)
 *    11     in Read count
 *    
 * ****************************************************************/
int Compare_Production ( uint64_t *AR,  /* CM to */ 
                         uint64_t *ER,
                         int64_t   Result_Size,     /* Size in words of the Results */
                         int       Display,         /* Display option */
                         uint64_t  OBM_Offset,      /* Print OBM index with offset */
                         int       ER_Offset,       /* starting index into ER & AR */
                         int       Bank_Modulus,    /* the number of banks strided in data */
                         int       Mode,            /* Mode to use in displaying errors */
                         char     *Address_Label,   /* address/index label   */
                         char     *Label,           /* Bank or test case specific label  */
                         int       Trace,           /* Trace level */
                         FILE     *logfile,         /* Pointer to file */
                         int       Log_File_Defined);/* boolean true if a log file exists */

int  Print_Production_Header ( char   *Address_Label,   /* address/index label  */
                               char   *Label );  /* Bank or test case specific label  */

int  fprint_Production_Header ( char   *Address_Label,   /*  address/index label  */
                                char   *Label,           /* Bank or test case specific label  */
                                FILE   *logfile );       /* file to write to */

void  XOR_Convert ( uint64_t   XOR_Numeric,
                    char*      XOR_ptr  );  /* */

void  XOR_Bits ( uint64_t AR,
                 uint64_t ER,
                 int index,
                 char* Bit_ptr );  /* */


void  Determine_Bank ( int   Mode,   /* actual bank label   */
                       int   Index,   /* Index to use to find bank. */
                       char   *Label ); /* actual bank label   */

 /* Select between the sectored display */
void  Results_Convert ( int      Sectored,       /* boolean to select sector */
                        uint64_t Numeric,        /* input 64 bit number */
                        char*    Display_ptr );  /* output Sectored string */

uint64_t  Determine_Address ( int      Mode,   /* actual bank method . */
                              uint64_t OBM_Offset,  /*  */
                              int      Index,   /* Count in Actual results */
                              int      Bank_Modulus, /* Modulus for stride  */
                              int      Read_Modulus); /* Modulus for multiple reads   */

/* **********************************************************************************
 * Generate_Error
 * 
 *  Error generator for testing the diagnostic function.  Normally a 1/6 Probability of error
 *   flag - Type error
 *    1     Zero actual result
 *    2     Set Bit error    the XOR will vary because of the different AR.
 *    3     Clear Bit error  the XOR will vary because of the different AR.
 *    4     Set Bit error, but real low Probability.
 *    5-9   Not used
 *    10    A % of  words randomized does not use mask.
 *    11    every word randomized does not use mask.
 *    12    A % of error bit set/cleared, XOR'ed so the XOR would be the same for all. 
 *    all others no errors
 *    
 *  Other notes: 
 *    The rand returns a signed int between 0 and 2**31, basicly
 *    an 31 bit word. This leaves one bit that cannot be toggled. 
 *    BUT when it is assigned to a 16 bit variable it will assign
 *    all 16 bits and it does not overflow the memory allocation 
 *    of the variable.
 *    
 *    Will use the Random seed as an error bit mask. So using -R to set the 
 *    seed would sorta be random bit errors (based on current time), and -r12  
 *    would be bits 2 and 3 in error. The Random seed would still be used to seed 
 *    the random number generator. The -r and -R options would have to appear
 *    on the command line before the -g option if they are to be used. Also note 
 *    that the seed is only 32 bit, so only the lower word would be affected by
 *    the mask. 
 *
 *    
 *  Ordering notes:
 *      |63                                                 0| 64 bits
 *      |      3    |      2     |      1      |     0       | 4 x 16bits
 *    
 *   
 * ************************************************** */
int Generate_Error ( uint64_t  *AR,
                     int     Generate_Error_Flag,/* type of error to generate */
                     uint64_t  Generate_Error_Mask,/* bit mask for error  generate */
                     int     Trace,  
                     FILE   *logfile,         /* Pointer to file */
                     int     Log_File_Defined);  /* boolean true if a log file exists */

   
/* **********************************************************************************
 *  Read_Command_Line
 * 
 *  Procedure to read the command line and process the options that
 * Apply to the common utilties, mostly related to data generation and
 * compare of results.
 * 
 * ************************************************** */
/* Read_Command_Line definitions
 *  The opts has the characters that it will scan for, if followed by a 
 *  colon expects a parameter. If expects a parameter will error and return 
 *  a -1 if there is not one. Double colon will accept an option w/o a parameter
 *  and not return an error.
 *
 * */

#define COMMON_VALID_OPTS  "O:t:d:I:i:S:W:v:w:k:m:r:f:F::B:X:g:L:YRo"

int Read_Command_Line_Common (int Read_Option);

/* ****************************************************************
 *  Load_Data  64  bit 
 *  Procedure to load the CM From data that is DMA'd to OBM and is used 
 *  for the expected results.
 *  
 *  Used to build the Stimulus data and the Expected 
 *  
 * ****************************************************************/
int Load_Data ( uint64_t *Data,             /* Data structure to load */ 
                int64_t  Data_Size,        /* Size of the Data structure to load. */
                int      Data_Load_Flag,   /* Data_Load_Flag type */
                int      Trace,            /* Trace level */
                uint64_t Start_Pattern,    /* Starting Pattern */
                uint64_t Stride,           /* Value to Inc the Starting value. */
                int      Seed );           /* Seed to use for reseeding if needed. */


/* **********************************************************************************
 *  CM_Packet_Aligned qwords
 * 
 *  Procedure to return a size/ address that is aligned on the current packet
 * alignment. 
 * The input number is assumed to be in qwords
 * 
 * ************************************************** */
/* Round down */
int  CM_Packet_Aligned  ( int Number,
                          int Trace );           /* Trace level */
			 
int  CM_Packet_Aligned_Down  ( int Number,
                               int Trace );           /* Trace level */
			 
uint64_t  CM_Packet_Aligned_Address  (  uint64_t Number,
                                        int Trace );           /* Trace level */

uint64_t  CM_Packet_Aligned_64_Down  (  uint64_t Number,
                                        int Trace );           /* Trace level */

/* Round up */
uint64_t  CM_Packet_Aligned_Length  (  uint64_t Number,
                                       int Trace );           /* Trace level */

uint64_t  CM_Packet_Aligned_64_Up  (  uint64_t Number,
                                      int Trace );           /* Trace level */

/* **********************************************************************************
 *  CM_Packet_Aligned bytes
 * 
 *  Procedure to return a size/ address that is aligned on the current packet
 * alignment. Always round UP to next aligned number.
 * The input number is assumed to be in bytes.
 * 
 * ************************************************** */
 uint64_t  CM_Packet_Aligned_Bytes  ( uint64_t Number,
                                      int      Trace );         /* Trace level */
				     

/* **********************************************************************************
 * Time Delta functions
 * 
 * TP_Delta 
 *   Determine delta in microseconds using the sec and usec fields from the TP struct.
 * sec is the number of seconds since epoch and usec is the number of microseconds 
 * since start of latest sec.
 *   
 * 
 * ************************************************** */
int long long  TP_Delta  ( int long l_sec, 
                           int long l_usec,
                           int long r_sec, 
                           int long r_usec );

/* **********************************************************************************
 * functions Display_MAP_Type 
 *           Display_MAP_Type_Series
 *           Convert_MAP_Type 
 *   
 *       
 * Display_MAP_Type 
 *   Take as input the integer MAP board type from the status, etc, and convert to a 
 *   displayable string.
 *     MAP type    String    
 *      0          Dual
 *      1          Single
 *      2          Compact
 *      3          Single Pro
 *      4          6U
 *      5          Portable
 *      6          SRC-7 series H
 *      7          SRC-7 series I
 *      8          CC
 *    
 * Convert_MAP_Type 
 *    Take as input the string value and convert to a numeric MAP board type. The 
 *    string value can be the full name or the series letter. The string is converted
 *    to uppercase so it is not case sensitive.
 *      String          MAP type     define    header package
 *      dual              0          MAP_B_BD  dv_common_util.h
 *       b                0          MAP_B_BD  dv_common_util.h
 *      single            1          MAP_C_BD  dv_common_util.h
 *       c                1          MAP_C_BD  dv_common_util.h
 *      compact           2          MAP_D_BD  dv_common_util.h
 *       d                2          MAP_D_BD  dv_common_util.h
 *      single_pro        3          MAP_E_BD  dv_common_util.h
 *      singlepro         3          MAP_E_BD  dv_common_util.h
 *      pro               3          MAP_E_BD  dv_common_util.h
 *       e                3          MAP_E_BD  dv_common_util.h
 *      portable          5          
 *       f                5          
 *      6U                4          
 *       g                4          
 *       h                6          MAP_H_BD  src7/include/map.h
 *       i                7          MAP_I_BD  src7/include/map.h    
 *       cc               8          MAP_CC_BD src7/include/map.h    
 *       j                9          MAP_J_BD  src7/include/map.h    
 *       l                11         MAP_L_BD  src7/include/map.h    
 *       
 *      
 * ************************************************** */
char*   Display_MAP_Type ( int MAP_Type );

char*   Display_MAP_Type_Series ( int MAP_Type );

int     Convert_MAP_Type ( char* MAP_String );

map_types   Convert_MAP_enum (  int MAP_Type );

/* **********************************************************************************
 * Display_SNAP_Processor_Type
 * 
 *   Take as input the integer SNAP board connection type from the status, etc, and convert to a 
 *   displayable string.
 *     SNAP type    String    
 *      9          P4 snapd
 *     17          Atom snapg rev b
 *     19          Atom snapg rev c
 *   other         UNKNOWN 
 *      
 * ************************************************** */

char*    Display_SNAP_Processor_Type ( int SNAP_Soft_Rev );

/* **********************************************************************************
 * Display_Node_Detect_Id
 *
 * converts the  node_detect_id enum type to text that can be displayed.  The node
 * detect is the possible values returned from the snap_node_detect_id() procedure.
 * which detects what a snap is attached to.
 * 
 *      typedef enum node_detect_id 
 *      
 * ************************************************** */

char*    Display_Node_Detect_Id (node_detect_id_t SNAP_Connected_To   );



/* **********************************************************************************
 * Use Memory Resources
 * 
 *   Will use a certain amount of local memory then exit after a certain amount of time.
 *   
 *   
 * ************************************************** */

int   Use_Resources ( int64_t   Chunk_Size,         /* Size of chunks of allocated memory */
                      uint64_t  Number_Of_Chunks,   /* number of memory chunks allotted */
                      int       Total_uSeconds,     /* How long to hold memory in microSeconds*/
                      uint64_t  Common_Memory_Flag, /* boolean use common memory */
                      int       Trace,
                      int       Countdown_Flag  );  /* Boolean turn on countdown */


/* ****************************************************************
 *  DV_Aligned_Allocate 
 *   
 * ****************************************************************/
void *DV_Aligned_Allocate (int Length_In_Bytes,
                           int Offset );


/* ****************************************************************
 *   Display VLM Port type
 *   
 * ****************************************************************/
char*    Display_VLM ( int VLM_Port );

/* ****************************************************************
 *   Display_Bank
 *   
 * ****************************************************************/

char*    Display_Bank ( int Bank_Number );

/* ****************************************************************
 *   Display Switch Board type
 *   
 * ****************************************************************/
char*    Display_Switch_Board_Type ( int Number );

  
/* ****************************************************************
 *    Print Version Header
 *   
 * ****************************************************************/

void  Version_Header  ( char   *Host,   /* host name   */
                        char   *Label,   /*  label   */
                        FILE   *Vfile );        /* file to write to  */


/* **********************************************************************************
 *  Current_System_Memory
 *   returns the number of bytes of memory currently available 
 * 
 * ************************************************** */

int long long  Current_System_Memory  ( );

 
/* ***********************************************************************************
 * Display of nominal test case lines
 *  Print_Test_Case_Header - start line w/ inputted options (defined common) 
 *  - allocate line w/ detailed options (defined suite)
 *  Display_Error_Summary - succeed/fail line  (defined common)
 *  
 * */
/* ***********************************************************************************
 * Common main procedures
 * 
 *   Display_Error_Summary at the end of the main.
 *   Print_Iteration_Results at end of interation loop
 *  
 * */

void Print_Iteration_Results ( int      Verbose_Flag, /* Verbose Flag  */
                               int64_t  Result_Size,   /*  */
                               int      err,   /*  */
                               int      Iteration_Count,   /* current  Iteration */
                               int      Number_Of_Iterations,   /*    */
                               int      *err_iterations, /*number Iteration with errors*/
                               int      Trace,   /* trace level */
                               FILE     *logfile,         /* Pointer to file */
                               int      Log_File_Defined
                               );  /*  */


int Display_Error_Summary ( char      *FT_Case,   /*  Test_Case  */
                            int      Verbose_Flag,   /*    */
                            int      Return_Code,   /*  Non Data error check  */
                            int      Iteration_Count,   /* current  Iteration */
                            int      Number_Of_Iterations,   /*    */
                            int      err_iterations,  /* number Iteration with errors*/
                            int      err_total,   /*  number of errors */
                            char    *Test_Name, /*   */
                            FILE    *logfile,         /* Pointer to file */
                            int      Log_File_Defined
                            );  /*   */

void Add_User_Chip ( int     Trace,           /* Trace level */
                     char   *Options);        /* display Options string */

void Add_Random_Seed ( int     Trace,           /* Trace level */
                       int     Random_Seed,     /* Random Seed used */
                       int     Data_Load_Flag,  /* Data_Pattern */
                       char    *Options);       /* display Options string */


void Print_Test_Case_Command_Line (char    *FT_Case,    /* Test Case string */
                                   int     Verbose_Flag,    /* Verbose flag */
                                   int     Trace,           /* Trace level */
                                   char   *Options,         /* String of options */
                                   FILE   *logfile,         /* Pointer to file */
                                   int     Log_File_Defined); /* boolean true if a log file exists */


void Print_Test_Case (char    *FT_Case,    /* Test Case string */
                      int     Verbose_Flag,    /* Verbose flag */
                      int     Trace,           /* Trace level */
                      int     MAP_Port,        /* Physical MAP Port */
                      FILE   *logfile,         /* Pointer to file */
                      int     Log_File_Defined, /* boolean true if a log file exists */
                      char    *Title_Options  );
/* display errors to log on non-data errors*/
void Print_Test_Error ( char    *Title_Options,
                        int     Error_Code,    /* Verbose flag */
                        int     Verbose_Flag,    /* Verbose flag */
                        int     Trace,           /* Trace level */
                        FILE   *logfile,         /* Pointer to file */
                        int     Log_File_Defined /* boolean true if a log file exists */
                        );



/* **************************************************************
 * Enumeration display procedures.
 * */


char*    Display_Multi_Bank ( uint64_t Banks );  /* 8.6 banks */
char*    Display_Data_Pattern ( int  Data_Load_Flag );  /* Data_Pattern name*/
                        

/* ***********************************************************************************
 *  Display Stuck bit table
 * 
 *   Address_Table sized for 32GB and populated with byte address
 * */

/* Sizes of array */
#define MAX_STUCK_BIT_ADDRESS  32  /* Number of addresses 32 divide evenly */

/* Addresses for 16 qword packets GCM and VLM are byte */
static int64_t Address_Table[MAX_STUCK_BIT_ADDRESS] = {0x0ULL,
                                                       0x80ULL,
                                                       0x100ULL,
                                                       0x200ULL,
                                                       0x400ULL,
                                                       0x800ULL,
                                                       0x1000ULL,
                                                       0x2000ULL,
                                                       0x4000ULL,
                                                       0x8000ULL,
                                                       0x10000ULL,
                                                       0x20000ULL,
                                                       0x40000ULL,
                                                       0x80000ULL,
                                                       0x100000ULL,
                                                       0x200000ULL,
                                                       0x400000ULL,
                                                       0x800000ULL,
                                                       0x1000000ULL,
                                                       0x2000000ULL,
                                                       0x4000000ULL, /* 0.5 GB */
                                                       0x8000000ULL,
                                                       0x10000000ULL,
                                                       0x20000000ULL,
                                                       0x40000000ULL,
                                                       0x80000000ULL,
                                                       0x100000000ULL,
                                                       0x200000000ULL,
                                                       0x400000000ULL, /* 16 GB */
                                                       0x800000000ULL,
                                                       0x1000000000ULL,
                                                       0x2000000000ULL};/* 32 slots */
/* Addresses for 1 qword access for GCM and VLM are byte address, 3 LSB not used
 * In the main modify the lower addresses used based on the size of packet being used. */
static int64_t Byte_Address_Table[MAX_STUCK_BIT_ADDRESS] = {0x0ULL,
                                                            0x8ULL,
                                                            0x10ULL,
                                                            0x20ULL,
                                                            0x40ULL,
                                                            0x80ULL,
                                                            0x100ULL,
                                                            0x200ULL,
                                                            0x400ULL,
                                                            0x800ULL,
                                                            0x1000ULL,
                                                            0x2000ULL,
                                                            0x4000ULL,
                                                            0x8000ULL,
                                                            0x10000ULL,
                                                            0x20000ULL,
                                                            0x40000ULL,
                                                            0x80000ULL,
                                                            0x100000ULL,
                                                            0x200000ULL,
                                                            0x400000ULL,
                                                            0x800000ULL,
                                                            0x1000000ULL,
                                                            0x2000000ULL,
                                                            0x4000000ULL,
                                                            0x8000000ULL,
                                                            0x10000000ULL,
                                                            0x20000000ULL,
                                                            0x40000000ULL,
                                                            0x80000000ULL,
                                                            0x100000000ULL,
                                                            0x200000000ULL};/* 32 slots */

/* Addresses for OBM are Qword and 1 qword packet size used. */
static int64_t OBM_Address_Table[MAX_STUCK_BIT_ADDRESS] = {0x0ULL,
                                                           0x1ULL,
                                                           0x2ULL,
                                                           0x4ULL,
                                                           0x8ULL,
                                                           0x10ULL,
                                                           0x20ULL,
                                                           0x40ULL,
                                                           0x80ULL,
                                                           0x100ULL,
                                                           0x200ULL,
                                                           0x400ULL,
                                                           0x800ULL,
                                                           0x1000ULL,
                                                           0x2000ULL,
                                                           0x4000ULL,
                                                           0x8000ULL,
                                                           0x10000ULL,
                                                           0x20000ULL,
                                                           0x40000ULL,
                                                           0x80000ULL,  /* SRC-7 OBM bank qword */
                                                           0x100000ULL, /* Saturn OBM bank qword */
                                                           0x200000ULL,
                                                           0x400000ULL,
                                                           0x800000ULL,/*  */
                                                           0x1000000ULL,
                                                           0x2000000ULL,
                                                           0x4000000ULL,
                                                           0x8000000ULL,
                                                           0x10000000ULL,
                                                           0x20000000ULL,
                                                           0x40000000ULL};/* 32 slots */


void Stuck_Bit_Table (  
  uint64_t *AR,
  uint64_t *ER,
  int64_t   Address_Table[],   /*in Addresses to check qword 32qw*/
  int       Number_Of_Address, /*in Count of addresses to check */
  int       Number_Of_Bank,    /*in Count of banks */
  int64_t   Chunk_Size,        /*in Size in words of the chunk used to test address */
  int       Offset,            /*in Address line to Address Bit offset. OBM = 1; GCM =0 */
  char     *Test_Name,         /* input, type of stuck bit test */
  int       Trace,
  int       Verbose,           /* input, True display to stdout and file */
  FILE     *logfile,            /* Pointer to file */
  int       Log_File_Defined ); /* boolean true if a log file exists */
 

 
/* *****************************************************************************************
 * Address_Bit  determine the bits that were tested for the address.
 *
 * */

int  Address_Bit ( uint64_t Addr );  /* */


#endif /* FT_COMMON_UTIL_H  */
