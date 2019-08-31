#include "server_app.h"

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
    assert(p_srv != NULL);

    /**
     * thread handling a request
     */
    std::thread thread_l1(&server_app::handle_request1, this, iterations);
	
    try {
        p_srv->server_setup();
        p_srv->server_listen();
        p_fd1 = -1;

        while( iterations-- ) {
            #ifdef CLS_DEBUG
                std::cout << "waiting for accept.." << std::endl;
            #endif
	    {
	        std::unique_lock<std::mutex> lock(p_thread1);
                p_fd1 = p_srv->server_wait();
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

    thread_l1.join();
}

void server_app::handle_request1(int iterations) {

   while (iterations--) {
      {
         std::cout << "handle request 1.." << std::endl;
         std::unique_lock<std::mutex> lock(p_thread1);
	 while(p_fd1 == -1) 
             p_cv_thread1.wait(lock);
      }
      handle_request request(p_fd1, p_srv);
      request.process_request();
      p_fd1 = -1;
      std::unique_lock<std::mutex> lock(p_main_loop);
      p_handle_request_ready = true;
      /**
       * unlock main_loop */
      p_cv_main_loop.notify_one();
   }
}

/**
 * handle request impl.
 */
handle_request::handle_request(int fd, std::shared_ptr<server> p_server) : p_fd(fd), p_srv(p_server) {
}	

void handle_request::process_request() {
    int bytes_read_l;
    unsigned char message_l[MESSAGE_MAX_SIZE];
    unsigned char* buff = &message_l[0];

    bytes_read_l = p_srv->cls_read(p_fd, (void*)message_l, MESSAGE_MAX_SIZE);
    if (bytes_read_l <= 0) {
        std::cout << "server, read socket error" << std::endl;
    }
    else { 
	
        p_header = new unsigned char[sizeof(message_header_t)];
        memcpy((void*)(p_header), (void*)message_l, (ssize_t)sizeof(message_header_t));
        message_header_t* p_msg_header_l = reinterpret_cast<message_header_t*>(p_header);

        if (p_msg_header_l->message_id == message_ids::TEST) {

            p_message = new unsigned char[sizeof(message_test_t)];
            buff += sizeof(message_header_t);
            memcpy((void*)p_message, (void*)buff, (ssize_t)sizeof(message_test_t));
            message_test_t* p_msg_test = reinterpret_cast<message_test_t*>(p_message);

            /**
             * prepare a test worker instance
     	     */
            worker* worker_l = new test_worker(p_msg_test->test_id, 
                        p_msg_test->items, 
                        p_msg_test->threshold);

            delete []p_message;

            /** process the request and reply */
            request_result_t res_l = worker_l->process();

            //dummy reply
            ssize_t written_l = p_srv->cls_write(p_fd, (void*)&res_l, sizeof(request_result_t));
            std::cout << "server " << written_l << " bytes replied.." << std::endl;
   
            delete worker_l;

            /** free file descriptor */
            p_srv->fd_close(p_fd);
       }
       delete []p_header;
    }
}

