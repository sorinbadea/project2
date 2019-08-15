#include <thread>
#include "client_app.h"


void send_request() {
    
    int loops = 3;
    static int test_id = 0;
    client_app a;
    while( loops-- ) {
       a.request_test(test_id++, 2, 3.33);
       a.send_request();
    }
}

int main() {

    std::thread thread_l1(&send_request);
    std::thread thread_l2(&send_request);
    std::thread thread_l3(&send_request);
    std::thread thread_l4(&send_request);
    thread_l1.join();
    thread_l2.join();
    thread_l3.join();
    thread_l4.join();
    return 0;
} 
