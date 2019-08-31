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

public:
   /**
    * client app constructor
    * @param c_type: tcp or udp client
    */
   explicit client_app(const client_type c_type);
   client_app(const client_app& ) = delete;
   client_app& operator= (const client_app& ) = delete;

   /**
    * prepare a test request
    * @param test_id: test_id
    * @param items: nb of repetitions
    * @param threshold: threshold
    */
   void request_test(const unsigned int test_id, const unsigned int items, const float threshold);

   /**
    * send the  request and returns the result
    * @return - NULL in case of I/O errors
    *         - the result of the request
    */
   request_result_t* send_request();
};

#endif
