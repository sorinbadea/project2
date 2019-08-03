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
#include <mutex>

class server_app;
class server_io;

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
    *  UDP or TCP connection, SOCK_DGRAM or SOCK_STREAM */
   int p_conn_type;

   explicit server (const unsigned int comm_port);

   /**
    * references to server_app and server_io classes
    */
   server_app* p_server_app;

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
   virtual void fd_close() = 0;


   /** 
    * UDP servers: blocks until a request arrives
    * TCP servers: calls "accept" and
    * returns the fd provided by accept;
    *
    */
   virtual int server_wait() = 0;
};

class server_tcp : public server {

   unsigned int p_queue_size;

public:

   server_tcp(const unsigned int, const unsigned int, server_app*);

   ~server_tcp();

   /**
    * calls "listen" on a created socket
    */
   void server_listen();

   /**
    * thread handling a new connection request
    * using the fd returned by accept()
    */
   void thread_io(int fd);

   int server_wait();

   void fd_close();
};


class server_udp : public server {

   unsigned int p_queue_size;

   fd_set p_set;
   struct timeval p_timeout;

public:

   server_udp(const unsigned int, const unsigned int, server_app*);

   ~server_udp();

   /**
    * does nothing
    */
   void server_listen();

   /**
    * thread handling a new connection request
    * using the fd returned by accept()
    */
   void thread_io(int fd);

   int server_wait();

   void fd_close();
};

#endif
