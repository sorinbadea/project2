#ifndef CLIENT_APP_H
#define CLIENT_APP_H
#include <assert.h>
#include <memory>
#include <thread>
#include "client.h"
#include "messages.h"

template <class T> unsigned char* get_request_buffer();

template <typename T>
class client_app {
   unsigned char* p_message;         /** request buffer */
   unsigned int p_message_length;    /** request length */
   client_type p_cl_type;            /** client type, udp or tcp */
   std::shared_ptr<client_tcp> p_cl; /** tcp client instance */

   void thread_wait_result();        /**  thread waiting the server response */
   std::thread thread_p;

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
   void send_request(const T& msg_request);
};

#endif
