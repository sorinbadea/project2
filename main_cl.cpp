#include <thread>
#include "client_app.h"

void send_request() {
    
    int loops = 3;
    static int test_id = 0;
    static int items = 2;
    static float threshold = 3.32;
    message_test_t message;
    request_result_t* p_res;

    client_app app(client_type::CLIENT_TCP);

    while (loops--) {

       message.test_id = test_id++;
       message.items = items;
       message.threshold = threshold;

       p_res = app.send_request(message);

       if (p_res != NULL) {
          if (p_res->res == result::SUCCESS) {
             std::cout << "message id:" << p_res->message_id << " OK" << std::endl;
          }
	  else {
              std::cout << "message id:" << p_res->message_id << " KO" << std::endl;
	  }
       }
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
