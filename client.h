#ifndef CL_SERVER_H
#define CL_SERVER_H
#include <iostream>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>

class client {

   /**
     communication socket
   */
   int p_sockfd;
   socklen_t p_peer_addr_size;
   struct sockaddr_in p_servaddr;
   struct sockaddr_in p_client;

   /**
    *  select params 
    */
   fd_set p_set;
   struct timeval p_timeout;

public:
    
   explicit client (const unsigned int comm_port);
   ~client();

   void client_connect();

   /*
    * @param: buffer: write message buffer
    * @param: bytes: message lenghth in bytes
    */
   size_t send_message (const unsigned char* buffer, const unsigned int bytes);

   /*
    * @param: buffer: read message buffer
    * @param: timeout: timeout in seconds
    */
   size_t read_message (char* buffer, unsigned int timeout);

};

#endif
