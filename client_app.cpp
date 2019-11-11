#ifndef CLIENT_APP_CPP
#define CLIENT_APP_CPP
#include <type_traits>
#include "client_app.h"
#include "exception.h"
#define TIMEOUT 3

/**
 * receiver buffer 
 */
char receive_buff[MESSAGE_MAX_SIZE];

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

/**
 * client app class
 */
template <typename T>
client_app<T>::client_app(client_type c_type, void (*cb)(const request_result_t&)) 
	: p_cl_type(c_type), p_cb(cb) {
}

template<typename T> unsigned char* client_app<T>::prepare_request(const T& request) {
 
    /**
     * allocate space for request
     */
    p_message = get_request_buffer<T>();
    assert(p_message != NULL);
    this->p_message_length = sizeof(message_header_t) + sizeof(T);

    /**
     * copy request info after the header
     */
    memcpy(p_message + sizeof(message_header_t), (unsigned char*)&request, sizeof(T));
    return p_message;
}

template <typename T> unsigned char* client_app<T>::send_request(const T& msg_request) {

   p_message = prepare_request(msg_request);
   assert(p_message != NULL);

   if (p_cl_type == client_type::CLIENT_TCP) {
       p_cl = std::shared_ptr<client_tcp>(new client_tcp(PORT));
   }
   assert(p_cl != NULL);

   try {
       p_cl->client_connect();

       /**
	* send request
	*/
       if (p_cl->send_message(this->p_message, this->p_message_length) >0) {
          std::thread thread_l(&client_app::thread_wait_result, this);
	  thread_l.join();
       }
   }
   catch (const client_exception& e) {
       std::cout << e.get_message() << std::endl;
       p_cb({0xFFFFFFFF,result::ERROR});
   }
   delete [] p_message;
}

template <typename T> void client_app<T>::thread_wait_result() {

   request_result_t *p_request_result;
   /**
    * read the result 
    */
   if (p_cl->read_message(&receive_buff[0], TIMEOUT) > 0) {
      p_request_result = reinterpret_cast<request_result_t*>(&receive_buff[0]);
      p_cb(*p_request_result);
   }
}
#endif
