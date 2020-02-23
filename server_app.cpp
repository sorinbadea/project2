#include "server_app.h"

/**
 * this class shall be ignored in case of TU compilation
 */
#ifndef U_TEST
/**
 * server_app impl.
 */
server_app::server_app ( const server_type s_type, 
                         const unsigned int port, 
                         const unsigned int q_size ) {

   if (s_type == server_type::SERVER_TCP) {
       p_srv = std::shared_ptr<server_tcp>(new server_tcp(port, q_size));
   }
   else if (s_type == server_type::SERVER_UDP) {
       p_srv = std::shared_ptr<server_udp>(new server_udp(port, q_size));
   }
   else {
      assert(0);
   }
}

server_app::~server_app() {
}

void server_app::start (){

    int fd_l;
    int iterations = 99;

    try {
       p_srv->server_setup();
       p_srv->server_listen();
       p_fd = -1;

       /**
       * thread handling a request
       */
       thread_p = std::thread(&server_app::thread_handle_request, this, iterations);

       while (iterations--) {

            std::cout << "waiting for accept.." << std::endl;
	    {
                p_fd = p_srv->server_wait();
	        std::unique_lock<std::mutex> lock(p_thread1);
	    }
	    /**
	     * unlock handle request thread */
            p_cv_thread1.notify_one();
	    {
	       std::unique_lock<std::mutex> lock(p_main_loop);
               while(p_handle_request_ready == false)
                   p_cv_main_loop.wait(lock);
	    }
	    p_handle_request_ready = false;
        }
    }
    catch(const server_exception& e) {
       std::cout << e.get_message() << std::endl;
    }
    if (thread_p.joinable()) {
       thread_p.join();
    }
}

void server_app::thread_handle_request(int iterations) {

   while (iterations--) {
      {
         std::cout << "handle request.." << iterations << std::endl;
         std::unique_lock<std::mutex> lock(p_thread1);
	 while (p_fd == -1) 
             p_cv_thread1.wait(lock);
      }
      std::unique_lock<std::mutex> lock(p_main_loop);
      p_handle_request_ready = true;
      /**
       * unlock the main_loop 
       */
      p_cv_main_loop.notify_one();
      /**
       * handle the new request 
       */
      int bytes_read_l = p_srv->cls_read(p_fd, (void*)&p_message_request[0], MESSAGE_MAX_SIZE);
      if (bytes_read_l <= 0) {
         std::cout << "server, read socket error" << std::endl;
      }
      else {  
         handle_request request(p_fd, p_srv);
         (void)request.process_request(&p_message_request[0]);
      }
      p_fd = -1;
   }
}
#endif  /* production CODE */

/**
 * handle request impl.
 */
handle_request::handle_request (int fd, std::shared_ptr<server> p_server) : p_fd(fd), p_srv(p_server) {
    assert(p_fd > 0);
    assert(p_server != NULL);
}

handle_request::handle_request(std::shared_ptr<server> p_server) : p_srv(p_server) {
    /**
     * this constructor is called in case of TU mocking
     */
}

request_result_t handle_request::process_request(unsigned char* p_request) {

     assert(p_request != NULL);
     request_result_t res_l;
     /**
     * check header info
     */	
     p_header = new unsigned char[sizeof(message_header_t)];
     memcpy((void*)(p_header), (void*)p_request, (ssize_t)sizeof(message_header_t));
     message_header_t* p_msg_header_l = reinterpret_cast<message_header_t*>(p_header);

     if (p_msg_header_l->message_id == message_ids::TEST) {

         message_test_t* p_msg_test_l = get_message_buffer<message_test_t>(p_request);
         /**
          * prepare a test worker instance
          */
         p_worker = std::shared_ptr<test_worker>(new test_worker(*p_msg_test_l));
         res_l = process_reply_result();
         delete []p_message;
     }
     else if (p_msg_header_l->message_id == message_ids::REGISTRATION) {

         message_registration_t* p_msg_registration_l = get_message_buffer<message_registration_t>(p_request);
         /**
         * prepare a registration worker instance
         */
         p_worker = std::shared_ptr<registration_worker>(new registration_worker(*p_msg_registration_l));
         res_l = process_reply_result();
         delete []p_message;
    }
    else {
         std::cout << "Unknown request!" << std::endl;
	 res_l.res = result::ERROR;
    }
    delete []p_header;
    return res_l;
}

request_result_t handle_request::process_reply_result() {

    assert(p_worker != NULL);
    /** process the request by the worker */
    request_result_t res_l = p_worker->process();
    /** reply to client */
    ssize_t written_l = p_srv->cls_write(p_fd, (void*)&res_l, sizeof(request_result_t));
    std::cout << "server " << written_l << " bytes replied.." << std::endl;

    /** free file descriptor */
    p_srv->fd_close(p_fd);
    return res_l;
}

template <typename T>
T* handle_request::get_message_buffer(unsigned char* p_request) {

    unsigned char* buff = p_request + sizeof(message_header_t);
    p_message = new unsigned char[sizeof(T)];
    memcpy((void*)p_message, (void*)buff, (ssize_t)sizeof(T));
    return reinterpret_cast<T*>(p_message);
}

