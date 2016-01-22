/*  $Id: ft_user_logic.h 35 2013-10-10 21:58:23Z quick $  */

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
 * Data pattern definitions so there can be included into the user logic. The common_util.h
 * include the map.h header which cannot be included to the user logic. 
 */

#ifndef FT_USER_LOGIC_H
#define FT_USER_LOGIC_H 1

/* *******************************************
 * Define the basic FT user logic sizes.
 * 
 * *******************************************/
#define RETURN_LENGTH (16)   /* Debug return in qwords change in .c also */

#define BRAM_ARRAY_SIZE 1024 /* smaller BRAM size used in 12,8,7 */

/* *******************************************
 * Define the Saturn sizes.
 * 
 * *******************************************/


// defining the OBM sizes here will sometimes cause problems in the Debug .mc compile.
//#ifdef MODE   /* if exists Sim mode */
 // #define  BRAM_ARRAY_MAX          20000  /* Max array qw size for SIM/debug modes. */

//#else  /* HW values  */
  #define  BRAM_ARRAY_MAX          32768   /* Max array qw size. series J/L */

//#endif


/* *******************************************
 * Data_Load_Flag type
 *   Add define here
 *   Add pattern generate in Load_Data procedure. 
 *   Add pattern name in Display_Data_Pattern procedure.
 *   Add to help CMN_DATA_PATTERNS in common_util.h
 *
 *   NOTES:
 *    pattern 6 is the externally generated patterns for the on chip generate
 *    and compare data.
 *   
 * *******************************************/
#define DATA_NO_PATTERN               999999  /* Pattern not used, for display */
#define DATA_EXTERNAL_PATTERN         6  /* Generated external to user chip, not option */
#define DATA_ZERO                     0  
#define DATA_SEQ                      1  /* Count */
#define DATA_ALL_ONE                 11  /* All ones */
#define DATA_ALL_A                   12  /* All hex A */
#define DATA_F00F                    13  /* F00F */
#define DATA_0FF0                    14  /* 0FF0 */
#define DATA_ALL_FIVE                15  /* All hex 5 */
#define DATA_RESEED                   2  
#define DATA_RANDOM                   3  
#define DATA_RANDOM_UPPER_HALF       31  /* Random in upper part of qword count other half*/
#define DATA_RANDOM_LOWER_HALF       32  /* Random in lower part of qword count other half*/
#define DATA_PATTERN_SEQ              4  /* Pattern the sequence. */
#define DATA_PATTERN_CHUNKS          41  /* upper and lower sequence. */
#define DATA_RANDOM_CHUNKS           42  /* upper random and lower sequence. */
#define DATA_PATTERN_INVERT           5  /* -F5 SLAM              */
#define DATA_PATTERN_INVERT_50       50  /* -F50 SLAM             */
#define DATA_PATTERN_INVERT_LOWER    51  /* Half SLAM Lower word */
#define DATA_PATTERN_INVERT_UPPER    52  /* Half SLAM Upper word */
#define DATA_PATTERN_INVERT_MIDDLE   53  /* Half SLAM Middle word  */
#define DATA_PATTERN_INVERT_ODD      54  /* Half SLAM ODD bits  */
#define DATA_PATTERN_INVERT_EVEN     55  /* Half SLAM Even bits  */
#define DATA_PATTERN_INVERT2          8  /* -F8 SLAM every 2 qwords     */
#define DATA_PATTERN_INVERT2_TOO     82  /* -F8 SLAM every 2 qwords     */
#define DATA_PATTERN_INVERT4         84  /* -F8 SLAM every 4 qwords     */
#define DATA_PATTERN_INVERT8         88  /* -F8 SLAM every 8 qwords     */
#define DATA_RANDOM_LM               58  /* Lock Mart TRACER Noise  data pattern*/
#define DATA_RANDOM_LMZ              59  /* Lock Mart random generated pattern data and zeros*/
#define DATA_RANDOM_DEFENDER         57  /* Lock Mart Defender random generated pattern data */

/* Special patterns  */

#define DATA_SPECIAL_LOAD_0          70  /* Special hardcoded pattern  old 92 */
#define DATA_ECC_PATTERN             71  /* Special pattern to touch the ECC values  old 91 */
#define DATA_MAGIC_PATTERN           72  /* Jeff's bad patterns randomly mixed. */
#define DATA_MAGIC_PATTERN_SEEDED    73  /* Jeff's bad patterns randomly mixed,
                                          * with all f and 0 alternated (inserted into pattern). */
#define DATA_SIOX_PATTERN            74  /* Defender SIOX special pattern. */

/* New Saturn related  */
#define DATA_WALKING1_PATTERN        75  /* Walking ones 65 then repeat pattern. */
#define DATA_WALKING1_INVERT         751 /* Inverted Walking ones 65 then repeat pattern. */
#define DATA_FULL_INVERT_EVEN        501 /* Full SLAM even bits       */
#define DATA_FULL_INVERT_ODD         502 /* Full SLAM odd bits       */
#define DATA_SEQ_BYTE_127            16  /* Count pattern in every byte to 127, restart pattern */
#define DATA_SEQ_BYTE_127_INVERT     161 /* Inverted Count pattern in every byte to 127,
                                          * restart pattern */


/* Read from file related */

#define DATA_FROM_FILE_HEX           931  /* file test pattern expected hex and size in 1st item*/
#define DATA_FROM_FILE_DEC           941  /* file test pattern expected dec and size in 1st item*/
#define DATA_FROM_FILE_I             951  /* file test pattern expected I format and size in 1st item*/
#define DATA_FROM_FILE_HEX_1         93  /* file test pattern expected hex, use 1st value */
#define DATA_FROM_FILE_DEC_1         94  /* file test pattern expected dec, use 1st value */
#define DATA_FROM_FILE_I_1           95  /* file test pattern expected I format, use 1st value */

#define DATA_FROM_FILE_BIN           96  /* file read in as binary, not ASCII */

/* internal processing related */
#define DATA_PATTERN_BIT_WALK        196  /* used for Bit walk test pattern.
                                           *  Meaningless with -FB */
#define DATA_INVERTED_BIT_WALK       197  /* used for Bit walk test pattern.
                                           *  Meaningless with -FB */
#define DATA_INVERT                  198  /* used for inverting data from iteration 
                                           * to iteration. Meaningless with -FB */
#define DATA_DO_NOTHING              199  /* Do not change contents. */


#endif /* FT_USER_LOGIC_H  */
