static char const cvsid[] = "$Id: satft_06.c 199 2014-02-06 22:19:38Z quick $";
/*
 *     
 *
 *	Copyright 2013 SRC Computers, LLC.  All Rights Reserved.
 *
 *		Manufactured in the United States of America.
 *
 *	SRC Computers, LLC.
 *	4240 N Nevada Avenue
 *	Colorado Springs, CO 80907
 *	(v) (719) 262-0213
 *	(f) (719) 262-0223
 *
 *	No permission has been granted to distribute this software
 *	without the express permission of SRC Computers, LLC.
 *
 *	This program is distributed WITHOUT ANY WARRANTY OF ANY KIND.
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/time.h>
#include <errno.h>

#include <map.h>
#include <common_mem.h> // requires something from map.h
#include <globals.h>
#include <maplib.h>
#include <direct_cmd.h>
#include "ft_common_util.h"
#include "ft_saturn_util.h"

/* socket related packages */
#include <arpa/inet.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

 
#define ntohll(x) (((int64_t)(ntohl((int)((x << 32) >> 32))) << 32) | (unsigned int)ntohl(((int)(x >> 32)))) //By Runner
#define htonll(x) ntohll(x)

struct timespec sleep_time, rem_time;
struct timespec resolution;

void subr (int nvec, int nsamp_4dma, int64_t *time, int mapnum);
 
#define MAXVECS 1024


  int Number_Of_Vectors; /* passed to user logic set to 1 */

/* socket related objects */
#define WORD_SIZE 8

#define BUFFER_SIZE   176  /* allow for size word. 1520 - header info bytes max for UDP */
//define BUFFER_SIZE   50  /* allow for size word. 1520 - header info bytes max for UDP */
#define BUFFER_SIZE_BYTE   (WORD_SIZE*BUFFER_SIZE)  /**/


//#define ETH_ADDRESS "192.168.0.151"
#define SERVER_ADDRESS "10.0.0.213"
#define ETH_PORT 11211

struct sockaddr_in server_addr; 
struct hostent *server;

#include <pthread.h>

int Byte_Count;

  int64_t *HO64;
  int32_t *HO32;
  int16_t *HO16;

  int64_t *HI64;
  int32_t *HI32;
  int16_t *HI16;

  int64_t *RI64;
  int32_t *RI32;
  int16_t *RI16;
  uint8_t  *RI08;

  int64_t *send_buf;
  int64_t *recv_buf;
  int64_t *cpu_buf;
  int64_t *map_buf;
  int64_t *temp_rcv;

  int64_t *A64, *B64, *Counts;

  FILE *res_map, *res_cpu;

  int slen=sizeof(server_addr);

  struct sockaddr_in server_addr;
  char ip_test_addr[256];
  char *ipaddr = &ip_test_addr[0];
  int ip_port;
  int nvec;

  int ij2,ij3;
  int iprint,iflag;

  int send_err[256];
  int recv_err[256];
  int nsend_err;
  int nrecv_err;

  int socket_fd = 0;
  int nano_value;
  int sleep_delay;

#define DEF_NVECS 5
#define DEF_PORT  11211

 /* ***********************************************************
 *   Start main procedure
 * ***********************************************************/
