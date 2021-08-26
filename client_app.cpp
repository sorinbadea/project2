#ifndef CLIENT_APP_CPP
#define CLIENT_APP_CPP
#include "client_app.h"
#include "exception.h"
#define TIMEOUT 3

/**
 * receiver buffer 
 */
char receive_buff[MESSAGE_MAX_SIZE];

/**
 * client app class
 */
template <typename T>
client_app<T>::client_app(client_type c_type, void (*cb)(const request_result_t&)) 
	: p_cl_type(c_type), p_cb(cb) {
}

template<typename T> 
unsigned char* client_app<T>::prepare_request(const T& request) {
 
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

template <typename T> 
void client_app<T>::send_request(const T& msg_request) {

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
          thread_p = std::thread(&client_app::thread_wait_result, this);
       }
   }
   catch (const client_exception& e) {
       std::cout << e.get_message() << std::endl;
       p_cb({0xFFFFFFFF,result::ERROR});
   }
   if (thread_p.joinable()) {
     thread_p.join();
   }
   delete [] p_message;
}

template <typename T> 
void client_app<T>::thread_wait_result() {

   request_result_t *p_result_ok;
   /**
    * read the result 
    */
   if (p_cl->read_message(&receive_buff[0], TIMEOUT) > 0) {
      p_result_ok = reinterpret_cast<request_result_t*>(&receive_buff[0]);
      p_cb(*p_result_ok);
   }
   else {
      p_cb({0xFFFFFFFF,result::ERROR});
   }
}
#endif
