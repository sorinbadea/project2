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
 * allocate space for a request
 */
template<typename T>
unsigned char* get_request_buffer() {

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

/**
 * client app class
 */
template <typename T>
client_app<T>::client_app(client_type c_type) : p_cl_type(c_type) {
}

template<typename T>
void client_app<T>::prepare_request(const T& request) {
   
    unsigned char* request_l;
 
    /**
     * allocate space for request
     */
    this->p_message = get_request_buffer<T>();
    request_l = p_message;
    this->p_message_length = sizeof(message_header_t) + sizeof(T);

    /**
     * copy the request header
     */
    request_l += sizeof(message_header_t);
    memcpy(request_l, (unsigned char*)&request, sizeof(T));
}

template <typename T>
request_result_t* client_app<T>::send_request(const T& msg_request) {

   this->prepare_request(msg_request);

   if (p_cl_type == client_type::CLIENT_TCP) {
       p_cl = std::shared_ptr<client_tcp>(new client_tcp(PORT));
   }
   assert(p_cl != NULL);

   try {
       p_cl->client_connect();

       std::cout << "sent " << p_cl->send_message(this->p_message, this->p_message_length) << " bytes" << std::endl;

       if (p_cl->read_message(&receive_buff[0], TIMEOUT) > 0) {
           p_request_result = reinterpret_cast<request_result_t*>(&receive_buff[0]);
       }
   }
   catch (const client_exception& e) {
       std::cout << e.get_message() << std::endl;
       p_request_result = NULL;
   }
   delete [] p_message;
   return p_request_result;
}
#endif
