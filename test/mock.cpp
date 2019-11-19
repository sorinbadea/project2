#include <iostream>
#include "messages.h"
#include "mock.h"

/**
 * mock test_worker
 */
test_worker::test_worker(const message_test_t& m_t) : mt(m_t) {
}
request_result_t test_worker::process(bool b) {
   request_result_t res = {0x0, result::SUCCESS};
   std::cout << "message \"test\" passed" << std::endl;
   return res;
}

/**
 * mock the registration worker
 */
registration_worker::registration_worker(const message_registration_t& m_r) : mr(m_r) {
}
request_result_t registration_worker::process(bool b) {
   request_result_t res = {0x0, result::SUCCESS};
   std::cout << "message \"registration\" passed" << std::endl;
   return res;
}
