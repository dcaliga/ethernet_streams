
#include <libmap.h>

/* socket related packages */
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define CETH_ADDRESS "10.0.0.213"
#define SETH_ADDRESS "10.0.0.213"
#define ETH_PORT 11211

#define WORD_SIZE 8
#define MAX_WORDS 176

void vec_stream_64_from_eth_term__dbg (Vec_Stream_64 *S, int num_vecs, int mapnum) {
    unsigned i, j;
    unsigned num_vals;
    uint64_t data_in[176];
    int64_t data;
    int Byte_Count;

    int socket_fd = 0;

    struct sockaddr_in servaddr;

    socket_fd=socket (AF_INET, SOCK_DGRAM,0);

    if((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) /* @@UDP  */
    {
      printf ("Could not create socket.\n");
      exit(1);
    }

    memset (&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr (SETH_ADDRESS);
    servaddr.sin_port = htons (ETH_PORT);

  printf ("here1 in from_eth\n");

    for (j=0; j<num_vecs; j++) {

  printf ("from_eth j %i b4 recvfrom \n",j);
     Byte_Count =recvfrom(socket_fd,
          data_in, WORD_SIZE*MAX_WORDS,0,
          NULL,NULL);
  printf ("from_eth j %i af recvfrom \n",j);

        data = data_in[0];

 printf ("from_eth data %016llx\n",data);

        // write vec stream header, with the length in bytes
        put_vec_stream_64_header__dbg (S, (int64_t)data*8, mapnum);
        num_vals = data & 0xffffffff;

        for (i=0; i<num_vals; i++) {
            data = data_in[i+1];
     printf (" data %016llx\n",data);
            put_vec_stream_64__dbg (S, (int64_t)data, 1, mapnum);
        }

        put_vec_stream_64_tail__dbg (S, 0, mapnum);
    }

    vec_stream_64_term__dbg (S);
}

void vec_stream_64_to_eth_term__dbg (Vec_Stream_64 *S, int mapnum) {
    unsigned i, j,count;
    unsigned num_vals;
    uint64_t data_in[176];
    int64_t data;

    int socket_fd = 0;

    struct sockaddr_in clientaddr;

    socket_fd=socket (AF_INET, SOCK_DGRAM,0);

    if((socket_fd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) /* @@UDP  */
    {
      printf ("Could not create socket.\n");
      exit(1);
    }

    memset (&clientaddr, 0, sizeof(clientaddr));
    clientaddr.sin_family = AF_INET;
    clientaddr.sin_addr.s_addr = inet_addr (CETH_ADDRESS);
    clientaddr.sin_port = htons (ETH_PORT);


    while (is_vec_stream_64_active__dbg (S)) {

        get_vec_stream_64_header__dbg (S, &data);
        count = 0;


        while (all_vec_streams_active_1__dbg (S)) {
            get_vec_stream_64__dbg (S, &data);
            data_in[count] = data;
            count++;
        }

        sendto(socket_fd,
          data_in, WORD_SIZE*count,0,
         (struct sockaddr *)&clientaddr,sizeof(clientaddr));

       get_vec_stream_64_tail__dbg (S, &data);
        
    } // end of while

}
