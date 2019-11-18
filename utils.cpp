#include <assert.h>
#include <type_traits>
#include <string.h>
#include "messages.h"
/**
 * allocate space for a request: header + message body
 */
template<class T> unsigned char* get_request_buffer() {

   message_header_t msg_head_l;
   unsigned char* p_request_l = new unsigned char[sizeof(message_header_t) + sizeof(T)];

   assert(p_request_l);
   if (std::is_same<T, message_test_t>::value) {
       msg_head_l.message_id = message_ids::TEST;
   }
   else if (std::is_same<T, message_registration_t>::value) {
       msg_head_l.message_id = message_ids::REGISTRATION;
   }
   else {
       assert(0);
   }
   msg_head_l.message_size = sizeof(T);
   memcpy((void*)p_request_l, (void*)&msg_head_l, sizeof(message_header_t));
   return p_request_l;
}

template unsigned char* get_request_buffer<message_test_t>();
template unsigned char* get_request_buffer<message_registration_t>();

