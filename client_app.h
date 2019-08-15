#ifndef CLIENT_APP_H
#define CLIENT_APP_H
#include <memory>
#include "client.h"
#include "messages.h"
#include "exception.h"

class client_app {
   /**
    * request buffer
    */
   unsigned char* p_message;
   /**
    * request length
    */
   unsigned int p_message_length;

public:
   client_app();
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
    * send the  request
    */
   void send_request();
};

#endif
