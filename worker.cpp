#include "worker.h"

test_worker::test_worker( unsigned int test_id,
                          unsigned int test_items,
                          float threshold ) :
                          p_test_id( test_id ), 
			  p_items( test_items ), 
			  p_threshold( threshold ) {
}

request_result_t test_worker::process() {

   std::cout << "request: TEST" << std::endl;
   std::cout << "Test id:" << p_test_id
             << " test item:" << p_items
             << " threshold:" << p_threshold 
             << std::endl;

    p_result.message_id = p_test_id;
    p_result.res = result::SUCCESS;
    return p_result;
}

