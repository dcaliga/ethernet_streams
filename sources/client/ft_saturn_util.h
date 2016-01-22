/*  $Id: ft_saturn_util.h 199 2014-02-06 22:19:38Z quick $  */

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
 * values from above:
 * SATURN_NUMBER_OF_VLM
 * SATURN_SIZE_OF_VLM_QWORD
 * 
 */
/* * Options:
 *       
 *       */
#ifndef FT_SATURN_UTIL_H
#define FT_SATURN_UTIL_H 1

/* Sizes of saturn resources */
#define SATURN_NUMBER_OF_VLM  2  /* Number of VLM banks */

#ifdef MODE   /* if exists Sim mode */
 #define SATURN_SIZE_OF_VLM    (int64_t)1  /* Size of VLM banks GB */

#else  /* HW values  */

 #define SATURN_SIZE_OF_VLM    (int64_t)16  /* Size of VLM banks GB */


#endif

#define   SATURN_SIZE_OF_VLM_QWORD (SATURN_SIZE_OF_VLM * (int64_t)GIGIBYTES_TO_QWORDS) 

#define SATURN_BRAM_ARRAY_MAX          32768   /* Max array qw size. H */


/* ***********************************************************************************
 * Saturn Common main procedures
 * 
 *   Display_Title_Options is the only one different than the SRC-7
 *   
 * */
void Saturn_Display_Title_Options( 
  int      Display_VLM_Flag,
  int      Display_Bank_Flag,      /*  */
  int      Display_Bank_Mask_Flag, /*   */
  int      Display_Multi_VLM_Flag, /* If set, then display both VLM as being used */
  int      Display_Robustness_Flag,/* for Number_Of_Reads use write/read or read */
  int      Display_Data_Port_Flag, /* Display the Data Port number  */
  int      Data_Load_Flag,     /*  option -F number  */
  int      VLM_Port,           /*  Port of tested CM */
  uint64_t Size_Of_Data_Used,  /* Size_Of_Data for display */
  uint64_t Bank,               /* OBM_bank to use  */
  uint64_t VLM_Address,        /* VLM address */
  int      Number_Of_Reads,    /* Number of reads for robust */
  int      Data_Port,          /* Data Port number  */
  char     *Test_Name,         /* input */
  char     *Title_Options,     /* Output: Title  string */
  int      Trace );

/* ***********************************************************************************
 * Saturn Functional tests (FT) procedures
 * Not used...
 * ??Not needed remove!!
 *   
 *   
 * */
int  Saturn_Print_Test_Case( 
  int      Verbose_Flag,    /*  Verbose mode */
  int      Error_Count,     /* determine pass fail for data errors  */
  int      System_Error,    /* determine pass fail for system errors */
  int      Serial_Number,   /* manufacturer Serial Number of tested Saturn board */
  int      Chassis_Slot,    /*  Slot of tested Saturn board */
  int      Chassis_Port,    /*  Port of tested Saturn board */
  char     *Test_Number,    /* input: string of the FT test case */
  char     *Test_Name,      /* input: */
  int      Trace,
  FILE    *logfile,         /* Pointer to file */
  int      Log_File_Defined /* boolean true if a log file exists */
                             );

/* ***********************************************************************************
 * Saturn Functional tests (FT) procedures
 * Print_Test_Case_Errors
 *   display the components that failed, this just formats
 *   goes to stdout and file if created. 
 *   If no data errors there is not a display
 *   
 * */
void  Print_Test_Case_Errors( 
  int      Verbose_Flag,    /*  Verbose mode */
  int      Trace,
  int      Fail_Count,       /* int count of errors */
  int      OBM_Fail_Table[], /*in boolean*/
  int      VLM_Fail_Table[], /*in boolean*/
  int      Port_Fail_Table[], /*in boolean*/
  int      Other_Flag,      /* boolean True if component failed */
  char     *Other_Error,    /* input: string with the OTHER errors defined */
  FILE    *logfile,         /* Pointer to file */
  int      Log_File_Defined /* boolean true if a log file exists */
                             );


#endif /* FT_SATURN_UTIL_H  */
