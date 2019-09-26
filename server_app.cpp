#include "server_app.h"

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

    /**
     * thread handling a request
     */
    std::thread thread_l(&server_app::thread_handle_request, this, iterations);
	
    try {
        p_srv->server_setup();
        p_srv->server_listen();
        p_fd = -1;

        while (iterations--) {

            #ifdef CLS_DEBUG
                std::cout << "waiting for accept.." << std::endl;
            #endif
	    {
                p_fd = p_srv->server_wait();
	        std::unique_lock<std::mutex> lock(p_thread1);
	    }
	    /**
	     * unlock handle request thread */
            p_cv_thread1.notify_one();

	    /**
	     * wait for the handle_request to finish 
	     */
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

    thread_l.join();
}

void server_app::thread_handle_request(int iterations) {

   while (iterations--) {
      {
         std::cout << "handle request 1.." << std::endl;
         std::unique_lock<std::mutex> lock(p_thread1);
	 while (p_fd == -1) 
             p_cv_thread1.wait(lock);
      }
      handle_request request(p_fd, p_srv);
      request.process_request();
      p_fd = -1;
      std::unique_lock<std::mutex> lock(p_main_loop);
      p_handle_request_ready = true;
      /**
       * unlock main_loop 
       */
      p_cv_main_loop.notify_one();
   }
}

/**
 * handle request impl.
 */
handle_request::handle_request
(int fd, std::shared_ptr<server> p_server) : p_fd(fd), p_srv(p_server) {
    assert(p_fd > 0);
    assert(p_server != NULL);
}	

void handle_request::process_request() {
    int bytes_read_l;

    bytes_read_l = p_srv->cls_read(p_fd, (void*)&p_message_request[0], MESSAGE_MAX_SIZE);
    if (bytes_read_l <= 0) {
        std::cout << "server, read socket error" << std::endl;
    }
    else { 
        /**
	 * check header info
	 */	
        p_header = new unsigned char[sizeof(message_header_t)];
        memcpy((void*)(p_header), (void*)&p_message_request[0], (ssize_t)sizeof(message_header_t));
        message_header_t* p_msg_header_l = reinterpret_cast<message_header_t*>(p_header);

        if (p_msg_header_l->message_id == message_ids::TEST) {

            message_test_t* p_msg_test_l = get_message_buffer<message_test_t>();
            /**
             * prepare a test worker instance
     	     */
            p_worker = std::shared_ptr<test_worker>(new test_worker(*p_msg_test_l));
            process_reply_result();
            delete []p_message;
       }
       else if (p_msg_header_l->message_id == message_ids::REGISTRATION) {

            message_registration_t* p_msg_registration_l = get_message_buffer<message_registration_t>();
            /**
             * prepare a registration worker instance
             */
	    p_worker = std::shared_ptr<registration_worker>(new registration_worker(*p_msg_registration_l));
            process_reply_result();
            delete []p_message;
       }
       delete []p_header;
    }
}

void handle_request::process_reply_result() {

    assert(p_worker != NULL);
    /** process the request by the worker */
    request_result_t res_l = p_worker->process();

    /** reply to client */
    ssize_t written_l = p_srv->cls_write(p_fd, (void*)&res_l, sizeof(request_result_t));
    std::cout << "server " << written_l << " bytes replied.." << std::endl;

    /** free file descriptor */
    p_srv->fd_close(p_fd);
}

template <typename T>
T* handle_request::get_message_buffer() {
    unsigned char* buff = &p_message_request[0] + sizeof(message_header_t);
    p_message = new unsigned char[sizeof(T)];
    memcpy((void*)p_message, (void*)buff, (ssize_t)sizeof(T));
    return reinterpret_cast<T*>(p_message);
}