main(int argc, char *argv[])
{
  MAP_SCA_Status_t  *MAP_status;
  /* 
   * Definitions.
   */
  int i,j,k, ij, jbgn, nerr;

  int r;
  int npackets;

  char c;

  int maxlen;
  int64_t tm,i64,j64;

  int32_t i32,ij1,cnt;


  int64_t temp[2];
  uint64_t data;
  int64_t  tmp;
  int32_t last,total_cnt;
  int ip_port_root;

  int veclen;

  int total_nsamp;
  int remaining, packet_size;
  int stat;

  void *proc0 (void *p);
  void *proc1 (void *p);

  pthread_t thread0, thread1;


// Buffers for MAP
  
  int mapnum = 0;          /* Logical MAP Number not port number.  */
  

    nvec = DEF_NVECS;
    maxlen = 1000;
    //ipaddr = "10.1.10.2";
    ipaddr = "ajaxc02p3";
    ip_port_root = 11221;
    iprint = 0;
    iprint = 0;
    nano_value = 100;
    sleep_delay = 5;
    veclen = 20;

    while ((c = getopt(argc, argv,
        "n:m:a:p:s:S:l:fvh")) != EOF) {
        switch (c) {
    case  'n':
        nvec  = atoi(optarg);
        printf("Number of vectors        %6i  \n",nvec);
        break;
    case  'm':
        maxlen = atoi(optarg);
        printf("Maximum length of vector %6i  \n",maxlen);
        break;
    case  'a':
        ipaddr = optarg;
        printf ("ipaddr %s \n",ipaddr);
        break;
    case  'p':
        ip_port_root = atoi(optarg);
        printf("Server Port          %6i \n",ip_port_root);
        break;
    case  's':
        nano_value = atoi(optarg);
        printf("Nanosec to sleep between sends            %6i  \n",nano_value);
        break;
    case  'S':
        sleep_delay = atoi(optarg);
        printf("Number of seconds to wait before sending  %6i  \n",sleep_delay);
        break;
    case  'l':
        veclen     = atoi(optarg);
        printf("Length of vector     %6i  \n",veclen);
        break;
    case  'f':
        iflag  = 1;
        printf("Output data has the vector number added \n");
        break;
    case  'v':
        iprint = 1;
        printf("Print details of vectors sent/received  \n");
        break;
    case 'h':
    default:
        printf("-n Number of vectors    Default is %i  Max number %i  \n",DEF_NVECS,MAXVECS);
        printf("-m Maximum length of vector\n");
        printf("-a Server Address         \n");
        printf("-p Server Port          Default is %i    \n",DEF_PORT);
        printf("-s Nanosec to sleep between sends  Default is %6i  \n",nano_value);
        printf("-S Number of seconds to wait before sending  Default is %2i sec  \n",sleep_delay);
        printf("-l Length of vector     Default is 20     \n");
        printf("-f Output data has the vector number added \n");
        printf("-v Print details of vectors sent/received  \n");
        exit(0);
    }
    }


  printf ("**** Summary ****\n");
  printf ("Number of Vectors        %d\n",nvec);
  printf ("Maximum length of vector %d\n",maxlen);
  printf ("Server IP Address        %s\n",ipaddr);
  printf ("Server Port              %d\n",ip_port_root);
  printf ("Nanosec to sleep         %6i\n",nano_value);
  printf ("Delay before sending     %6i\n",sleep_delay);
  if (iflag)
     printf ("Output data has the vector number added \n");
  if (iprint)
     printf ("Print details of vectors sent/received  \n");

   ip_port = ip_port_root;

   Size_Of_Data = maxlen;
  
   printf ("ipaddr %s : ip_port %d \n",ipaddr, ip_port);
  
  /* The data size is limited to the size of a socket buffer
   * TCP 2048 bytes
   * UDP 65kb??
   * */
    
    printf ("\n  Max Packet data size is %d qwords. Max Input size: %lld \n",
            BUFFER_SIZE, Size_Of_Data);
    

   stat = clock_getres(CLOCK_REALTIME, &resolution);

   printf ("resolution in nsec %10lli\n",resolution.tv_nsec);
 
  
  /* ****************************************************************
   *  Set Up MAP
   *  
   *    Map Allocate defines the data structures needed for the MAP.
   *  @@
   * ****************************************************************/
  

    if ((res_map = fopen ("res_map", "w")) == NULL) {
        fprintf (stderr, "failed to open file 'res_map'\n");
        exit (1);
        }

    if ((res_cpu = fopen ("res_cpu", "w")) == NULL) {
        fprintf (stderr, "failed to open file 'res_cpu'\n");
        exit (1);
        }

    Counts = (int64_t*) malloc (nvec * sizeof (int64_t));

    total_nsamp = 0;
    for (i=0; i<nvec; i++) {
        //Counts[i] = random () % maxlen;

     //   Counts[i] = (i/3 + 1)*20;
        Counts[i] = veclen;
//  printf ("Line %2i Samples %4lli\n",i,Counts[i]);
        total_nsamp = total_nsamp + Counts[i];
    }

    A64      = (int64_t*) malloc ((total_nsamp+nvec) * sizeof (int64_t));
    B64      = (int64_t*) malloc ((total_nsamp+nvec) * sizeof (int64_t));
    send_buf = (int64_t*) malloc (2*BUFFER_SIZE * sizeof (int64_t));
    recv_buf = (int64_t*) malloc (2*BUFFER_SIZE * sizeof (int64_t));
    temp_rcv = (int64_t*) malloc (2*sizeof(int64_t));
    cpu_buf  = (int64_t*) malloc ((total_nsamp+nvec) * sizeof (int64_t));
    map_buf  = (int64_t*) malloc ((total_nsamp+nvec) * sizeof (int64_t));

      HO64 = (int64_t*)temp;
      HO32 = (int32_t*)temp;
      HO16 = (int16_t*)temp;

      RI64 = (int64_t*)temp_rcv;
      RI32 = (int32_t*)temp_rcv;
      RI16 = (int16_t*)temp_rcv;
      RI08 = (uint8_t*)recv_buf;

    ij1 = 0;
    ij2 = 0;
    for (i=0;i<nvec;i++) {
      cnt = Counts[i];

      packet_size = cnt;


      last = 1;
      if (last) i32 = (cnt | 0x40000000);

// format packet header
      HO32[0] = i32;
      HO32[1] = packet_size*8;
      fprintf (res_cpu, "%10lld\n",temp[0]);
      cpu_buf[ij2] = temp[0]; ij2++;

      for (j=0;j<cnt;j++) {
           i64 = j + (i+1)*4096;
           j64 = i64 + 0x100000;
           if (iflag) j64 = j64 + i*0x1000000;

           A64[ij1] = i64; ij1++;      // input data
           cpu_buf[ij2] = j64; ij2++;  // cpu output data

           fprintf (res_cpu, "%10lld\n",j64);
           }
    }

  
    
    if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0) 
    {
      printf ("Could not create socket.\n");
      exit(1);
    } 
    else
	  printf ("created socket ***\n");

    memset (&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family      = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr (ipaddr);
    server_addr.sin_port        = htons (11211);

   if (connect(socket_fd, (struct sockaddr *)&server_addr, sizeof(server_addr))<0) {
      printf ("connect socket cpu failed\n");
      perror("connect failed");
      exit(1);
      }
   else
	 printf ("connected ***\n");

#ifndef MODE /* if not exists HW mode */
  printf ("\nbefore usleep\n");
  sleep (sleep_delay);
  printf ("after  usleep\n");
#endif


      HO64 = (int64_t*)send_buf;
      HO32 = (int32_t*)send_buf;
      HO16 = (int16_t*)send_buf;

      HI64 = (int64_t*)recv_buf;
      HI32 = (int32_t*)recv_buf;
      HI16 = (int16_t*)recv_buf;

      pthread_create (&thread0, NULL, &proc0, NULL);
      pthread_create (&thread1, NULL, &proc1, NULL);

      pthread_join (thread0, NULL);
      pthread_join (thread1, NULL);
  
 printf ("\n*** after join ***\n");

      close(socket_fd);

  if (ij2!=ij3)  {
    printf ("*** Error *** input sample count %3i  output sample count %3i\n",ij2,ij3);
  }
  else {
      printf ("\n*** Sample counts match\n");
      nerr = 0;
      for (i=0;i<ij2;i++)  {
        if (cpu_buf[i] != map_buf[i]) { 
           nerr++;
           if (nerr>0 && nerr<6) printf ("  i %3i  cpu %016llx  map %016llx\n",i,cpu_buf[i],map_buf[i]);
        }
      }

      if (nerr > 0)  printf ("*** Sample value mismatch  no. of errors %3i\n",nerr);
      else           printf ("*** Success              \n");
   }

}


