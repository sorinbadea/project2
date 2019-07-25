#ifndef SERVER_APP_H
#define SERVER_APP_H
#include "server.h"
#include "worker.h"
#include "messages.h"
#include <vector>

class server;
class server_tcp;
class server_udp;

class server_app {

   std::shared_ptr<server> p_srv;

   /**
    * request deserialization pointer
    */
   unsigned char* p_header;
   unsigned char* p_message;

   std::mutex p_handle_request_mutex;

   /**
    * pointer to a worker instance
    * each worker are dedicated to a speciffic request:
    * test request, registration request, connection request
    **/
   worker* p_worker;

   /**
    * file descriptor to reply at
    */
   int p_fd;

   /**
    * execute and reply the result to server
    * @param: none
    **/ 
   void thread_server_reply();

public:

   server_app(const server_app& ) = delete;
   server_app& operator=(const server_app&) = delete;

   ~server_app();

   /**
    * server app constructor
    * @param: s_type : server type UDP or TCP
    * @param: port : communication port
    * @param: q_size : queue size
    */
   explicit server_app(const server_type s_type, const unsigned int port, const unsigned int q_size);

   /**
    * subscribe to a speciffic request
    * @param: message type to subscribe at
    */
   void subscribe( const unsigned int message_id);

   /**
    *start a server instance
    */
   void start();

   /**
    * handle client requests
    * TEST/REGISTRATION/CONNECTION
    * @param: fd : file descriptor to reply at
    **/ 
   void handle_request (int fd);
};
#endif
