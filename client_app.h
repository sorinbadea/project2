#ifndef CLIENT_APP_H
#define CLIENT_APP_H
#include <assert.h>
#include <memory>
#include "client.h"
#include "messages.h"

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
    * udp client instance  
    */
   std::shared_ptr<client_tcp> p_cl;

   /**
    * prepare a request, TEST, REGISTRATION, etc..
    * @param msg_test - test parameters
    */
   template <typename T>
   void prepare_request(const T& request);

public:
   /**
    * client app constructor
    * @param c_type: tcp or udp client
    */
   explicit client_app(const client_type c_type);
   client_app(const client_app& ) = delete;
   client_app& operator= (const client_app& ) = delete;

   /**
    * send the request and returns the result
    * @param: - test parameters
    * @return - NULL in case of I/O errors
    *         - the result of the request
    */
   request_result_t* send_request(const message_test_t& msg_test);
};

#endif
