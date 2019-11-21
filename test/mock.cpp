#include <iostream>
#include "mock.h"

/**
 * mock test_worker
 */
test_worker::test_worker(const message_test_t& m_t) : mt(m_t) {
}

request_result_t test_worker::process() {
   request_result_t res = {0x0, result::SUCCESS};
   std::cout << "message \"test\" passed" << std::endl;
   return res;
}

/**
 * mock the registration worker
 */
registration_worker::registration_worker(const message_registration_t& m_r) : mr(m_r) {
}

request_result_t registration_worker::process() {
   request_result_t res = {0x0, result::SUCCESS};
   std::cout << "message \"registration\" passed" << std::endl;
   return res;
}

/**
 * mock the server_tcp impl.
 */
ssize_t server_tcp::cls_write(int fd, void* message_l, size_t size) {
   std::cout << "server_tcp \"write\" passed" << std::endl;
   return 1;
}

void server_tcp::fd_close(int fd) {
   std::cout << "server_tcp \"closed\" passed" << std::endl;
}
