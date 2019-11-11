#ifndef CLIENT_APP_H
#define CLIENT_APP_H
#include <assert.h>
#include <memory>
#include "client.h"
#include "messages.h"

template <class T> unsigned char* get_request_buffer();

template <typename T>
class client_app {
   /**
    * request buffer
    */
   unsigned char* p_message;
   /**
    * request length
    */
   unsigned int p_message_length;

   /**
    * stores the client type, udp or tcp
    */
   client_type p_cl_type;

   /**
    * tcp client instance  
    */
   std::shared_ptr<client_tcp> p_cl;

   /**
    * prepare a request, TEST, REGISTRATION, etc..
    * @param msg_test - test parameters
    */
   unsigned char* prepare_request(const T& request);

   /**
    * result callback
    * @param request_result_t - request result
    */
   void (*p_cb)(const request_result_t&);

   /**
    * thread reading the server response
    */
   void thread_wait_result();

public:
   /**
    * client app constructor
    * @param c_type: tcp or udp client
    */
   explicit client_app(const client_type c_type, void(*cb)(const request_result_t&));
   client_app(const client_app& ) = delete;
   client_app& operator= (const client_app& ) = delete;

   /**
    * send the request and returns the result
    * @param: - msg_request request type, TEST, REGISTRATION..
    * @return - NULL in case of I/O errors
    *         - the result of the request
    */
   unsigned char* send_request(const T& msg_request);
};

#endif
