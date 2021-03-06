#ifndef SERVER_APP_H
#define SERVER_APP_H

#ifndef U_TEST
/** non TU compilation */
#include "server.h"
#else
/** TU compilation */
#include <iostream>
#include <memory>
#include <assert.h>
#include <string.h>
#include "test/mock.h"
#endif

#ifndef U_TEST
/** non TU compilation */
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

/**
 * this class not necessary in case of TU runing;
 * server application impl. using the server class services;
 */ 
class server_app {

   std::shared_ptr<server> p_srv;  /** server_tcp instance */

   std::mutex p_thread1;           /** mutex for threads synchronization */
   std::mutex p_main_loop;         

   /**
    * thread proccesing the request condition variable */
   std::condition_variable p_cv_thread1;

   /**
    * main loop condition variable */
   std::condition_variable p_cv_main_loop;

   /**
    * the processing thread done it's job
    */
   bool p_handle_request_ready = false;

   int p_fd;              /** socket to respond at */

   std::thread thread_p;  /** thread handling a request */

   unsigned char p_message_request[MESSAGE_MAX_SIZE];

public:

   server_app(const server_app& ) = delete;
   server_app& operator=(const server_app&) = delete;
   ~server_app();

   /**
    * \description
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
   void thread_handle_request(int);
};
#endif  /*for PRODUCTION */

/**
 * class handling request, TEST, REGISTRATION,..
 */
class handle_request {

   /**
    * request deserialization pointers
    */
   unsigned char* p_header;
   unsigned char* p_message;

   /**
    * socket to respond at */
   int p_fd;

   /**
    * pointer to base class server hierarchy impl.*/
   std::shared_ptr<server> p_srv;

   /**
    * pointer to a processing instance:
    * test_worker, registration_worker and other
    * derivates from worker */
   std::shared_ptr<worker> p_worker;

   /**
    * reply the result of processing to the client */
   request_result_t process_reply_result();
  
   /**
    * generic method to allocate the resource request
    */
   template <typename T>
   T* get_message_buffer(unsigned char*); 

public:
    /**
     * handle request constructor
     * @param fd: socket to respond at
     * @param p_server: reference to a server instance, UDP or TCP
     */
    explicit handle_request(int fd, std::shared_ptr<server> p_server);
    handle_request( handle_request& ) = delete;
    handle_request& operator= ( handle_request& ) = delete;

    /**
     * constructor used in case of TU */
    handle_request(std::shared_ptr<server>);

    /**
     * decode and dispatch the process request 
     * @param request: received request */
    request_result_t process_request(unsigned char*request);
};
#endif
