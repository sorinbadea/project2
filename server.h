#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <assert.h>
#include <thread>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <fcntl.h>

#include "messages.h"

class server {
protected:
   /**
    * socket data */
   socklen_t p_peer_addr_size;
   struct sockaddr_in p_servaddr;
   struct sockaddr_in p_client;

   /**
    * communication socket */
   int p_sockfd;

   /**
    * fd provided by accept */
   int p_a_sockfd;
   
   /**
    *  UDP or TCP connection, SOCK_DGRAM or SOCK_STREAM 
    */
   int p_conn_type;

   explicit server (const unsigned int comm_port);

   /**
    * queue size
    */
   unsigned int p_queue_size;

public:
   virtual ~server();

   /**
    * create the socket server, than bind
    */
   void server_setup();

   /**
    * for TCP sockets:
    *    calls "listen" on a created socket
    * for UDP servers it does nothing
    */
   virtual void server_listen() = 0;

   /**
    * for TCP servers:
    * close the fd provided by accept
    * for UDP serveres: nothing */
   virtual void fd_close(int) = 0;

   /** 
    * UDP servers: blocks until a request arrives
    * TCP servers: calls "accept" and
    * returns the fd provided by accept;
    *
    */
   virtual int server_wait() = 0;

   /**
    * read,write interface;
    * separate impl. for UDP and TCP server
    */
   virtual int cls_read(int, void*, size_t) = 0;
   virtual ssize_t cls_write(int, void*, size_t) = 0;
};

/** 
  TCP server impl.
*/
class server_tcp : public server {

public:

   explicit server_tcp(const unsigned int, const unsigned int);
   ~server_tcp();

   /**
    * calls "listen" on a created socket
    */
   void server_listen();

   /**
    * blocks until a request arrive
    */
   int server_wait();

   /**
    * release fd
    */
   void fd_close(int);

   /*
    * TCP server read specialization
    */
   int cls_read(int, void*, size_t);

   /**
    * TCP server write specialzation
    */
   ssize_t cls_write(int, void*, size_t);
};

/**
  UDP server impl.
*/
class server_udp : public server {

   fd_set p_set;
   struct timeval p_timeout;

public:

   server_udp(const unsigned int, const unsigned int);
   ~server_udp();

   /**
    * does nothing
    */
   void server_listen();

   /**
    * blocks until a request arrive
    */
   int server_wait();

   /**
    * release the fd
    */
   void fd_close(int);

   /**
    * UDP server write specialzation
    */
   int cls_read(int, void*, size_t);

   /**
    * UDP server write specialization
    */
   ssize_t cls_write(int, void*, size_t);
};

#endif
