#ifndef SERVER_APP_H
#define SERVER_APP_H
#include "server.h"
#include "worker.h"
#include "messages.h"
#include "exception.h"
#include <map>
#include <mutex>
#include <condition_variable>

/**
 * forward declarations 
*/
class server;
class server_tcp;
class server_udp;

class server_app {

   std::shared_ptr<server> p_srv;

   /**
    * mutex for threads synchronization
    */
   std::mutex p_thread1;
   std::mutex p_main_loop;

   /**
    * thread proccesing the request condition variable */
   std::condition_variable p_cv_thread1;

   /**
    * main loop condition variable 
    */
   std::condition_variable p_cv_main_loop;

   /**
    * the processing thread done it's job
    */
   bool p_handle_request_ready = false;

   /**
    * socket to respond at 
    */
   int p_fd1;

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
    * start a server instance
    */
   void start();

   /**
    * thread handle a client requests i.e: TEST/REGISTRATION/CONNECTION
    **/ 
   void handle_request1 (int);
};

class handle_request {

   /**
    * request deserialization pointer
    */
   unsigned char* p_header;
   unsigned char* p_message;

   /**
    * socket to respond at */
   int p_fd;

   /**
    * pointer to a tcp_server or other types of
    * servers, i.e. UDP
    */
   std::shared_ptr<server> p_srv;

   /**
    * pointer to a processing instance:
    * test_worker, registration_worker and other
    * derivates from worker 
    */
   std::shared_ptr<worker> p_worker;

   /**
    * reply the result of processing to the client
    */
   void process_reply_result();

public:
    explicit handle_request(int fd, std::shared_ptr<server> p_server);

    handle_request( handle_request& ) = delete;
    handle_request& operator= ( handle_request& ) = delete;

    void process_request();
};
#endif
