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
     * test result 
     **/
    request_result_t p_result;

    /*
     * test parameters
     **/
    unsigned int p_test_id;
    unsigned int p_items;
    float        p_threshold;

public:
   explicit test_worker( unsigned int, unsigned int, float );

   request_result_t process();
};

#endif