void *proc0 (void *p)
{
  int ij1,ij2,i,k;
  int packet_size,cnt,last,i32;
  int test,errno,nbytes;

  ij1 = 0;
  ij2 = 0;
  nsend_err = 0;

  for (i=0;i<nvec;i++) {
     cnt = Counts[i];
    
         packet_size = cnt;


      last = 1;
      if (last) i32 = (cnt | 0x40000000);

// format packet header
      HO32[0] = i32;
      HO32[1] = packet_size*8;
      
       

         printf ("A64      %016llx  ij1 %i\n",A64[ij1],ij1);
      for (k=0;k<packet_size;k++)  {
         if ((k<5 | k>(packet_size-5)) && iprint) printf ("A64      %016llx  ij1 %i\n",A64[ij1],ij1);
         send_buf[k+1] = A64[ij1]; ij1++;
      }

   sleep_time.tv_sec = 0;
   sleep_time.tv_nsec = nano_value;

   test = nanosleep (&sleep_time, &sleep_time);

   //printf ("rem_time sec %lli nan %lli nano_val %i\n",rem_time.tv_sec,rem_time.tv_nsec,nano_value);
   printf ("rem_time sec %lli nan %lli nano_val %i\n",
           sleep_time.tv_sec,sleep_time.tv_nsec,nano_value);

// printf ("af k loop ij1 %i\n",ij1);

  /* send data to server. */
       
       nbytes = WORD_SIZE*(packet_size+1);

       errno =send(socket_fd, send_buf, nbytes, 0);
       if (errno == nbytes)  printf ("send okay\n");
          if (errno < 0)  {
            printf ("                    **** err sendto server\n");
  
            int errsv = errno;
            printf ("                                  ****** sendto errsv % *****i\n",errsv);
            send_err[i] = errno;

            nsend_err++;
          }

 printf ("i %2i main af sendto bytes %4i\n",i,nbytes);
  
      } // end nvec loop

      if (nsend_err > 0)  {
         printf ("******** send errors *************\n");
         for (i=0;i<nvec;i++)  {
           printf ("ivec %2i error %3i\n",i,send_err[i]);
         }
      }

   return NULL;

} /* Thread end  */
  
