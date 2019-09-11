#include "worker.h"

test_worker::test_worker(const message_test_t& msg)
       : p_msg_test(msg) {
}

request_result_t test_worker::process() {

   std::cout << "request: TEST" << std::endl;
   std::cout << "Test id:" << p_msg_test.test_id
             << " test item:" << p_msg_test.items
             << " threshold:" << p_msg_test.threshold
             << std::endl;
    /**
     * response 
     */
    p_result.message_id = p_msg_test.test_id;
    p_result.res = result::SUCCESS;
    return p_result;
}

registration_worker::registration_worker(const message_registration_t& msg )
       : p_msg_registration(msg) {
}

request_result_t registration_worker::process() {

   std::cout << "request: REGISTRATION" << std::endl;
   std::cout << "message id:" << p_msg_registration.message_id
             << " network id:" << p_msg_registration.network_id
             << std::endl;

    p_result.message_id = p_msg_registration.message_id;
    p_result.res = result::SUCCESS;
    return p_result;
}


