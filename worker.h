#ifndef WORKER_H
#define WORKER_H
#include <iostream>
#include "messages.h"

class worker {
public:
   virtual request_result_t process() = 0;
};

class test_worker : public worker {

    /*
     * request result 
     **/
    request_result_t p_result;

    /*
     * message test parameters
     **/
    message_test_t p_msg_test;

public:
   explicit test_worker(const message_test_t&);

   request_result_t process();
};

class registration_worker : public worker {

    /*
     * request result 
     **/
    request_result_t p_result;

    /*
     * message registration parameters
     **/
    message_registration_t p_msg_registration;

public:
   explicit registration_worker(const message_registration_t&);

   request_result_t process();
};
#endif