void *proc1 (void *p)
{
  int i,j,ij,jj,jbgn,Byte_Count;
  int cnt,last,i32,nvec_recv;
  int64_t i64,j64;
  int8_t  c3;

    ij = 0;
    ij3 = 0;
    jbgn = 0;

    nvec_recv = 0;
    do {

        Byte_Count = 0;
        recv_buf[0] = -1;
/*
        Byte_Count =recvfrom(socket_fd,
             recv_buf, WORD_SIZE*BUFFER_SIZE,0,
             NULL,NULL);
*/
        Byte_Count =recv(socket_fd,
             recv_buf, WORD_SIZE*BUFFER_SIZE,0);

        printf ("                                      nvec %2i main af recvfrom bytes %4i\n",
                                                       nvec_recv,Byte_Count);


          
        if (Byte_Count < 0)  {
            int errsv = Byte_Count;
            printf ("                                  ****** recvfrom errsv % *****i\n",errsv);
            recv_err[i] = errsv;

            nrecv_err++;
        }

       printf ("ri08  ");
        for (j=0; j < 8; j++) 
       printf ("%x ",RI08[j]);
       printf ("\n");

        cnt = Byte_Count/8;
        for (j=0; j < cnt; j++) {
            jj = 3 + j*8;
            c3 = RI08[jj];
 if (j==0) printf ("c3 %x\n",c3);
            if (c3==0x40) { nvec_recv++; printf ("*** got vec header\n"); }
            i64 = recv_buf[j];

           if (iprint) printf ("B64   ij3 %3i   %016llx\n",ij3,i64);
           map_buf[ij3] = i64; ij3++;

          fprintf (res_map, "%10lld\n", i64);
        }


  } while (nvec_recv < nvec);  // loop over vectors
    
        close(socket_fd);

      if (nsend_err > 0)  {
         printf ("******** recv errors *************\n");
         for (i=0;i<nvec;i++)  {
           printf ("ivec %2i error %3i\n",i,recv_err[i]);
         }
      }
   
 
   return NULL;

} /* Thread end  */

